#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include "evenement.h"
#include <QMessageBox>
#include <QtCharts>
#include <QFileDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QTextCursor>
#include <QCalendarWidget>
#include "clipboardhelper.h"
#include <QQmlContext>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ClipboardHelper *clipboardHelper = new ClipboardHelper(this);

        // Expose the ClipboardHelper instance to QML
        ui->quickWidget->rootContext()->setContextProperty("ClipboardHelper", clipboardHelper);

     ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/map.qml")));

    connect(ui->pushButton_refresh, &QPushButton::clicked, this, &MainWindow::on_pushButton_refresh_pressed);
    ui->tableWidget->setColumnCount(10);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "NOM" << "DESCRIPTION" << "LIEU" << "DATE_DEBUT "<< "DATE_FIN" << "PRIXENTRE"<< "TYPE"<<"DELETE"<<"UPDATE");

        int rowHeight = 50;
        int rowWidth = 120;
        ui->tableWidget->verticalHeader()->setDefaultSectionSize(rowHeight);
        ui->tableWidget->horizontalHeader()->setDefaultSectionSize(rowWidth);



}

MainWindow::~MainWindow()
{
    delete ui;

}


void MainWindow::on_pushButton_clicked()
{
    QString text1 = ui->lineEdit_nom->text();
    QString text2 = ui->lineEdit_description->text();

    QString text4 = ui->lineEdit_prixentre->text();

    QRegularExpression caract("^[a-zA-Z]+$");
    QRegularExpression chiffre("^[0-9]+$");


    ui->label_nom_error->clear();
    ui->label_description_error->clear();

    ui->label_error_prix->clear();

    if (text1.isEmpty() || !text1.contains(caract)) {
        ui->label_nom_error->setText("<font color='red'>Le nom n'est pas valide.</font>");
    }else{
        ui->label_nom_error->setText("<font color='green'>correct.</font>");
    }

    if (text2.isEmpty() || !text2.contains(caract)) {
        ui->label_description_error->setText("<font color='red'>La description n'est pas valide.</font>");
    }else{
        ui->label_description_error->setText("<font color='green'>correct.</font>");
    }



    if (text4.isEmpty() || !text4.contains(chiffre)) {
        ui->label_error_prix->setText("<font color='red'>Le prix d'entrée n'est pas valide.</font>");
    }else{
        ui->label_error_prix->setText("<font color='green'>correct.</font>");
    }

    if (text1.isEmpty() || text2.isEmpty() || text4.isEmpty() ||!text1.contains(caract) || !text2.contains(caract) || !text4.contains(chiffre)) {
            return;
        }

    evenement e;

    e.set_nom(ui->lineEdit_nom->text());
    e.set_description(ui->lineEdit_description->text());
    e.set_lieu(ui->lineEdit_lieu->text());


       e.set_datedebut(ui->datedebutEdit->date());
       e.set_datefin(ui->datefinEdit->date());

       QString prixText = ui->lineEdit_prixentre->text();
       double prix = prixText.toDouble();
       e.set_prixentre(prix);

       QString selectedType = ui->comboBox_type->currentText();
       e.set_type(selectedType);

       ui->lineEdit_nom->clear();
           ui->lineEdit_description->clear();
           ui->lineEdit_lieu->clear();

           ui->lineEdit_prixentre->clear();


       QSqlQuery query;

           query.prepare("INSERT INTO EVENEMENT (nom, description, lieu, DATE_DEBUT, DATE_FIN, PRIXENTRE, TYPE) "
                         "VALUES (:eventName, :eventDescription, :eventPlace, :eventStartDate, :eventEndDate, :eventEntryPrice, :eventType)");

           query.bindValue(":eventName", e.get_nom());
           query.bindValue(":eventDescription", e.get_description());
           query.bindValue(":eventPlace", e.get_lieu());

           query.bindValue(":eventStartDate", e.get_datedebut());
           query.bindValue(":eventEndDate", e.get_datefin());
           query.bindValue(":eventEntryPrice", e.get_prixentre());
           query.bindValue(":eventType", e.get_type());

           if (query.exec()) {
               QMessageBox::information(this, "Success", "Event data saved successfully");
               // Mark the date_debut on the calendar widget
                       QDate debutDate = e.get_datedebut();
                       QCalendarWidget* calendarWidget = ui->calendarWidgetx; // Replace with your actual calendar widget
                       QTextCharFormat format = calendarWidget->dateTextFormat(debutDate);
                       format.setForeground(Qt::cyan);
                       calendarWidget->setDateTextFormat(debutDate, format);
           } else {
               QMessageBox::critical(this, "Error", "Failed to save event data: " + query.lastError().text());
           }
}


