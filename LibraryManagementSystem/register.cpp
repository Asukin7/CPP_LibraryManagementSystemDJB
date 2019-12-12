#include "register.h"
#include "ui_register.h"

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
}

Register::~Register()
{
    delete ui;
}

void Register::on_pushButton_Register_clicked()
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
    QString LEPassword2 = ui->lineEdit_Password_2->text();
    if(LEPassword2.length() == 0)
    {
        QMessageBox::information(this,"错误","请再输入密码");
        return;
    }
    if(LEUsername.length() < 6)
    {
        QMessageBox::information(this,"错误","账号长度不可小于 6 位");
        return;
    }
    if(LEPassword.length() < 6)
    {
        QMessageBox::information(this,"错误","密码长度不可小于 6 位");
        return;
    }
    if(LEPassword != LEPassword2)
    {
        QMessageBox::information(this,"错误","密码输入不一致");
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
    if(query.next())
    {
        QMessageBox::information(this,"错误","账号已存在");
        return;
    }
    else
    {
        query.exec(QString("insert into %1 values ('%2','%3')").arg(managerORuser,LEUsername,LEPassword));
        QMessageBox::information(this,"成功","账号注册成功");
    }
}
