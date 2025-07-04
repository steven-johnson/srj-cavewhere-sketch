#pragma once

// Qt includes
#include <QColor>
#include <QMatrix4x4>
#include <QPointF>
#include <QRectF>
#include <QObject>
#include <QObjectBindableProperty>
#include <QConcatenateTablesProxyModel>

// Your includes
#include "AbstractPainterPathModel.h"
#include "FixedGridModel.h"

namespace cwSketch {

class InfiniteGridModel : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(double majorGridInterval READ majorGridInterval WRITE setMajorGridInterval NOTIFY majorGridIntervalChanged BINDABLE bindableMajorGridInterval)
    Q_PROPERTY(double minorGridInterval READ minorGridInterval WRITE setMinorGridInterval NOTIFY minorGridIntervalChanged BINDABLE bindableMinorGridInterval)
    Q_PROPERTY(int maxZoomLevel READ maxZoomLevel WRITE setMaxZoomLevel NOTIFY maxZoomLevelChanged BINDABLE bindableMaxZoomLevel)
    Q_PROPERTY(double minorGridMinPixelInterval READ minorGridMinPixelInterval WRITE setMinorGridMinPixelInterval NOTIFY minorGridMinPixelIntervalChanged BINDABLE bindableMinorGridMinPixelInterval)

    Q_PROPERTY(double lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged BINDABLE bindableLineWidth)
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged BINDABLE bindableLineColor)
    Q_PROPERTY(QColor labelColor READ labelColor WRITE setLabelColor NOTIFY labelColorChanged BINDABLE bindableLabelColor)
    Q_PROPERTY(QFont labelFont READ labelFont WRITE setLabelFont NOTIFY labelFontChanged BINDABLE bindableLabelFont)
    Q_PROPERTY(double labelBackgroundMargin READ labelBackgroundMargin WRITE setLabelBackgroundMargin NOTIFY labelBackgroundMarginChanged BINDABLE bindableLabelBackgroundMargin)
    Q_PROPERTY(QColor labelBackgroundColor READ labelBackgroundColor WRITE setLabelBackgroundColor NOTIFY labelBackgroundColorChanged BINDABLE bindableLabelBackgroundColor)
    Q_PROPERTY(double labelMinorScale READ labelMinorScale WRITE setLabelMinorScale NOTIFY labelMinorScaleChanged BINDABLE bindableLabelMinorScale)
    Q_PROPERTY(double labelMinorMargin READ labelMinorMargin WRITE setLabelMinorMargin NOTIFY labelMinorMarginChanged BINDABLE bindableLabelMinorMargin)
    Q_PROPERTY(double minorColorLightness READ minorColorLightness WRITE setMinorColorLightness NOTIFY minorColorLightnessChanged BINDABLE bindableMinorColorLightness)

    Q_PROPERTY(double viewScale READ viewScale WRITE setViewScale NOTIFY viewScaleChanged BINDABLE bindableViewScale)
    Q_PROPERTY(QMatrix4x4 mapMatrix READ mapMatrix WRITE setMapMatrix NOTIFY mapMatrixChanged BINDABLE bindableMapMatrix)
    Q_PROPERTY(QPointF gridOrigin READ gridOrigin WRITE setGridOrigin NOTIFY gridOriginChanged BINDABLE bindableGridOrigin)
    Q_PROPERTY(QRectF viewport READ viewport WRITE setViewport NOTIFY viewportChanged BINDABLE bindableViewport)

    Q_PROPERTY(FixedGridModel* majorGridModel READ majorGridModel CONSTANT)
    Q_PROPERTY(FixedGridModel* minorGridModel READ minorGridModel CONSTANT)

    Q_PROPERTY(TextModel* majorTextModel READ majorTextModel CONSTANT)
    Q_PROPERTY(TextModel* minorTextModel READ minorTextModel CONSTANT)

    //For testing
    // Q_PROPERTY(QRectF originMask READ originMask WRITE setOriginMask NOTIFY originMaskChanged BINDABLE bindableOriginMask)