void MainWindow::on_pushButton_refresh_pressed()
{
    ui->lineEdit_search->clear();
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT ID, NOM, DESCRIPTION, LIEU, TO_CHAR(DATE_DEBUT, 'MM-DD-YYYY'), TO_CHAR(DATE_FIN, 'MM-DD-YYYY'), PRIXENTRE, TYPE FROM EVENEMENT");

    if (model->lastError().isValid()) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to retrieve data: ") + model->lastError().text());
        delete model;
        return;
    }

    ui->tableWidget->setRowCount(model->rowCount());
    ui->tableWidget->setColumnCount(model->columnCount()+2);

    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            item->setData(Qt::DisplayRole, model->data(model->index(row, col)));
            ui->tableWidget->setItem(row, col,item);
        }

        QPushButton *deleteButton = new QPushButton("Delete");
        deleteButton->setStyleSheet("QPushButton { font: 700 16pt 'Sitka Subheading'; background-color: red; }");
        connect(deleteButton, &QPushButton::clicked, this, [this, row]() {
            onDeleteButtonClicked(row);
        });
        ui->tableWidget->setCellWidget(row, model->columnCount(), deleteButton);


        QPushButton *updateButton = new QPushButton("Update");
        updateButton->setStyleSheet("QPushButton { font: 700 16pt 'Sitka Subheading'; background-color: green; }");
        connect(updateButton, &QPushButton::clicked, this, [this, row]() {
            onRowUpdateClicked(row);
        });
        ui->tableWidget->setCellWidget(row, model->columnCount() +1, updateButton);
    }

    // Clear the marked dates on the calendar widget
    QCalendarWidget* calendarWidget = ui->calendarWidgetx; // Replace with your actual calendar widget
    calendarWidget->setDateTextFormat(QDate(), QTextCharFormat()); // Clear all existing formats

    // Mark the dates from the DATE_DEBUT column
    for (int row = 0; row < model->rowCount(); ++row) {
        QString dateDebutString = model->data(model->index(row, 4)).toString();
        QDate dateDebut = QDate::fromString(dateDebutString, "MM-dd-yyyy");
        if (dateDebut.isValid()) {
            for (int column = 0; column < model->columnCount(); ++column) {
                QModelIndex index = model->index(row, column);
                QString dateString = model->data(index).toString();
                QDate date = QDate::fromString(dateString, "MM-dd-yyyy");
                if (date.isValid()) {
                    QTextCharFormat format = calendarWidget->dateTextFormat(date);
                    format.setBackground(Qt::red);
                    calendarWidget->setDateTextFormat(date, format);
                }
            }
        }
    }


    delete model;
}
QDate MainWindow::getDateDebutForId(int id)
{
    QSqlQuery query;
    query.prepare("SELECT DATE_DEBUT FROM EVENEMENT WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        // Retrieve the date_debut from the query result
        QVariant dateVariant = query.value(0);
        if (dateVariant.isValid() && dateVariant.canConvert<QDate>()) {
            return dateVariant.toDate();
        }
    }

    // Return an invalid date if the query fails or the date_debut is not found
    return QDate();
}

