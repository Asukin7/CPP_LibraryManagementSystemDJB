#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QtSql>
#include <QtCore>
#include "login.h"
#include "register.h"
#include "statistics.h"

extern QString zhanghao;
extern int quanxian;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void label_Time_TimeUpDate();

    void on_pushButton_Login_clicked();

    void on_pushButton_Register_clicked();

    void on_pushButton_1_Search_clicked();

    void on_tableView_1_clicked();

    void on_pushButton_1_Rent_clicked();

    void on_pushButton_1_Previous_clicked();

    void on_pushButton_1_Next_clicked();

    void on_pushButton_1_Management_Add_clicked();

    void on_pushButton_1_Management_Update_clicked();

    void on_pushButton_1_Management_delete_clicked();

    void on_pushButton_2_Search_clicked();

    void on_tableView_2_clicked();

    void on_pushButton_2_return_clicked();

    void on_pushButton_3_Search_clicked();

    void on_tableView_3_clicked();

    void on_pushButton_3_Update_clicked();

    void on_pushButton_3_Update_Save_clicked();

    void on_pushButton_3_Update_Cancel_clicked();

    void on_pushButton_3_Delete_clicked();

    void on_pushButton_3_Previous_clicked();

    void on_pushButton_3_Next_clicked();

    void on_pushButton_4_Manager_Update_clicked();

    void on_pushButton_4_Manager_Delete_clicked();

    void on_pushButton_4_Manager_Operation_Cancel_clicked();

    void on_pushButton_4_Data_Backup_clicked();

    void on_pushButton_4_Data_Restore_clicked();

    void on_pushButton_4_Software_About_clicked();

    void on_pushButton_Statistics_clicked();

private:
    Ui::MainWindow *ui;

    statistics *tu;

    QByteArray ReadData();
};

#endif // MAINWINDOW_H
