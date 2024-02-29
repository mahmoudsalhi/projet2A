#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Produit.h"
#include "dialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QtSql>

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);
    connect(ui->pushButton_reload, &QPushButton::clicked, this, &MainWindow::on_pushButton_reload_pressed);
    connect(ui->pushButton_update, &QPushButton::clicked, this, &MainWindow::on_pushButton_update_Clicked);
    connect(ui->pushButton_reset, &QPushButton::clicked, this, &MainWindow::on_pushButton_reset_clicked);


    // Initialize the table widget
    ui->tableWidget->setColumnCount(7); // Increase column count to accommodate the delete button
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "Name" << "Quantity" << "Price" << "Image" << "Delete" << "Update");
    // Set row height
    int rowHeight = 70; // Adjust this value as needed
    int rowWidth = 120; // Adjust this value as needed
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(rowHeight);
    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(rowWidth);
    ui->pushButton_update->setVisible(false);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    Produit P;
    // setting the values
    QString priceText = ui->lineEdit_price->text(); // text value from input field
    double price = priceText.toDouble(); // value switched to double
    P.setprice(price);

    QString quantityText = ui->lineEdit_Quantity->text();
    int quantity = quantityText.toInt();
    P.setquantity(quantity);

    P.settotal_price(price, quantity);

    P.setproduit(ui->lineEdit_Produit_name->text());

    // Insert data into the database
    QSqlQuery query;
    query.prepare("INSERT INTO PRODUIT (NOM_PRODUIT, QUANTITE_PRODUIT, PRIX_PRODUIT, IMAGE_PRODUIT) "
                  "VALUES (:name, :quantity, :price, :imagePath)");
    query.bindValue(":name", P.get_produit());
    query.bindValue(":quantity", P.get_quantity());
    query.bindValue(":price", P.get_price());
    query.bindValue(":imagePath", ui->lineEdit_choose_picture->text());

    if (query.exec()) {
        QMessageBox::information(this, tr("Success"), tr("Data saved successfully."));
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Failed to save data: ") + query.lastError().text());
    }
    ui->lineEdit_price->clear();
    ui->lineEdit_Quantity->clear();
    ui->lineEdit_Produit_name->clear();
    ui->lineEdit_choose_picture->clear();
}


void MainWindow::on_pushButton_choose_file_pressed(){
    QString filename = QFileDialog::getOpenFileName(
                        this,
                        tr("Open File"),
                        "C://",
                        "Image files (*.png *.jpg)"
                        );

    if (!filename.isEmpty()) {
        // Check if the selected file is an image (PNG or JPG)
        QFileInfo fileInfo(filename);
        QString extension = fileInfo.suffix();
        if (extension.toLower() == "png" || extension.toLower() == "jpg") {
            ui->lineEdit_choose_picture->setText(filename);// Set the text of the QLineEdit with the selected file path
        } else {
            // Display an error message if the selected file is not an image
            QMessageBox::warning(this, tr("Invalid File"),tr("Please select a PNG or JPG file."));
        }
    }
}


void MainWindow::on_pushButton_reload_pressed() {
    // Create a QSqlQueryModel and set the query
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT ID_PRODUIT, NOM_PRODUIT, QUANTITE_PRODUIT, PRIX_PRODUIT, IMAGE_PRODUIT FROM PRODUIT");
    if (model->lastError().isValid()) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to retrieve data: ") + model->lastError().text());
        delete model;
        return;
    }
    // Set the model to the table widget
    ui->tableWidget->setRowCount(model->rowCount());
    ui->tableWidget->setColumnCount(model->columnCount() + 2); // Add two columns for the delete and update buttons
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setFlags(item->flags() & ~Qt::ItemIsEditable); // Make cells read-only
            if (col == MainWindow::IMAGE_COLUMN_INDEX) {
                QString imagePath = model->data(model->index(row, col)).toString();
                QPixmap image(imagePath);
                if (!image.isNull()) {
                    // Resize the image to fit the row height
                    QSize scaledSize = image.size();
                    scaledSize.scale(this->imageSize, this->imageSize, Qt::KeepAspectRatio);
                    QPixmap scaledImage = image.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    // Set the scaled image as the decoration
                    item->setData(Qt::DecorationRole, scaledImage);
                    item->setData(Qt::UserRole, imagePath); // Set the image path as user data
                } else {
                    item->setText("Image not found");
                }
            } else {
                item->setData(Qt::DisplayRole, model->data(model->index(row, col)));
                item->setTextAlignment(Qt::AlignCenter); // Center the text
            }
            ui->tableWidget->setItem(row, col, item);
        }
        // Add delete button to the second last column
        QPushButton *deleteButton = new QPushButton("Delete");
        deleteButton->setStyleSheet("QPushButton { font: 700 16pt 'Sitka Subheading'; background-color: red; }");
        connect(deleteButton, &QPushButton::clicked, this, [this, row]() {
            onDeleteButtonClicked(row);
        });
        ui->tableWidget->setCellWidget(row, model->columnCount(), deleteButton);

        // Add update button to the last column
        QPushButton *updateButton = new QPushButton("Update");
        updateButton->setStyleSheet("QPushButton { font: 700 16pt 'Sitka Subheading'; background-color: green; }");
        connect(updateButton, &QPushButton::clicked, this, [this, row]() {
            onRowUpdateClicked(row); // Call the function to handle row update
        });
        ui->tableWidget->setCellWidget(row, model->columnCount() + 1, updateButton);
    }
    delete model; // Release the memory
}