void MainWindow::onDeleteButtonClicked(int row)
{
    QSqlTableModel *model = static_cast<QSqlTableModel*>(ui->tableWidget->model());
    QModelIndex index = model->index(row, 0); // Assuming the ID_PRODUIT is in the first column
    int id = model->data(index).toInt();


    QSqlQuery query;
    query.prepare("DELETE FROM EVENEMENT WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        // Clear the marked date from the calendar widget
                QDate debutDate = getDateDebutForId(id);
                QCalendarWidget* calendarWidget = ui->calendarWidgetx; // Replace with your actual calendar widget
                QTextCharFormat format = calendarWidget->dateTextFormat(debutDate);
                format.clearForeground();
                calendarWidget->setDateTextFormat(debutDate, format);
        // Update the table widget after successful deletion
        ui->tableWidget->removeRow(row);
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Failed to delete row: ") + query.lastError().text());
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->lineEdit_nom->clear();
        ui->lineEdit_description->clear();
        ui->lineEdit_lieu->clear();
        ui->datedebutEdit->clear();
        ui->datefinEdit->clear();
        ui->lineEdit_prixentre->clear();
}

void MainWindow::onRowUpdateClicked(int row) {

    int ID = ui->tableWidget->item(row, 0)->text().toInt();
    QString nom = ui->tableWidget->item(row, 1)->text();
    QString description = ui->tableWidget->item(row, 2)->text();
    QString lieu = ui->tableWidget->item(row, 3)->text();
    QDate datedeb = QDate::fromString(ui->tableWidget->item(row, 4)->text(), "yyyy-MM-dd");
    QDate datefin = QDate::fromString(ui->tableWidget->item(row, 5)->text(), "yyyy-MM-dd");
    QString prixentre = ui->tableWidget->item(row, 6)->text();
    QString type = ui->tableWidget->item(row, 7)->text();


    ui->lineEdit_ID->setText(QString::number(ID));
    ui->lineEdit_nom->setText(nom);
    ui->lineEdit_description->setText(description);
    ui->lineEdit_lieu->setText(lieu);
    ui->datedebutEdit->setDate(datedeb);
    ui->datefinEdit->setDate(datefin);


    ui->lineEdit_prixentre->setText(prixentre);


    int index = ui->comboBox_type->findText(type);
    if (index != -1) {
        ui->comboBox_type->setCurrentIndex(index);
    }


    selectedRowForUpdate = ID;
}


void MainWindow::on_pushButton_6_clicked()
{
    int id = ui->lineEdit_ID->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    QString description = ui->lineEdit_description->text();
    QString lieu = ui->lineEdit_lieu->text();

    QDate datedeb = ui->datedebutEdit->date();
    QDate datefin = ui->datefinEdit->date();
    double prixentre = ui->lineEdit_prixentre->text().toDouble();
    QString type = ui->comboBox_type->currentText();
    // Update data in the database

    QSqlQuery query;
    query.prepare("UPDATE EVENEMENT SET NOM = :nom, DESCRIPTION = :description, LIEU = :lieu, DATE_DEBUT = :datedeb,DATE_FIN = :datefin,PRIXENTRE = :prixentre,TYPE = :type WHERE ID = :id");
    query.bindValue(":nom", nom);
    query.bindValue(":description", description);
    query.bindValue(":lieu", lieu);

    query.bindValue(":datedeb", datedeb);
    query.bindValue(":datefin", datefin);
    query.bindValue(":prixentre", prixentre);
    query.bindValue(":type", type);
    query.bindValue(":id", id);
    if (query.exec()) {
        QMessageBox::information(this, tr("Success"), tr("Data updated successfully."));
        ui->lineEdit_ID->clear();
        ui->lineEdit_nom->clear();
        ui->lineEdit_description->clear();
        ui->lineEdit_lieu->clear();

        ui->lineEdit_prixentre->clear();
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Failed to update data: ") + query.lastError().text());
    }
}

void MainWindow::on_pushButton_5_clicked()
{
        int sortingIndex = ui->comboBox->currentIndex();
        QString sortingOrder;
        switch (sortingIndex) {
        case 0:
            sortingOrder = "ID ASC";
            break;
        case 1:
            sortingOrder = "ID DESC";
            break;
        case 2:
            sortingOrder = "NOM ASC";
            break;
        case 3:
            sortingOrder = "NOM DESC";
            break;
        case 4:
            sortingOrder = "DESCRIPTION ASC";
            break;
        case 5:
            sortingOrder = "DESCRIPTION DESC";
            break;
        case 6:
            sortingOrder = "LIEU ASC";
            break;
        case 7:
            sortingOrder = "LIEU DESC";
            break;
        case 8:
            sortingOrder = "DATE_DEBUT ASC";
            break;
        case 9:
            sortingOrder = "DATE_DEBUT DESC";
            break;
        case 10:
            sortingOrder = "DATE_FIN ASC";
            break;
        case 11:
            sortingOrder = "DATE_FIN DESC";
            break;
        case 12:
            sortingOrder = "PRIXENTRE ASC";
            break;
        case 13:
            sortingOrder = "PRIXENTRE DESC";
            break;
        case 14:
            sortingOrder = "TYPE ASC";
            break;
        case 15:
            sortingOrder = "TYPE DESC";
            break;
        default:
            sortingOrder = "ID ASC";
            break;
        }

        QString queryString = QString("SELECT ID, NOM, DESCRIPTION, LIEU, TO_CHAR(DATE_DEBUT, 'MM-DD-YYYY') ,TO_CHAR(DATE_FIN, 'MM-DD-YYYY'), PRIXENTRE, TYPE FROM EVENEMENT ORDER BY %1").arg(sortingOrder);

        QSqlQueryModel *model = new QSqlQueryModel(this);
        model->setQuery(queryString);

        if (model->lastError().isValid()) {
            QMessageBox::critical(this, tr("Error"), tr("Failed to retrieve data: ") + model->lastError().text());
            delete model;
            return;
        }

        ui->tableWidget->setRowCount(model->rowCount());
        ui->tableWidget->setColumnCount(model->columnCount() + 2);

        for (int row = 0; row < model->rowCount(); ++row) {
            for (int col = 0; col < model->columnCount(); ++col) {
                QTableWidgetItem *item = new QTableWidgetItem;
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                item->setData(Qt::DisplayRole, model->data(model->index(row, col)));
                ui->tableWidget->setItem(row, col, item);
            }

            QPushButton *deleteButton = new QPushButton("Delete");
            deleteButton->setStyleSheet("QPushButton { font: 700 16pt 'Sitka Subheading'; background-color: red; }");
            connect(deleteButton, &QPushButton::clicked, this, [this, row]() {
                onDeleteButtonClicked(row);
            });
            ui->tableWidget->setCellWidget(row, model->columnCount(), deleteButton);

            QPushButton *updateButton = new QPushButton("Update");
            updateButton->setStyleSheet("QPushButton { font: 700 16pt 'Sitka Subheading'; background-color: green; }");
            connect(updateButton, &QPushButton::clicked, this, [this, row]() {
                onRowUpdateClicked(row); // Call the function to handle row update
            });
            ui->tableWidget->setCellWidget(row, model->columnCount() + 1, updateButton);
        }

        delete model;
}

void MainWindow::on_pushButton_7_clicked()
{

        QString keyword = ui->lineEdit_search->text().trimmed();

        QString queryString = QString("SELECT ID, NOM, DESCRIPTION, LIEU, TO_CHAR(DATE_DEBUT, 'MM-DD-YYYY') ,TO_CHAR(DATE_FIN, 'MM-DD-YYYY'), PRIXENTRE, TYPE FROM EVENEMENT WHERE ");


        queryString += "NOM LIKE '%" + keyword + "%' OR ";
        queryString += "DESCRIPTION LIKE '%" + keyword + "%' OR ";
        queryString += "LIEU LIKE '%" + keyword + "%' OR ";
        queryString += "PRIXENTRE LIKE '%" + keyword + "%' OR ";
        queryString += "TYPE LIKE '%" + keyword + "%'";

        QSqlQueryModel *model = new QSqlQueryModel(this);
        model->setQuery(queryString);

        if (model->lastError().isValid()) {
            QMessageBox::critical(this, tr("Error"), tr("Failed to retrieve data: ") + model->lastError().text());
            delete model;
            return;
        }

        ui->tableWidget->setRowCount(model->rowCount());
        ui->tableWidget->setColumnCount(model->columnCount() + 2);

        for (int row = 0; row < model->rowCount(); ++row) {
            for (int col = 0; col < model->columnCount(); ++col) {
                QTableWidgetItem *item = new QTableWidgetItem;
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                item->setData(Qt::DisplayRole, model->data(model->index(row, col)));
                ui->tableWidget->setItem(row, col, item);
            }

            QPushButton *deleteButton = new QPushButton("Delete");
            deleteButton->setStyleSheet("QPushButton { font: 700 16pt 'Sitka Subheading'; background-color: red; }");
            connect(deleteButton, &QPushButton::clicked, this, [this, row]() {
                onDeleteButtonClicked(row);
            });
            ui->tableWidget->setCellWidget(row, model->columnCount(), deleteButton);

            QPushButton *updateButton = new QPushButton("Update");
            updateButton->setStyleSheet("QPushButton { font: 700 16pt 'Sitka Subheading'; background-color: green; }");
            connect(updateButton, &QPushButton::clicked, this, [this, row]() {
                onRowUpdateClicked(row);
            });
            ui->tableWidget->setCellWidget(row, model->columnCount() + 1, updateButton);
        }

        delete model;
}

void MainWindow::on_pushButton_3_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF"), "", tr("PDF Files (*.pdf)"));
            if (!fileName.isEmpty()) {
                QPrinter printer(QPrinter::PrinterResolution);
                printer.setOutputFormat(QPrinter::PdfFormat);
                printer.setPaperSize(QPrinter::A4);
                printer.setOutputFileName(fileName);

                QTextDocument doc;
                QTextCursor cursor(&doc);



                // Add table data
                for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
                    QStringList rowData;
                    for (int col = 0; col < ui->tableWidget->columnCount() - 2; ++col) {
                        QTableWidgetItem* item = ui->tableWidget->item(row, col);
                        rowData << (item ? item->text() : "");
                    }
                    cursor.insertText(rowData.join("\t") + "\n");
                }

                doc.print(&printer);
                QMessageBox::information(this, "Succes", "PDF genere avec succes.");
            }
}

