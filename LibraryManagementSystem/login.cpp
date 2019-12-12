#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_Login_clicked()
{
    QString LEUsername = ui->lineEdit_Username->text();
    if(LEUsername.length() == 0)
    {
        QMessageBox::information(this,"错误","请输入账号");
        return;
    }
    QString LEPassword = ui->lineEdit_Password->text();
    if(LEPassword.length() == 0)
    {
        QMessageBox::information(this,"错误","请输入密码");
        return;
    }
    QString managerORuser;
    if(ui->checkBox->isChecked())
    {
        managerORuser = "manager";
    }
    else
    {
        managerORuser = "[user]";
    }
    QSqlQuery query;
    query.exec(QString("select * from %1 where Username = '%2'").arg(managerORuser,LEUsername));
    QSqlRecord rec = query.record();
    if(query.next())
    {
        int ISQLPassword = rec.indexOf("Password");
        QString QSQLPassword = query.value(ISQLPassword).toString();
        if (LEPassword == QSQLPassword)
        {
            zhanghao = ui->lineEdit_Username->text();
            if(ui->checkBox->isChecked())
            {
                quanxian = 0;
            }
            else
            {
                quanxian = 1;
            }
            QMessageBox::information(this,"成功","登录成功");
            Login::accept();
            return;
        }
        else
        {
            QMessageBox::information(this,"错误","密码错误");
            return;
        }
    }
    else
    {
        QMessageBox::information(this,"错误","账号不存在");
        return;
    }
}
