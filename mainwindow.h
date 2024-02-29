#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_pressed();
    void on_pushButton_choose_file_pressed();
    void on_pushButton_reload_pressed();
    void onDeleteButtonClicked(int row);
    void onRowUpdateClicked(int row);
    void on_pushButton_update_Clicked();
    void on_pushButton_reset_clicked();

    void on_pushButton_trier_produit_clicked();

    void on_pushButton_search_produit_clicked();

private:
    Ui::MainWindow *ui;
    const int imageSize = 80; // Adjust this value as needed
    static const int IMAGE_COLUMN_INDEX = 4;
    int selectedRowForUpdate;
};
#endif // MAINWINDOW_H