void MainWindow::on_pushButton_4_clicked()
{
    QDate selectedDate = ui->dateEdit->date();

        QSqlQueryModel *model = new QSqlQueryModel(this);
        model->setQuery(QString("SELECT ID, NOM, DESCRIPTION, LIEU, TO_CHAR(DATE_DEBUT, 'MM-DD-YYYY') ,TO_CHAR(DATE_FIN, 'MM-DD-YYYY'), PRIXENTRE, TYPE FROM EVENEMENT WHERE DATE_DEBUT = TO_DATE('%1', 'YYYY-MM-DD')")
                        .arg(selectedDate.toString("yyyy-MM-dd")));

        if (model->lastError().isValid()) {
            QMessageBox::critical(this, tr("Error"), tr("Failed to retrieve data: ") + model->lastError().text());
            delete model;
            return;
        }

        ui->tableWidget->setRowCount(model->rowCount());
        ui->tableWidget->setColumnCount(model->columnCount() + 2);

        for (int row = 0; row < model->rowCount(); ++row) {
            for (int col = 0; col < model->columnCount(); ++col) {
                QTableWidgetItem *item = new QTableWidgetItem;
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                item->setData(Qt::DisplayRole, model->data(model->index(row, col)));
                ui->tableWidget->setItem(row, col, item);
            }

            QPushButton *deleteButton = new QPushButton("Delete");
            deleteButton->setStyleSheet("QPushButton { font: 700 16pt 'Sitka Subheading'; background-color: red; }");
            connect(deleteButton, &QPushButton::clicked, this, [this, row]() {
                onDeleteButtonClicked(row);
            });
            ui->tableWidget->setCellWidget(row, model->columnCount(), deleteButton);

            QPushButton *updateButton = new QPushButton("Update");
            updateButton->setStyleSheet("QPushButton { font: 700 16pt 'Sitka Subheading'; background-color: green; }");
            connect(updateButton, &QPushButton::clicked, this, [this, row]() {
                onRowUpdateClicked(row);
            });
            ui->tableWidget->setCellWidget(row, model->columnCount() + 1, updateButton);
        }

        delete model;
}



