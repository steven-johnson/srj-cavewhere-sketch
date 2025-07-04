#include "InfiniteGridModel.h"

//Qt incudes
#include <QScopedPropertyUpdateGroup>

using namespace cwSketch;

InfiniteGridModel::InfiniteGridModel(QObject *parent)
    : QObject(parent)
    , m_majorGrid(new FixedGridModel(this))
    , m_minorGrid(new FixedGridModel(this))
{
    m_minorGrid->setObjectName(QStringLiteral("minorGridModel"));
    m_majorGrid->setObjectName(QStringLiteral("majorGridModel"));

    //Orders the grids and labels so they're interleved
    m_majorGrid->setLabelZ(2);
    m_minorGrid->setLabelZ(2);
    m_majorGrid->setLabelBackgroundZ(1);
    m_minorGrid->setLabelBackgroundZ(1);
    m_majorGrid->setGridZ(0);
    m_minorGrid->setGridZ(0);

    //For hidding origin labels
    m_originMask.setBinding([this]() {
        QSizeF size = m_majorGrid->maxLabelSizePixels();
        size.setHeight(size.height() * 0.5);
        size.setWidth(size.width() * 0.25);
        QRectF viewport = m_viewport.value();
        QPointF bottomLeft = viewport.bottomLeft();
        return QRectF(QPointF(bottomLeft.x(), bottomLeft.y() - size.height()),
                      size);
    });

    m_majorGrid->bindableLabelMasks().setBinding([this]() {
        return QList<QRectF>({m_originMask});
    });

    m_minorGrid->bindableLabelMasks().setBinding([this]() {
        return m_majorGrid->labelMasks();
    });

    // Individual bindings
    m_majorGrid->bindableViewport().setBinding([this]() {
        return m_viewport.value();
    });
    m_minorGrid->bindableViewport().setBinding([this]() {
        return m_viewport.value();
    });

    m_majorGrid->bindableLabelColor().setBinding([this]() {
        return m_labelColor.value();
    });

    auto clampLightness = [](double factor) -> int {
        return std::max(100, std::min(200, (int)(100.0 + factor * 100.0)));
    };

    m_minorGrid->bindableLabelColor().setBinding([this, clampLightness]() {
        return m_labelColor.value().lighter(clampLightness(m_minorColorLightness));
    });

    m_majorGrid->bindableLineColor().setBinding([this]() {
        return m_lineColor.value();
    });
    m_minorGrid->bindableLineColor().setBinding([this, clampLightness]() {
        return m_lineColor.value().lighter(clampLightness(m_minorColorLightness));
    });

    m_majorGrid->bindableLabelFont().setBinding([this]() {
        return m_labelFont.value();
    });
    m_minorGrid->bindableLabelFont().setBinding([this]() {
        auto font =  m_labelFont.value();
        font.setPointSizeF(font.pointSizeF() * m_labelMinorScale.value());
        return font;
    });

    m_majorGrid->bindableLabelBackgroundMargin().setBinding([this]() {
        return m_labelBackgroundMargin.value();
    });
    m_minorGrid->bindableLabelBackgroundMargin().setBinding([this]() {
        return m_labelBackgroundMargin.value();
    });

    m_majorGrid->bindableLabelBackgroundColor().setBinding([this]() {
        return m_labelBackgroundColor.value();
    });
    m_minorGrid->bindableLabelBackgroundColor().setBinding([this]() {
        return m_labelBackgroundColor.value();
    });

    m_majorGrid->bindableLabelScale().setBinding([this]() {
        return m_viewScale.value();
    });
    m_minorGrid->bindableLabelScale().setBinding([this]() {
        return m_viewScale.value();
    });

    m_majorGrid->bindableMapMatrix().setBinding([this]() {
        return m_mapMatrix.value();
    });
    m_minorGrid->bindableMapMatrix().setBinding([this]() {
        return m_mapMatrix.value();
    });

    m_majorGrid->bindableLineWidth().setBinding([this]() {
        double lineWidth = m_lineWidth.value();
        return std::max(lineWidth, lineWidth * m_viewScale.value());
    });

    m_minorGrid->bindableLineWidth().setBinding([this]() {
        auto lerp = [](double start, double end, double t) {
            return start + t * (end - start);
        };

        const double minorLineWidthScale = 0.5;
        const double viewScale = m_viewScale.value();

        double minorLineWidth = m_lineWidth.value() * minorLineWidthScale;
        double minorLineWidthWithScale = minorLineWidth * viewScale;
        double majorLineWidth = m_majorGrid->lineWidth();

        //Rescale the m_zoomTransition, only use transition if we're above zoom level 1.0
        if(m_zoomLevel >= 1.0) {
            const double transition = std::min(1.0, m_zoomTransition * 2.0);
            return lerp(majorLineWidth, minorLineWidthWithScale, transition);
        } else {
            return minorLineWidth;
        }
    });

    m_minorGrid->bindableLabelVisible().setBinding([this]() {
        double minorGridPixels = m_minorGrid->gridIntervalPixels();
        double margin = m_labelMinorMargin.value() * 2.0; //pixels
        // qDebug() << "Minor grid:" << minorGridPixels << "label size:" << m_minorGrid->maxLabelSizePixels().width() << "margin:" << margin << "visible:" << (minorGridPixels > m_minorGrid->maxLabelSizePixels().width() + margin);
        return minorGridPixels > m_minorGrid->maxLabelSizePixels().width() + margin;
    });

    // m_minorGrid->setLabelVisible(false);

    auto interval = [](double base, double zoomLevel) {
        return base * std::pow(10, zoomLevel);
    };

    m_majorZoomGridInterval.setBinding([this, interval]() {
        return interval(m_majorGridInterval.value(), m_clampedZoomLevel.value());
    });

    m_minorZoomGridInterval.setBinding([this, interval]() {
        return interval(m_minorGridInterval.value(), m_clampedZoomLevel.value());
    });

    auto updateMajorInterval = [this]() {
        m_majorGrid->gridInterval()->setValue(m_majorZoomGridInterval.value());
        m_minorGrid->setHiddenInterval(m_majorZoomGridInterval.value());
    };
    m_majorZoomGridIntervalNotifier = m_majorZoomGridInterval.addNotifier(updateMajorInterval);
    updateMajorInterval();

    auto updateMinorInterval = [this]() {
        m_minorGrid->gridInterval()->setValue(m_minorZoomGridInterval.value());
    };
    m_minorZoomGridIntervalNotifier = m_minorZoomGridInterval.addNotifier(updateMinorInterval);
    updateMinorInterval();

    m_zoomLevel.setBinding([this]() -> double {
        double mapScale = m_mapMatrix.value()(0,0);
        double zoomScale = m_viewScale / mapScale;
        double minPixelScale = m_minorGridMinPixelInterval / 10.0; //Uses the min pixel interval to switch to the next zoomLevel
        double offset = 2.0; //Not sure why 2.0 works, but it does
        return std::log10(zoomScale * minPixelScale) + offset;
    });

    m_zoomTransition.setBinding([this]() -> double {
        double fraction = std::fmod(m_zoomLevel, 1.0);
        return fraction;
    });

}

int InfiniteGridModel::clampZoomLevel() const
{
    // qDebug() << "Clamped Zoom level:" << std::max(0, std::min(m_zoomLevel.value(), m_maxZoomLevel.value()));
    return std::max(0, std::min((int)m_zoomLevel.value(), m_maxZoomLevel.value()));
}


FixedGridModel *InfiniteGridModel::majorGridModel() const
{
    return m_majorGrid;
}

FixedGridModel *InfiniteGridModel::minorGridModel() const
{
    return m_minorGrid;
}

TextModel *InfiniteGridModel::majorTextModel() const
{
    return m_majorGrid->textModel();
}

TextModel *InfiniteGridModel::minorTextModel() const
{
    return m_minorGrid->textModel();
}