public:
    explicit InfiniteGridModel(QObject* parent = nullptr);

    double majorGridInterval() const { return m_majorGridInterval.value(); }
    void setMajorGridInterval(const double& majorGridInterval) { m_majorGridInterval = majorGridInterval; }
    QBindable<double> bindableMajorGridInterval() { return &m_majorGridInterval; }

    double minorGridInterval() const { return m_minorGridInterval.value(); }
    void setMinorGridInterval(const double& minorGridInterval) { m_minorGridInterval = minorGridInterval; }
    QBindable<double> bindableMinorGridInterval() { return &m_minorGridInterval; }

    double minorGridMinPixelInterval() const { return m_minorGridMinPixelInterval.value(); }
    void setMinorGridMinPixelInterval(const double& minorGridMinPixelInterval) { m_minorGridMinPixelInterval = minorGridMinPixelInterval; }
    QBindable<double> bindableMinorGridMinPixelInterval() { return &m_minorGridMinPixelInterval; }

    double lineWidth() const { return m_lineWidth; }
    void setLineWidth(double value) { m_lineWidth = value; }
    QBindable<double> bindableLineWidth() { return &m_lineWidth; }

    QColor lineColor() const { return m_lineColor; }
    void setLineColor(const QColor& color) { m_lineColor = color; }
    QBindable<QColor> bindableLineColor() { return &m_lineColor; }

    QColor labelColor() const { return m_labelColor; }
    void setLabelColor(const QColor& color) { m_labelColor = color; }
    QBindable<QColor> bindableLabelColor() { return &m_labelColor; }

    QFont labelFont() const { return m_labelFont; }
    void setLabelFont(const QFont& font) { m_labelFont = font; }
    QBindable<QFont> bindableLabelFont() { return &m_labelFont; }

    double labelBackgroundMargin() const { return m_labelBackgroundMargin; }
    void setLabelBackgroundMargin(double value) { m_labelBackgroundMargin = value; }
    QBindable<double> bindableLabelBackgroundMargin() { return &m_labelBackgroundMargin; }

    QColor labelBackgroundColor() const { return m_labelBackgroundColor; }
    void setLabelBackgroundColor(const QColor& color) { m_labelBackgroundColor = color; }
    QBindable<QColor> bindableLabelBackgroundColor() { return &m_labelBackgroundColor; }

    double labelMinorScale() const { return m_labelMinorScale.value(); }
    void setLabelMinorScale(const double& labelMinorScale) { m_labelMinorScale = labelMinorScale; }
    QBindable<double> bindableLabelMinorScale() { return &m_labelMinorScale; }

    double labelMinorMargin() const { return m_labelMinorMargin.value(); }
    void setLabelMinorMargin(const double& labelMinorMargin) { m_labelMinorMargin = labelMinorMargin; }
    QBindable<double> bindableLabelMinorMargin() { return &m_labelMinorMargin; }

    double viewScale() const { return m_viewScale; }
    void setViewScale(double value) { m_viewScale = value; }
    QBindable<double> bindableViewScale() { return &m_viewScale; }

    QMatrix4x4 mapMatrix() const { return m_mapMatrix; }
    void setMapMatrix(const QMatrix4x4& matrix) { m_mapMatrix = matrix; }
    QBindable<QMatrix4x4> bindableMapMatrix() { return &m_mapMatrix; }

    QPointF gridOrigin() const { return m_gridOrigin; }
    void setGridOrigin(const QPointF& origin) { m_gridOrigin = origin; }
    QBindable<QPointF> bindableGridOrigin() { return &m_gridOrigin; }

    QRectF viewport() const { return m_viewport; }
    void setViewport(const QRectF& rect) { m_viewport = rect; }
    QBindable<QRectF> bindableViewport() { return &m_viewport; }

    int maxZoomLevel() const { return m_maxZoomLevel.value(); }
    void setMaxZoomLevel(const int& maxZoomLevel) { m_maxZoomLevel = maxZoomLevel; }
    QBindable<int> bindableMaxZoomLevel() { return &m_maxZoomLevel; }

    double minorColorLightness() const { return m_minorColorLightness.value(); }
    void setMinorColorLightness(const double& minorColorLightness) { m_minorColorLightness = minorColorLightness; }
    QBindable<double> bindableMinorColorLightness() { return &m_minorColorLightness; }


    TextModelConcatenate *textModel() const;

    QHash<int, QByteArray> roleNames() const { return m_majorGrid->roleNames(); }

    FixedGridModel *majorGridModel() const;
    FixedGridModel *minorGridModel() const;

    TextModel *majorTextModel() const;
    TextModel *minorTextModel() const;

    //For testing:
    // QRectF originMask() const { return m_originMask.value(); }
    // void setOriginMask(const QRectF& originMask) { m_originMask = originMask; }
    // QBindable<QRectF> bindableOriginMask() { return &m_originMask; }