void MainWindow::on_pushButton_8_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    QBarSeries *barSeries = new QBarSeries();

    // Create a pie series to hold the data for type evenement
    QPieSeries *pieSeries = new QPieSeries();

    // Query the database to retrieve event statistics by lieu
    QSqlQuery lieuQuery;
    lieuQuery.prepare("SELECT lieu, COUNT(*) AS event_count FROM EVENEMENT GROUP BY lieu");

    if (lieuQuery.exec()) {
        // Populate the bar series with data
        while (lieuQuery.next()) {
            QString lieu = lieuQuery.value(0).toString();
            int eventCount = lieuQuery.value(1).toInt();

            // Add data to the bar series
            QBarSet *barSet = new QBarSet(lieu);
            *barSet << eventCount;
            barSeries->append(barSet);
        }
    } else {
        QMessageBox::critical(this, "Error", "Failed to retrieve event statistics by lieu: " + lieuQuery.lastError().text());
        return;
    }

    // Query the database to retrieve event statistics by type evenement
    QSqlQuery typeQuery;
    typeQuery.prepare("SELECT type, COUNT(*) AS event_count FROM EVENEMENT GROUP BY type");

    if (typeQuery.exec()) {
        // Populate the pie series with data
        while (typeQuery.next()) {
            QString type = typeQuery.value(0).toString();
            int eventCount = typeQuery.value(1).toInt();

            // Add data to the pie series
            pieSeries->append(type, eventCount);
        }
    } else {
        QMessageBox::critical(this, "Error", "Failed to retrieve event statistics by type evenement: " + typeQuery.lastError().text());
        return;
    }

    // Create a bar chart and add the bar series to it
    QChart *barChart = new QChart();
    barChart->addSeries(barSeries);
    barChart->setTitle("Statistiques des evenements par lieu :");

    // Create a pie chart and add the pie series to it
    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Statistiques des evenements par type :");

    // Create chart views for both charts
    QChartView *barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);

    QChartView *pieChartView = new QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);

    // Add the chart views to your UI layout accordingly
    // For example, if you have a layout:
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(barChartView);
    layout->addWidget(pieChartView);

    // Set the layout to your central widget or another container widget
    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

