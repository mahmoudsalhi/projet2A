#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "dons.h"
#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // table dons
    ui->tableWidget_dons->setColumnCount(6);
    ui->tableWidget_dons->setHorizontalHeaderLabels(QStringList() << "ID" << "destination" << "montant"  << "date" << "Delete" << "Update");
    int rowHeight = 70; // Adjust this value as needed
    int rowWidth = 100; // Adjust this value as needed
    ui->tableWidget_dons->verticalHeader()->setDefaultSectionSize(rowHeight);
    ui->tableWidget_dons->horizontalHeader()->setDefaultSectionSize(rowWidth);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_reset_dons_clicked()
{
    ui->lineEdit_id_dons->clear();
    ui->lineEdit_destination->clear();
    ui->lineEdit_montant->clear();
    ui->dateEdit_date_dons->clear();
}

void MainWindow::on_pushButton_ajouter_dons_clicked()
{
    dons D;
    D.setdestination(ui->lineEdit_destination->text());
    QString montantText = ui->lineEdit_montant->text();
    double price = montantText.toDouble();
    D.setmontant(price);

    D.setdate(ui->dateEdit_date_dons->date());

    // Insert data into the database
    QSqlQuery query;
    query.prepare("INSERT INTO DON (DESTINATION, MONTANT, DATE_DON)"
                  "VALUES (:dest, :montant, :dat)");
    query.bindValue(":dest", D.getdestination());
    query.bindValue(":montant", D.getmontant());
    query.bindValue(":dat", D.getdate());

    if (query.exec()) {
        QMessageBox::information(this, tr("Success"), tr("Data saved successfully."));
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Failed to save data: ") + query.lastError().text());
    }
    ui->lineEdit_id_dons->clear();
    ui->lineEdit_destination->clear();
    ui->lineEdit_montant->clear();
    ui->dateEdit_date_dons->clear();
}

void MainWindow::on_afficher_dons_pushbutton_clicked(){
    // Create a QSqlQueryModel and set the query
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT ID, DESTINATION, MONTANT, DATE_DON FROM DONS");
    if (model->lastError().isValid()) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to retrieve data: ") + model->lastError().text());
        delete model;
        return;
    }
    // Set the model to the table widget
    ui->tableWidget_dons->setRowCount(model->rowCount());
    ui->tableWidget_dons->setColumnCount(model->columnCount() + 2); // Add two columns for the delete and update buttons
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setFlags(item->flags() & ~Qt::ItemIsEditable); // Make cells read-only
            item->setData(Qt::DisplayRole, model->data(model->index(row, col)));
            item->setTextAlignment(Qt::AlignCenter); // Center the text
            ui->tableWidget_dons->setItem(row, col, item);
        }
        // Add delete button to the second last column
        QPushButton *deleteButton = new QPushButton("Delete");
        deleteButton->setStyleSheet("QPushButton { font: 700 16pt 'Sitka Subheading'; background-color: red; }");
        connect(deleteButton, &QPushButton::clicked, this, [this, row]() {
            onDeleteButtonClicked_dons(row);
        });
        ui->tableWidget_dons->setCellWidget(row, model->columnCount(), deleteButton);

        // Add update button to the last column
        QPushButton *updateButton = new QPushButton("Update");
        updateButton->setStyleSheet("QPushButton { font: 700 16pt 'Sitka Subheading'; background-color: green; }");
        connect(updateButton, &QPushButton::clicked, this, [this, row]() {
            onRowUpdateClicked_dons(row); // Call the function to handle row update
        });
        ui->tableWidget_dons->setCellWidget(row, model->columnCount() + 1, updateButton);
    }
    delete model;
}

void MainWindow::onDeleteButtonClicked_dons(int row)
{
    QSqlTableModel *model = static_cast<QSqlTableModel*>(ui->tableWidget_dons->model());
    QModelIndex index = model->index(row, 0);
    int id = model->data(index).toInt();
    // Perform deletion from the database
    QSqlQuery query;
    query.prepare("DELETE FROM DONS WHERE ID = :id");
    query.bindValue(":id", id);
    if (query.exec()) {
        // Update the table widget after successful deletion
        ui->tableWidget_dons->removeRow(row);
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Failed to delete row: ") + query.lastError().text());
    }
}

void MainWindow::onRowUpdateClicked_dons(int row) {
    int Id = ui->tableWidget_dons->item(row, 0)->text().toInt();
    // Get other data from the selected row
    QString destination = ui->tableWidget_dons->item(row, 1)->text();
    QString montant = ui->tableWidget_dons->item(row, 2)->text();
    QDate date = QDate::fromString(ui->tableWidget_dons->item(row, 3)->text(), "yyyy-MM-dd");
    // Populate the form fields with the data
    ui->lineEdit_id_dons->setText(QString::number(Id));
    ui->lineEdit_destination->setText(destination);
    ui->lineEdit_montant->setText(montant);
    ui->dateEdit_date_dons->setDate(date);
    selectedRowForUpdate_dons = Id;
}

void MainWindow::on_pushButton_update_dons_clicked()
{
    int productId = ui->lineEdit_id_dons->text().toInt();
    QString dest = ui->lineEdit_destination->text();
    double mont = ui->lineEdit_montant->text().toDouble();
    QDate date_don = ui->dateEdit_date_dons->date();

    QSqlQuery query;
    query.prepare("UPDATE DONS SET DESTINATION = :dest, MONTANT = :mont, DATE_DON = :dat WHERE ID = :id");
    query.bindValue(":dest", dest);
    query.bindValue(":mont", mont);
    query.bindValue(":dat", date_don);

    query.bindValue(":id", productId);
    if (query.exec()) {
        QMessageBox::information(this, tr("Success"), tr("Data updated successfully."));
        ui->lineEdit_id_dons->clear();
        ui->lineEdit_destination->clear();
        ui->lineEdit_montant->clear();
        ui->dateEdit_date_dons->clear();
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Failed to update data: ") + query.lastError().text());
    }
}
