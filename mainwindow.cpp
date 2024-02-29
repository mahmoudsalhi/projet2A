#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include "evenement.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    QString text1 =ui->lineEdit_nom->text();
    QString text2 =ui->lineEdit_description->text();
    QString text3 =ui->lineEdit_lieu->text();
    QString text4 =ui->lineEdit_prixentre->text();

    QRegularExpression caract("^[a-zA-Z]+$");
    QRegularExpression chiffre("^[0-9]+$");

    if (!text1.isEmpty() && !text2.isEmpty() && !text3.isEmpty() && !text4.isEmpty() && text1.contains(caract)&& text2.contains(caract) && text3.contains(caract) && text4.contains(chiffre)) {
        qDebug() << "Le texte saisi est valide : " ;
    } else {
        QMessageBox::critical(this, "Erreur", "Le texte saisi n'est pas valide !");
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
           } else {
               QMessageBox::critical(this, "Error", "Failed to save event data: " + query.lastError().text());
           }
}


void MainWindow::on_pushButton_refresh_pressed()
{
    ui->lineEdit_search->clear();
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT ID, NOM, DESCRIPTION, LIEU,TO_CHAR(DATE_DEBUT, 'MM-DD-YYYY') ,TO_CHAR(DATE_FIN, 'MM-DD-YYYY'),PRIXENTRE,TYPE FROM EVENEMENT");

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

    delete model;
}


void MainWindow::onDeleteButtonClicked(int row)
{
    QSqlTableModel *model = static_cast<QSqlTableModel*>(ui->tableWidget->model());
    QModelIndex index = model->index(row, 0); // Assuming the ID_PRODUIT is in the first column
    int id = model->data(index).toInt();

    // Perform deletion from the database
    QSqlQuery query;
    query.prepare("DELETE FROM EVENEMENT WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
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
    // Get the data from the selected row
    int ID = ui->tableWidget->item(row, 0)->text().toInt();
    QString nom = ui->tableWidget->item(row, 1)->text();
    QString description = ui->tableWidget->item(row, 2)->text();
    QString lieu = ui->tableWidget->item(row, 3)->text();
    QDate datedeb = QDate::fromString(ui->tableWidget->item(row, 4)->text(), "yyyy-MM-dd");
    QDate datefin = QDate::fromString(ui->tableWidget->item(row, 5)->text(), "yyyy-MM-dd"); // Assuming your date format is yyyy-MM-dd
    QString prixentre = ui->tableWidget->item(row, 6)->text();
    QString type = ui->tableWidget->item(row, 7)->text();

    // Populate the form fields with the data
    ui->lineEdit_ID->setText(QString::number(ID)); // convert ID to string
    ui->lineEdit_nom->setText(nom);
    ui->lineEdit_description->setText(description);
    ui->lineEdit_lieu->setText(lieu);
    ui->datedebutEdit->setDate(datedeb);
    ui->datefinEdit->setDate(datefin); // Assuming you have a QDateEdit named datefinEdit

    // Set prixentre in your combobox or line edit
    // For example, if prixentre is supposed to be set in a line edit, you can do:
    ui->lineEdit_prixentre->setText(prixentre);

    // Set type in your combobox or line edit
    // For example, if type is supposed to be set in a line edit, you can do:
    int index = ui->comboBox_type->findText(type);
    if (index != -1) { // If the 'type' exists in the combobox
        ui->comboBox_type->setCurrentIndex(index);
    }

    // Store the selected row for update
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