void MainWindow::on_pushButton_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}



void MainWindow::on_calendarWidgetx_clicked(const QDate &date)
{
    ui->tableWidget->clearContents();

       // Fetch events for the selected date
       QSqlQueryModel *model = new QSqlQueryModel(this);
       model->setQuery("SELECT ID, NOM, DESCRIPTION, LIEU, TO_CHAR(DATE_DEBUT, 'MM-DD-YYYY'), TO_CHAR(DATE_FIN, 'MM-DD-YYYY'), PRIXENTRE, TYPE FROM EVENEMENT WHERE TO_CHAR(DATE_DEBUT, 'MM-DD-YYYY') = '" + date.toString("MM-dd-yyyy") + "'");

       if (model->lastError().isValid()) {
           QMessageBox::critical(this, tr("Error"), tr("Failed to retrieve data: ") + model->lastError().text());
           delete model;
           return;
       }

       // Display events in the table widget
       ui->tableWidget->setRowCount(model->rowCount());
       ui->tableWidget->setColumnCount(model->columnCount() + 2);

       // Populate table with event data
       for (int row = 0; row < model->rowCount(); ++row) {
           for (int col = 0; col < model->columnCount(); ++col) {
               QTableWidgetItem *item = new QTableWidgetItem;
               item->setFlags(item->flags() & ~Qt::ItemIsEditable);
               item->setData(Qt::DisplayRole, model->data(model->index(row, col)));
               ui->tableWidget->setItem(row, col, item);
           }

           QPushButton *deleteButton = new QPushButton("Delete");
           deleteButton->setStyleSheet("QPushButton { font: 700 16pt 'Sitka Subheading'; background-color: red; }");
           connect(deleteButton, &QPushButton::clicked, this, [this, row]() {
               onDeleteButtonClicked(row);
           });
           ui->tableWidget->setCellWidget(row, model->columnCount(), deleteButton);

           QPushButton *updateButton = new QPushButton("Update");
           updateButton->setStyleSheet("QPushButton { font: 700 16pt 'Sitka Subheading'; background-color: green; }");
           connect(updateButton, &QPushButton::clicked, this, [this, row]() {
               onRowUpdateClicked(row);
           });
           ui->tableWidget->setCellWidget(row, model->columnCount() + 1, updateButton);
       }

       delete model;
}
