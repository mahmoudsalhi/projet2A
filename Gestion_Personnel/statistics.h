#ifndef STATISTICS_H
#define STATISTICS_H

#include <QDialog>
#include <QSqlQueryModel>
#include "employee.h"
#include <QGraphicsItem>



namespace Ui {
class statistics;
}

class statistics : public QDialog
{
    Q_OBJECT

public:
    explicit statistics(QWidget *parent = nullptr);
    ~statistics();
    void displayStatisticsInGraphicsView();
    QGraphicsItem* createStatisticItem(const QString& name, qreal percentage, const QPointF& center, qreal radius, qreal angle);

private:
    Ui::statistics *ui;
};

#endif // STATISTICS_H
