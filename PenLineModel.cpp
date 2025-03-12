//Qt includes
#include <QDebug>
#include <QLineF>

//Our includes
#include "PenLineModel.h"

PenLineModel::PenLineModel(QObject* parent)
    : QAbstractItemModel(parent)
{
}

int PenLineModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return m_lines.at(parent.row()).points.size();
    }
    return m_lines.size();
}

QVariant PenLineModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    qintptr id = static_cast<qintptr>(index.internalId());
    if(id == -1) {
        //Line object level
        if(index.row() >= 0 && index.row() < m_lines.size()) {
            const PenLine& line = m_lines.at(index.row());
            switch(role) {
            case LineRole:
                return QVariant::fromValue(line);
            case LineWidthRole:
                return line.width;
            }
        } else {
            return QVariant();
        }
    } else {
        //Line position data level
        const PenLine& line = m_lines.at(id);
        if(role == PenPointRole) {
            return QVariant::fromValue(line.points.at(index.row()));
        }
    }

    return QVariant();
}

QHash<int, QByteArray> PenLineModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[LineRole] = "line";
    roles[LineWidthRole] = "lineWidth";
    return roles;
}

QModelIndex PenLineModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(parent == QModelIndex()) {
        return createIndex(row, 0, -1);
    } else {
        auto lineIndex = parent.row();
        if(lineIndex >= 0 && lineIndex < m_lines.size()) {
            return createIndex(row, 0, lineIndex);
        }
    }
    return QModelIndex(); //invalid index
}

QModelIndex PenLineModel::parent(const QModelIndex &child) const
{
    qintptr id = static_cast<qintptr>(child.internalId());
    if(id == -1) {
        return QModelIndex();
    } else if(id >= 0 && id < m_lines.size()) {
        return createIndex(id, 0, -1);
    } else {
        return QModelIndex();
    }
}

int PenLineModel::addNewLine()
{
    int lastIndex = m_lines.size();
    PenLine line;
    line.width = m_currentStrokeWidth;
    qDebug() << "newline:" << line.width;

    addLine(line);
    Q_ASSERT(lastIndex == m_lines.size() - 1);
    return lastIndex;
}

void PenLineModel::addLine(const PenLine &line) {
    // qDebug() << "-------------- Add line -------------";
    beginInsertRows(QModelIndex(), m_lines.size(), m_lines.size());
    m_lines.append(line);
    endInsertRows();
}

void PenLineModel::addPoint(int lineIndex, PenPoint point)
{
    if (lineIndex < 0 || lineIndex >= m_lines.size()) {
        qWarning() << "addPoint: lineIndex out of bounds:" << lineIndex;
        return;
    }

    // if(point.pressure < 0.1) {
    //     qDebug() << "Rejected for too small width:" << point.pressure;
    //     return;
    // }


    // Append the point to the specified PenLine.
    auto& points =  m_lines[lineIndex].points;
    auto appendPoint = [&](PenPoint point) {
        int lastIndex = points.size();

        // Notify that the data for this row has changed so that any views update.
        QModelIndex index = this->index(lineIndex);

        beginInsertRows(index, lastIndex, lastIndex) ;
        points.append(point);
        endInsertRows();
    };

    if(!points.isEmpty()) {
        QLineF distanceLine(points.last().position, point.position);
        if(distanceLine.length() <= 0.2) {
            return;
        } else {
            // qDebug() << "Point:" << point.position << point.width;
            appendPoint(point);
        }
    } else {
        // qDebug() << "Point:" << point.position << point.width;
        appendPoint(point);
    }

}

void PenLineModel::clear() {
    beginResetModel();
    m_lines.clear();
    endResetModel();
}