void MainWindow::onRowUpdateClicked(int row) {
    ui->pushButton_update->setVisible(true);
    // Get the ID from the selected row
    int productId = ui->tableWidget->item(row, 0)->text().toInt();
    // Query the database to get the image path for the selected product
    QSqlQuery query;
    query.prepare("SELECT IMAGE_PRODUIT FROM PRODUIT WHERE ID_PRODUIT = :id");
    query.bindValue(":id", productId);
    if (query.exec() && query.next()) {
        QString imagePath = query.value(0).toString();
        // Get other data from the selected row
        QString produit = ui->tableWidget->item(row, 1)->text();
        QString quantity = ui->tableWidget->item(row, 2)->text();
        QString price = ui->tableWidget->item(row, 3)->text();
        // Populate the form fields with the data
        ui->lineEdit_ID->setText(QString::number(productId));
        ui->lineEdit_Produit_name->setText(produit);
        ui->lineEdit_Quantity->setText(quantity);
        ui->lineEdit_price->setText(price);
        ui->lineEdit_choose_picture->setText(imagePath);
        // Store the selected row for update
        selectedRowForUpdate = productId;
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Failed to retrieve image path for product ID: %1").arg(productId));
    }
}



void MainWindow::on_pushButton_update_Clicked(){
    int productId = ui->lineEdit_ID->text().toInt();
    QString produit = ui->lineEdit_Produit_name->text();
    int quantity = ui->lineEdit_Quantity->text().toInt();
    double price = ui->lineEdit_price->text().toDouble();
    QString imagePath = ui->lineEdit_choose_picture->text();
    // Update data in the database
    QSqlQuery query;
    query.prepare("UPDATE PRODUIT SET NOM_PRODUIT = :produit, QUANTITE_PRODUIT = :quantity, PRIX_PRODUIT = :price, IMAGE_PRODUIT = :imagePath WHERE ID_PRODUIT = :id");
    query.bindValue(":produit", produit);
    query.bindValue(":quantity", quantity);
    query.bindValue(":price", price);
    query.bindValue(":imagePath", imagePath);
    query.bindValue(":id", productId);
    if (query.exec()) {
        QMessageBox::information(this, tr("Success"), tr("Data updated successfully."));
        ui->lineEdit_ID->clear();
        ui->lineEdit_price->clear();
        ui->lineEdit_Quantity->clear();
        ui->lineEdit_Produit_name->clear();
        ui->lineEdit_choose_picture->clear();
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Failed to update data: ") + query.lastError().text());
    }
    ui->pushButton_update->setVisible(false);
    ui->pushButton->setVisible(true);
}



void MainWindow::onDeleteButtonClicked(int row)
{
    QSqlTableModel *model = static_cast<QSqlTableModel*>(ui->tableWidget->model());
    QModelIndex index = model->index(row, 0); // Assuming the ID_PRODUIT is in the first column
    int id = model->data(index).toInt();

    // Perform deletion from the database
    QSqlQuery query;
    query.prepare("DELETE FROM PRODUIT WHERE ID_PRODUIT = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        // Update the table widget after successful deletion
        ui->tableWidget->removeRow(row);
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Failed to delete row: ") + query.lastError().text());
    }
}


void MainWindow::on_pushButton_reset_clicked()
{
    ui->lineEdit_ID->clear();
    ui->lineEdit_price->clear();
    ui->lineEdit_Quantity->clear();
    ui->lineEdit_Produit_name->clear();
    ui->lineEdit_choose_picture->clear();
}



void MainWindow::on_pushButton_2_pressed()
{
    close();
}



