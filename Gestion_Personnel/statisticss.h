#ifndef STATISTICITEM_H
#define STATISTICITEM_H

#include <QGraphicsItem>
#include <QPainter>

class StatisticItem : public QGraphicsItem {
public:
    StatisticItem(const QString& name, qreal percentage, const QPointF& position, QGraphicsItem* parent = nullptr)
        : QGraphicsItem(parent), m_name(name), m_percentage(percentage), m_position(position) {}

    QRectF boundingRect() const override {
        return QRectF(0, 0, 100, 50); // Adjust the size as needed
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        // Draw the statistic
        painter->drawText(boundingRect(), Qt::AlignCenter, QString("%1: %2%").arg(m_name).arg(m_percentage));
    }

private:
    QString m_name;
    qreal m_percentage;
    QPointF m_position;
};

#endif // STATISTICITEM_H