signals:
    void lineWidthChanged();
    void lineColorChanged();
    void labelColorChanged();
    void labelFontChanged();
    void labelBackgroundMarginChanged();
    void labelBackgroundColorChanged();
    void msaaScaleChanged();
    void viewScaleChanged();
    void mapMatrixChanged();
    void gridOriginChanged();
    void viewportChanged();
    void majorGridIntervalChanged();
    void minorGridIntervalChanged();
    void minorGridMinPixelIntervalChanged();
    void maxZoomLevelChanged();
    void labelMinorScaleChanged();
    void labelMinorMarginChanged();
    void minorColorLightnessChanged();

    //For testing
    // void originMaskChanged();

private:
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, double, m_majorGridInterval, 5.0, &InfiniteGridModel::majorGridIntervalChanged);
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, double, m_minorGridInterval, 1.0, &InfiniteGridModel::minorGridIntervalChanged);
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, double, m_minorGridMinPixelInterval, 10.0, &InfiniteGridModel::minorGridMinPixelIntervalChanged);
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, int, m_maxZoomLevel, 5, &InfiniteGridModel::maxZoomLevelChanged);

    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, double, m_lineWidth, 1.0, &InfiniteGridModel::lineWidthChanged)
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, QColor, m_lineColor, Qt::black, &InfiniteGridModel::lineColorChanged)
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, QColor, m_labelColor, Qt::black, &InfiniteGridModel::labelColorChanged)
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, QFont, m_labelFont, QFont(), &InfiniteGridModel::labelFontChanged)
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, double, m_labelBackgroundMargin, 2.0, &InfiniteGridModel::labelBackgroundMarginChanged)
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, QColor, m_labelBackgroundColor, Qt::white, &InfiniteGridModel::labelBackgroundColorChanged)
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, double, m_labelMinorScale, 0.75, &InfiniteGridModel::labelMinorScaleChanged);
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, double, m_labelMinorMargin, 2.0, &InfiniteGridModel::labelMinorMarginChanged);
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, double, m_minorColorLightness, 0.07, &InfiniteGridModel::minorColorLightnessChanged); //0.0 to 1.0, 1.0 very light, 0.0 is no change

    // Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, double, m_msaaScale, 1.0, &InfiniteGridModel::msaaScaleChanged)
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, double, m_viewScale, 1.0, &InfiniteGridModel::viewScaleChanged)
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, QMatrix4x4, m_mapMatrix, QMatrix4x4(), &InfiniteGridModel::mapMatrixChanged)
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, QPointF, m_gridOrigin, QPointF(), &InfiniteGridModel::gridOriginChanged)
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, QRectF, m_viewport, QRectF(), &InfiniteGridModel::viewportChanged)

    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, double, m_zoomLevel, 0.0);
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, double, m_zoomTransition, 0.0);

    int clampZoomLevel() const;
    Q_OBJECT_COMPUTED_PROPERTY(InfiniteGridModel, int, m_clampedZoomLevel, &InfiniteGridModel::clampZoomLevel);
    Q_OBJECT_BINDABLE_PROPERTY(InfiniteGridModel, double, m_majorZoomGridInterval)
    Q_OBJECT_BINDABLE_PROPERTY(InfiniteGridModel, double, m_minorZoomGridInterval)

    // Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(InfiniteGridModel, QRectF, m_originMask, QRectF(), &InfiniteGridModel::originMaskChanged);

    Q_OBJECT_BINDABLE_PROPERTY(InfiniteGridModel, QRectF, m_originMask);

    QPropertyNotifier m_majorZoomGridIntervalNotifier;
    QPropertyNotifier m_minorZoomGridIntervalNotifier;
    QPropertyNotifier m_minorGridIntervalPixelNotifier;

    FixedGridModel* m_majorGrid;
    FixedGridModel* m_minorGrid;
};
}

