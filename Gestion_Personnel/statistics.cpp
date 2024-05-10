#include "statistics.h"
#include "ui_statistics.h"
#include "Gestion_Personnel/statisticss.h"
#include <qmath.h>
statistics::statistics(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::statistics)
{

    ui->setupUi(this);
    displayStatisticsInGraphicsView();
}

statistics::~statistics()
{
    delete ui;
}

void statistics::displayStatisticsInGraphicsView() {
    // Retrieve statistics
    QSqlQueryModel* model = employee::afficher_Employees();
    int rowCount = model->rowCount();

    // Create a scene
    QGraphicsScene* scene = new QGraphicsScene(this);

    // Set the radius and center of the circle
    qreal radius = 150.0;
    QPointF center(scene->width() / 2, scene->height() / 2);

    // Add statistic items in a circular layout
    qreal angleIncrement = 360.0 / rowCount; // Divide the circle into equal segments
    qreal currentAngle = 0;

    for (int i = 0; i < rowCount; ++i) {
        // Retrieve data
        QString name = model->data(model->index(i, 0)).toString(); // Assuming column 0 contains names
        qreal percentage = model->data(model->index(i, 1)).toDouble(); // Assuming column 1 contains percentages

        // Add statistic item
        scene->addItem(createStatisticItem(name, percentage, center, radius, currentAngle));

        // Increment angle
        currentAngle += angleIncrement;
    }

    // Add the scene to the graphics view
    ui->graphicsView->setScene(scene);
}


QGraphicsItem* statistics::createStatisticItem(const QString& name, qreal percentage, const QPointF& center, qreal radius, qreal angle) {
    // Convert angle from degrees to radians
    qreal radians = qDegreesToRadians(angle);

    // Calculate position of the item in the circle
    qreal x = center.x() + radius * qCos(radians);
    qreal y = center.y() + radius * qSin(radians); // Positive y because y-axis is not inverted in graphics scene

    // Create and return the statistic item
    return new StatisticItem(name, percentage, QPointF(x, y));
}

