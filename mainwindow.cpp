#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"artist.h"
#include <QtSql>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::on_pushButton_3_pressed);

            // Initialize the table widget
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "NOM" << "ADR" << "NUMTEL" <<"Delete"<<"UPDATE" );
    ui->lineEdit_id_hidden->setVisible(false);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    Artist A;
    A.setnom(ui->Name->text());
    A.setadr(ui->adr->text());
    QString Numteltext = ui->Num_Tel->text();
    int Numtel = Numteltext.toInt();
    A.setNumTel(Numtel);


    ui->adr->clear();
    ui->Num_Tel->clear();
    ui->Name->clear();

      // Inform the user that the information has been saved

    QSqlQuery query;

   query.prepare("INSERT INTO ARTIST (NOM, ADR, NUMTEL) "
                 "VALUES (:Nomartiste, :adresseartiste, :Numartiste)");

   query.bindValue(":Nomartiste", A.getnom());
   query.bindValue(":adresseartiste", A.getadr());
   query.bindValue(":Numartiste", A.getNumTel());


   if (query.exec()) {

   } else {
       QMessageBox::critical(this, "Error", "Failed to save event data: " + query.lastError().text());
   }
}

void MainWindow::on_pushButton_2_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_3_pressed()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT ID, NOM, ADR, NUMTEL FROM ARTIST");

    if (model->lastError().isValid()) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to retrieve data: ") + model->lastError().text());
        delete model;
        return;
    }

    // Set the model to the table widget
    ui->tableWidget->setRowCount(model->rowCount());
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
            ui->tableWidget->setItem(row, col, item);
        }

        // Add a "Delete" button to each row
        QPushButton *deleteButton = new QPushButton("Delete", this);
        QPushButton *updateButton = new QPushButton("Update", this);

        connect(deleteButton, &QPushButton::clicked, [this, row]() {
            // Retrieve the ID of the selected row
            QString id = ui->tableWidget->item(row, 0)->text(); // Assuming ID is in the first column

            // Perform the delete operation with the retrieved ID
            QSqlQuery query;
            query.prepare("DELETE FROM ARTIST WHERE ID = :id");
            query.bindValue(":id", id);

            if (query.exec()) {
                on_pushButton_3_pressed(); // Refresh the table after deletion
            } else {
                QMessageBox::critical(this, "Error", "Failed to delete user: " + query.lastError().text());
            }
        });

        connect(updateButton, &QPushButton::clicked, [this, row]() {
            // Retrieve the ID of the selected row
            QString id = ui->tableWidget->item(row, 0)->text(); // Assuming ID is in the first column

            // Retrieve user details from the selected row
            QString nom = ui->tableWidget->item(row, 1)->text(); // Assuming NOM is in the second column
            QString adr = ui->tableWidget->item(row, 2)->text(); // Assuming ADR is in the third column
            QString numtel = ui->tableWidget->item(row, 3)->text(); // Assuming NUMTEL is in the fourth column

            // Set the user details in the LineEdits for editing
            ui->lineEdit_id_hidden->setText(id);
            ui->Name->setText(nom);
            ui->adr->setText(adr);
            ui->Num_Tel->setText(numtel);
        });

        // Add the delete and update buttons to the table widget
        ui->tableWidget->setCellWidget(row, model->columnCount(), deleteButton); // Assuming the second last column is for delete button
        ui->tableWidget->setCellWidget(row, model->columnCount() + 1, updateButton); // Assuming the last column is for update button
    }

    delete model; // Release the memory
}

void MainWindow::on_pushButton_Update_clicked()
{

    int Artiste_Id = ui->lineEdit_id_hidden->text().toInt();
    QString name = ui->Name->text();
    QString adresse = ui->adr->text();
    int num_tel = ui->Num_Tel->text().toInt();
    // Update data in the database
    QSqlQuery query;
    query.prepare("UPDATE ARTIST SET NOM = :nom, ADR = :adr, NUMTEL = :num WHERE ID = :id");
    query.bindValue(":nom", name);
    query.bindValue(":adr", adresse);
    query.bindValue(":num", num_tel);
    query.bindValue(":id", Artiste_Id);
    if (query.exec()) {
        QMessageBox::information(this, tr("Success"), tr("Data updated successfully."));
        ui->lineEdit_id_hidden->clear();
        ui->Name->clear();
        ui->adr->clear();
        ui->Num_Tel->clear();
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Failed to update data: ") + query.lastError().text());
    }

}



void MainWindow::on_pushButton_triasc_clicked()
{
    // Fetch data from the database and sort it in ascending order based on ID
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT ID, NOM, ADR, NUMTEL FROM ARTIST ORDER BY ID ASC");

    if (model->lastError().isValid()) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to retrieve data: ") + model->lastError().text());
        delete model;
        return;
    }

    // Clear the existing content in the table widget
    ui->tableWidget->clearContents();

    // Set the sorted data to the table widget
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
            ui->tableWidget->setItem(row, col, item);
        }
    }

    delete model;
}

void MainWindow::on_pushButton_tridesc_clicked()
{
    // Fetch data from the database and sort it in descending order based on ID
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT ID, NOM, ADR, NUMTEL FROM ARTIST ORDER BY ID DESC");

    if (model->lastError().isValid()) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to retrieve data: ") + model->lastError().text());
        delete model;
        return;
    }

    // Clear the existing content in the table widget
    ui->tableWidget->clearContents();

    // Set the sorted data to the table widget
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
            ui->tableWidget->setItem(row, col, item);
        }
    }

    delete model;
}