void MainWindow::on_pushButton_trier_produit_clicked()
{
    int sortingIndex = ui->comboBox_tri_produit->currentIndex();
    QString sortingOrder;
    switch (sortingIndex) {
    case 0:
        sortingOrder = "ID_PRODUIT ASC";
        break;
    case 1:
        sortingOrder = "ID_PRODUIT DESC";
        break;
    case 2:
        sortingOrder = "NOM_PRODUIT ASC";
        break;
    case 3:
        sortingOrder = "NOM_PRODUIT DESC";
        break;
    case 4:
        sortingOrder = "QUANTITE_PRODUIT ASC";
        break;
    case 5:
        sortingOrder = "QUANTITE_PRODUIT DESC";
        break;
    case 6:
        sortingOrder = "PRIX_PRODUIT ASC";
        break;
    case 7:
        sortingOrder = "PRIX_PRODUIT DESC";
        break;
    default:
        sortingOrder = "ID_PRODUIT ASC";
        break;
    }

    QString queryString = QString("SELECT ID_PRODUIT, NOM_PRODUIT, QUANTITE_PRODUIT, PRIX_PRODUIT, IMAGE_PRODUIT FROM PRODUIT ORDER BY %1").arg(sortingOrder);

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
            item->setFlags(item->flags() & ~Qt::ItemIsEditable); // Make cells read-only
            if (col == MainWindow::IMAGE_COLUMN_INDEX) {
                QString imagePath = model->data(model->index(row, col)).toString();
                QPixmap image(imagePath);
                if (!image.isNull()) {
                    // Resize the image to fit the row height
                    QSize scaledSize = image.size();
                    scaledSize.scale(this->imageSize, this->imageSize, Qt::KeepAspectRatio);
                    QPixmap scaledImage = image.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    // Set the scaled image as the decoration
                    item->setData(Qt::DecorationRole, scaledImage);
                    item->setData(Qt::UserRole, imagePath); // Set the image path as user data
                } else {
                    item->setText("Image not found");
                }
            } else {
                item->setData(Qt::DisplayRole, model->data(model->index(row, col)));
                item->setTextAlignment(Qt::AlignCenter); // Center the text
            }
            ui->tableWidget->setItem(row, col, item);
        }
        // Add delete button to the second last column
        QPushButton *deleteButton = new QPushButton("Delete");
        deleteButton->setStyleSheet("QPushButton { font: 700 16pt 'Sitka Subheading'; background-color: red; }");
        connect(deleteButton, &QPushButton::clicked, this, [this, row]() {
            onDeleteButtonClicked(row);
        });
        ui->tableWidget->setCellWidget(row, model->columnCount(), deleteButton);

        // Add update button to the last column
        QPushButton *updateButton = new QPushButton("Update");
        updateButton->setStyleSheet("QPushButton { font: 700 16pt 'Sitka Subheading'; background-color: green; }");
        connect(updateButton, &QPushButton::clicked, this, [this, row]() {
            onRowUpdateClicked(row); // Call the function to handle row update
        });
        ui->tableWidget->setCellWidget(row, model->columnCount() + 1, updateButton);
    }

    delete model;
}



void MainWindow::on_pushButton_search_produit_clicked()
{
    QString keyword = ui->lineEdit_search_produit->text().trimmed();

    QString queryString = QString("SELECT ID_PRODUIT, NOM_PRODUIT, QUANTITE_PRODUIT, PRIX_PRODUIT, IMAGE_PRODUIT FROM PRODUIT WHERE ");
    queryString += "NOM_PRODUIT LIKE '%" + keyword + "%' OR ";
    queryString += "QUANTITE_PRODUIT LIKE '%" + keyword + "%' OR ";
    queryString += "PRIX_PRODUIT LIKE '%" + keyword + "%'";

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
            item->setFlags(item->flags() & ~Qt::ItemIsEditable); // Make cells read-only
            if (col == MainWindow::IMAGE_COLUMN_INDEX) {
                QString imagePath = model->data(model->index(row, col)).toString();
                QPixmap image(imagePath);
                if (!image.isNull()) {
                    // Resize the image to fit the row height
                    QSize scaledSize = image.size();
                    scaledSize.scale(this->imageSize, this->imageSize, Qt::KeepAspectRatio);
                    QPixmap scaledImage = image.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    // Set the scaled image as the decoration
                    item->setData(Qt::DecorationRole, scaledImage);
                    item->setData(Qt::UserRole, imagePath); // Set the image path as user data
                } else {
                    item->setText("Image not found");
                }
            } else {
                item->setData(Qt::DisplayRole, model->data(model->index(row, col)));
                item->setTextAlignment(Qt::AlignCenter); // Center the text
            }
            // Add delete button to the second last column
            QPushButton *deleteButton = new QPushButton("Delete");
            deleteButton->setStyleSheet("QPushButton { font: 700 16pt 'Sitka Subheading'; background-color: red; }");
            connect(deleteButton, &QPushButton::clicked, this, [this, row]() {
                onDeleteButtonClicked(row);
            });
            ui->tableWidget->setCellWidget(row, model->columnCount(), deleteButton);

            // Add update button to the last column
            QPushButton *updateButton = new QPushButton("Update");
            updateButton->setStyleSheet("QPushButton { font: 700 16pt 'Sitka Subheading'; background-color: green; }");
            connect(updateButton, &QPushButton::clicked, this, [this, row]() {
                onRowUpdateClicked(row); // Call the function to handle row update
            });
            ui->tableWidget->setCellWidget(row, model->columnCount() + 1, updateButton);
            ui->tableWidget->setItem(row, col, item);
    }
    delete model;
    }
}
