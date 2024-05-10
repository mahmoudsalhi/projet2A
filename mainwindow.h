#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_reset_dons_clicked();

    void on_pushButton_ajouter_dons_clicked();

    void on_afficher_dons_pushbutton_clicked();

    void onDeleteButtonClicked_dons(int row);
    void onRowUpdateClicked_dons(int row);

    void on_pushButton_update_dons_clicked();

private:
    Ui::MainWindow *ui;
    int selectedRowForUpdate_dons;
};
#endif // MAINWINDOW_H
