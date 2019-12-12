#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*******初始化用户信息*******/
    zhanghao = "NULL";
    quanxian = -1;
    /*******END*******/

    /*******初始化label_Time*******/
    QTimer *timer = new QTimer(this);//新建定时器
    connect(timer,SIGNAL(timeout()),this,SLOT(label_Time_TimeUpDate()));//关联定时器计满信号和相应的槽函数
    timer->start(1000);//定时器开始计时，其中1000表示1000ms即1秒
    /*******END*******/

    /*******初始化tabWidget*******/
    for(int i = 1;i <= 3;i++)//禁用页面1~3
    {
        ui->tabWidget->setTabEnabled(i, false);
    }
    ui->tabWidget->setStyleSheet("QTabBar::tab:disabled {width: 0; color: transparent;}");//设置页面透明
    /*******END*******/

    /*******初始化tab_1*******/
    ui->tableView_1->setSelectionBehavior(QAbstractItemView::SelectRows);//设置只能选择行
    ui->tableView_1->setSelectionMode(QAbstractItemView::SingleSelection);//设置只能单选
    ui->pushButton_1_Rent->setEnabled(false);
    ui->pushButton_1_Previous->setEnabled(false);
    ui->pushButton_1_Next->setEnabled(false);
    ui->tableView_1->verticalHeader()->hide();
    ui->groupBox_1_Management->hide();
    QRegExp RegExp1("^(([1-9]\\d*)(\\.\\d*)?)|((0)(\\.\\d*))$");//只能输入非负数
    QValidator *Validator1 = new QRegExpValidator(RegExp1,this);
    ui->lineEdit_1_Management_Price->setValidator(Validator1);
    QRegExp RegExp2("^\\d*$");//只能输入非负整数
    QValidator *Validator2 = new QRegExpValidator(RegExp2,this);
    ui->lineEdit_1_Management_Number->setValidator(Validator2);
    QRegExp RegExp3("^\\d*$");//只能输入非负整数
    QValidator *Validator3 = new QRegExpValidator(RegExp3,this);
    ui->lineEdit_1_Management_Number->setValidator(Validator3);
    /*******END*******/

    /*******初始化tab_2*******/
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);//设置只能选择行
    ui->tableView_2->setSelectionMode(QAbstractItemView::SingleSelection);//设置只能单选
    ui->pushButton_2_return->setEnabled(false);
    ui->tableView_2->verticalHeader()->hide();
    /*******END*******/

    /*******初始化tab_3*******/
    ui->tableView_3->setSelectionBehavior(QAbstractItemView::SelectRows);//设置只能选择行
    ui->tableView_3->setSelectionMode(QAbstractItemView::SingleSelection);//设置只能单选
    ui->pushButton_3_Update->setEnabled(false);
    ui->pushButton_3_Delete->setEnabled(false);
    ui->pushButton_3_Previous->setEnabled(false);
    ui->pushButton_3_Next->setEnabled(false);
    ui->tableView_3->verticalHeader()->hide();
    ui->groupBox_3_Update->hide();
    /*******END*******/

    /*******初始化tab_4*******/
    ui->groupBox_4_Manager_Operation->hide();
    /*******END*******/

    on_pushButton_Statistics_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::label_Time_TimeUpDate()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy年MM月dd日dddd   hh:mm:ss");//设置系统时间显示格式
    ui->label_Time->setText(str);//在标签上显示时间
}

void MainWindow::on_pushButton_Login_clicked()
{
    //注销时重置主窗口//
    if(zhanghao != "NULL")
    {
        QMessageBox::information(this,"成功","注销成功");
        qApp->exit(666);
    }

    Login L;
    L.exec();
    //登录成功时打开相应功能//
    if(zhanghao != "NULL")
    {
        ui->pushButton_Login->setText("注销");
        ui->label_zhanghao->setText("当前账号：" + zhanghao);
        if(quanxian > 0)
        {
            ui->tabWidget->setTabEnabled(1, true);
        }
        if(quanxian == 0)
        {
            ui->groupBox_1_Management->show();
            ui->tabWidget->setTabEnabled(2, true);
            ui->tabWidget->setTabEnabled(3, true);
        }
        ui->tabWidget->setStyleSheet("QTabBar::tab:disabled{width:0;color:transparent;}");
    }
}

void MainWindow::on_pushButton_Register_clicked()
{
    //判断是否为管理员//
    if(quanxian != 0)
    {
        QMessageBox::information(this,"通知","请联系管理员注册账号");
        return;
    }

    Register R;
    R.exec();
}

void MainWindow::on_pushButton_1_Search_clicked()
{
    ui->pushButton_1_Rent->setEnabled(false);
    ui->pushButton_1_Previous->setEnabled(false);
    ui->pushButton_1_Next->setEnabled(false);

    QSqlQuery query;
    QSqlQueryModel *model = new QSqlQueryModel(ui->tableView_1);
    QString LESearch = ui->lineEdit_1_Search->text();
    QString CBSearch = ui->comboBox_1_Search->currentText();
    //判断根据编号或是书名搜索//
    if(CBSearch == "编号")
    {
        CBSearch = "BookNumber";
    }
    else if(CBSearch == "书名")
    {
        CBSearch = "Name";
    }
    //判断是否存在书籍//
    query.exec(QString("select * from book where %1 like '%%2%'").arg(CBSearch,LESearch));
    if(!query.next())
    {
        QMessageBox::information(this,"错误",QString("未找到书籍 %1 ").arg(LESearch));
        return;
    }
    //根据选择的搜索方式进行搜索前20条数据//
    model->setQuery(QString("select * from (select ROW_NUMBER() over(order by %1) as Num,* from (select * from book where %2 like '%%3%') as t) as t0 where Num >= 1 AND Num <= 20").arg(CBSearch,CBSearch,LESearch));
    //判断是否存在第20条后的数据//
    query.exec(QString("select * from (select ROW_NUMBER() over(order by %1) as Num,* from (select * from book where %2 like '%%3%') as t) as t0 where Num >= 21 AND Num <= 40").arg(CBSearch,CBSearch,LESearch));
    if(query.next())
    {
        ui->pushButton_1_Next->setEnabled(true);
    }

    model->setHeaderData(0, Qt::Horizontal, tr(""));
    model->setHeaderData(1, Qt::Horizontal, tr("编号"));
    model->setHeaderData(2, Qt::Horizontal, tr("书名"));
    model->setHeaderData(3, Qt::Horizontal, tr("价格"));
    model->setHeaderData(4, Qt::Horizontal, tr("库存"));
    model->setHeaderData(5, Qt::Horizontal, tr("总数"));

    ui->tableView_1->setModel(model);
}

void MainWindow::on_tableView_1_clicked()
{
    ui->pushButton_1_Rent->setEnabled(true);
    //如果是管理员则将选中的书籍信息读到管理框//
    if(quanxian != 0)
    {
        return;
    }
    QAbstractItemModel *model = ui->tableView_1->model();
    QModelIndex index;
    QString Qindex;
    int row = ui->tableView_1->currentIndex().row();
    index = model->index(row,1);
    Qindex = model->data(index).toString();
    ui->lineEdit_1_Management_BookNumber->setText(Qindex);
    index = model->index(row,2);
    Qindex = model->data(index).toString();
    ui->lineEdit_1_Management_Name->setText(Qindex);
    index = model->index(row,3);
    Qindex = model->data(index).toString();
    ui->lineEdit_1_Management_Price->setText(Qindex);
    index = model->index(row,4);
    Qindex = model->data(index).toString();
    ui->lineEdit_1_Management_Number->setText(Qindex);
    index = model->index(row,5);
    Qindex = model->data(index).toString();
    ui->lineEdit_1_Management_Total->setText(Qindex);
}

void MainWindow::on_pushButton_1_Rent_clicked()
{
    //判断是否为用户//
    if(quanxian == -1)
    {
        on_pushButton_Login_clicked();
        return;
    }
    if(quanxian == 0)
    {
        QMessageBox::information(this,"错误","请使用用户账号借阅书籍");
        return;
    }

    QSqlQuery query;
    QAbstractItemModel *model = ui->tableView_1->model();
    QModelIndex index;
    QString Qindex;
    int Iindex;
    int row = ui->tableView_1->currentIndex().row();
    //读取选中的书籍的编号和库存//
    index = model->index(row,1);
    Qindex = model->data(index).toString();
    index = model->index(row,4);
    Iindex = model->data(index).toInt();
    //判断库存是否为0//
    if(Iindex == 0)
    {
        QMessageBox::information(this,"错误",QString("书籍 %1 库存为 0 \n借阅失败").arg(Qindex));
        return;
    }
    //判断用户是否已借该书籍//
    query.exec(QString("select * from rent where Username = '%1' and BookNumber = '%2'").arg(zhanghao,Qindex));
    if(query.next())
    {
        QMessageBox::information(this,"错误",QString("用户 %1 已借阅 %2 \n借阅失败").arg(zhanghao,Qindex));
        return;
    }
    //确认是否借阅书籍//
    QMessageBox messageBox(QMessageBox::NoIcon,"借阅",QString("是否借阅书籍 %1 ").arg(Qindex),QMessageBox::Yes|QMessageBox::No);
    int result=messageBox.exec();
    switch (result)
    {
    case QMessageBox::Yes:
    {
        QDateTime QDT = QDateTime::currentDateTime();
        QString QDate = QDT.toString("yyyy-MM-dd");
        query.exec(QString("insert into rent values ('%1','%2','%3','%4')").arg(zhanghao,Qindex,"1",QDate));
        query.exec(QString("update book set Number = Number - 1 where BookNumber = '%1'").arg(Qindex));
        QMessageBox::information(this,"成功","借阅成功");
        ui->pushButton_1_Rent->setEnabled(false);
        break;
    }
    case QMessageBox::No:
    {
        return;
    }
    default:break;
    }

    on_pushButton_1_Search_clicked();
}

void MainWindow::on_pushButton_1_Previous_clicked()
{
    ui->pushButton_1_Rent->setEnabled(false);
    ui->pushButton_1_Previous->setEnabled(false);
    ui->pushButton_1_Next->setEnabled(true);

    QSqlQuery query;
    QSqlQueryModel *model = new QSqlQueryModel(ui->tableView_1);
    QAbstractItemModel *index = ui->tableView_1->model();
    QString LESearch = ui->lineEdit_1_Search->text();
    QString CBSearch = ui->comboBox_1_Search->currentText();
    //设置查找范围//
    int HeadN = index->data(index->index(0,0)).toInt() - 20;
    int TailN = HeadN + 19;
    //判断根据编号或是书名搜索//
    if(CBSearch == "编号")
    {
        CBSearch = "BookNumber";
    }
    else if(CBSearch == "书名")
    {
        CBSearch = "Name";
    }
    //判断是否存在书籍//
    query.exec(QString("select * from book where %1 like '%%2%'").arg(CBSearch,LESearch));
    if(!query.next())
    {
        QMessageBox::information(this,"错误",QString("未找到书籍 %1 ").arg(LESearch));
        return;
    }
    //根据设置的查找范围以及选择的搜索方式进行搜索数据//
    model->setQuery(QString("select * from (select ROW_NUMBER() over(order by %1) as Num,* from (select * from book where %2 like '%%3%') as t) as t0 where Num >= %4 AND Num <= %5").arg(CBSearch,CBSearch,LESearch,QString::number(HeadN),QString::number(TailN)));
    //判断是否存在查找范围前的数据//
    query.exec(QString("select * from (select ROW_NUMBER() over(order by %1) as Num,* from (select * from book where %2 like '%%3%') as t) as t0 where Num >= %4 AND Num <= %5").arg(CBSearch,CBSearch,LESearch,QString::number(HeadN - 20),QString::number(TailN - 20)));
    if(query.next())
    {
        ui->pushButton_1_Previous->setEnabled(true);
    }

    model->setHeaderData(0, Qt::Horizontal, tr(""));
    model->setHeaderData(1, Qt::Horizontal, tr("编号"));
    model->setHeaderData(2, Qt::Horizontal, tr("书名"));
    model->setHeaderData(3, Qt::Horizontal, tr("价格"));
    model->setHeaderData(4, Qt::Horizontal, tr("库存"));
    model->setHeaderData(5, Qt::Horizontal, tr("总数"));

    ui->tableView_1->setModel(model);
}

void MainWindow::on_pushButton_1_Next_clicked()
{
    ui->pushButton_1_Rent->setEnabled(false);
    ui->pushButton_1_Previous->setEnabled(true);
    ui->pushButton_1_Next->setEnabled(false);

    QSqlQuery query;
    QSqlQueryModel *model = new QSqlQueryModel(ui->tableView_1);
    QAbstractItemModel *index = ui->tableView_1->model();
    QString LESearch = ui->lineEdit_1_Search->text();
    QString CBSearch = ui->comboBox_1_Search->currentText();
    //设置查找范围//
    int HeadN = index->data(index->index(0,0)).toInt() + 20;
    int TailN = HeadN + 19;
    //判断根据编号或是书名搜索//
    if(CBSearch == "编号")
    {
        CBSearch = "BookNumber";
    }
    else if(CBSearch == "书名")
    {
        CBSearch = "Name";
    }
    //判断是否存在书籍//
    query.exec(QString("select * from book where %1 like '%%2%'").arg(CBSearch,LESearch));
    if(!query.next())
    {
        QMessageBox::information(this,"错误",QString("未找到书籍 %1 ").arg(LESearch));
        return;
    }
    //根据设置的查找范围以及选择的搜索方式进行搜索数据//
    model->setQuery(QString("select * from (select ROW_NUMBER() over(order by %1) as Num,* from (select * from book where %2 like '%%3%') as t) as t0 where Num >= %4 AND Num <= %5").arg(CBSearch,CBSearch,LESearch,QString::number(HeadN),QString::number(TailN)));
    //判断是否存在查找范围后的数据//
    query.exec(QString("select * from (select ROW_NUMBER() over(order by %1) as Num,* from (select * from book where %2 like '%%3%') as t) as t0 where Num >= %4 AND Num <= %5").arg(CBSearch,CBSearch,LESearch,QString::number(HeadN + 20),QString::number(TailN + 20)));
    if(query.next())
    {
        ui->pushButton_1_Next->setEnabled(true);
    }

    model->setHeaderData(0, Qt::Horizontal, tr(""));
    model->setHeaderData(1, Qt::Horizontal, tr("编号"));
    model->setHeaderData(2, Qt::Horizontal, tr("书名"));
    model->setHeaderData(3, Qt::Horizontal, tr("价格"));
    model->setHeaderData(4, Qt::Horizontal, tr("库存"));
    model->setHeaderData(5, Qt::Horizontal, tr("总数"));

    ui->tableView_1->setModel(model);
}

void MainWindow::on_pushButton_1_Management_Add_clicked()
{
    QSqlQuery query;
    QString LEBookNumber = ui->lineEdit_1_Management_BookNumber->text();
    QString LEName = ui->lineEdit_1_Management_Name->text();
    QString LEPrice = ui->lineEdit_1_Management_Price->text();
    QString LENumber = ui->lineEdit_1_Management_Number->text();
    QString LETotal = ui->lineEdit_1_Management_Total->text();
    //检查输入信息是否存在问题//
    if(LEBookNumber.length() == 0 || LEName.length() == 0 || LEPrice.length() == 0 || LENumber.length() == 0 || LETotal.length() == 0)
    {
        QMessageBox::information(this,"错误",QString("存在信息未填\n添加失败"));
        return;
    }
    if(LEBookNumber.length() != 6)
    {
        QMessageBox::information(this,"错误",QString("编号长度应为 6 位\n添加失败"));
        return;
    }
    if(LEPrice.toFloat() == float(0))
    {
        QMessageBox::information(this,"错误",QString("价格不能为 0 \n添加失败"));
        return;
    }
    if(LENumber.toInt() != LETotal.toInt())
    {
        QMessageBox::information(this,"错误",QString("库存和总数不相等\n添加失败"));
        return;
    }
    //判断是否存在书籍//
    query.exec(QString("select * from book where BookNumber = '%1'").arg(LEBookNumber));
    if(query.next())
    {
        QMessageBox::information(this,"错误",QString("已存在书籍 %1 \n添加失败").arg(LEBookNumber));
        return;
    }
    //添加书籍//
    query.exec(QString("insert into book values ('%1','%2','%3','%4','%5')").arg(LEBookNumber,LEName,LEPrice,LENumber,LETotal));
    QMessageBox::information(this,"成功",QString("添加书籍 %1 成功").arg(LEBookNumber));

    on_pushButton_1_Search_clicked();
}

void MainWindow::on_pushButton_1_Management_Update_clicked()
{
    QSqlQuery query;
    QString LEBookNumber = ui->lineEdit_1_Management_BookNumber->text();
    QString LEName = ui->lineEdit_1_Management_Name->text();
    QString LEPrice = ui->lineEdit_1_Management_Price->text();
    QString LENumber = ui->lineEdit_1_Management_Number->text();
    QString LETotal = ui->lineEdit_1_Management_Total->text();
    //检查输入信息是否存在问题//
    if(LEBookNumber.length() == 0 || LEName.length() == 0 || LEPrice.length() == 0 || LENumber.length() == 0 || LETotal.length() == 0)
    {
        QMessageBox::information(this,"错误",QString("存在信息未填\n修改失败"));
        return;
    }
    if(LEBookNumber.length() != 6)
    {
        QMessageBox::information(this,"错误",QString("编号长度应为 6 位\n修改失败"));
        return;
    }
    if(LEPrice.toFloat() == float(0))
    {
        QMessageBox::information(this,"错误",QString("价格不能为 0 \n修改失败"));
        return;
    }
    //判断是否存在书籍//
    query.exec(QString("select * from book where BookNumber = '%1'").arg(LEBookNumber));
    if(!query.next())
    {
        QMessageBox::information(this,"错误",QString("不存在书籍 %1 \n修改失败").arg(LEBookNumber));
        return;
    }
    //判断库存与总数变化量是否相等//
    QSqlRecord rec = query.record();
    int NumberChange = LENumber.toInt() - query.value(rec.indexOf("Number")).toInt();
    int TotalChange = LETotal.toInt() - query.value(rec.indexOf("Total")).toInt();
    if(NumberChange != TotalChange)
    {
        QMessageBox::information(this,"错误",QString("库存与总数变化量不相等\n库存变化量为 %1 \n总数变化量为 %2 \n修改失败").arg(QString::number(NumberChange),QString::number(TotalChange)));
        return;
    }
    //修改书籍//
    query.exec(QString("update book set Name = '%1',Price = '%2',Number = '%3',Total = '%4' where BookNumber = '%5'").arg(LEName,LEPrice,LENumber,LETotal,LEBookNumber));
    QMessageBox::information(this,"成功",QString("修改书籍 %1 成功").arg(LEBookNumber));

    on_pushButton_1_Search_clicked();
}

void MainWindow::on_pushButton_1_Management_delete_clicked()
{
    QSqlQuery query;
    QString LEBookNumber = ui->lineEdit_1_Management_BookNumber->text();
    //检查输入信息是否存在问题//
    if(LEBookNumber.length() != 6)
    {
        QMessageBox::information(this,"错误",QString("书籍编号应为 6 位\n删除失败"));
        return;
    }
    //判断是否存在书籍//
    query.exec(QString("select * from book where BookNumber = '%1'").arg(LEBookNumber));
    if(!query.next())
    {
        QMessageBox::information(this,"错误",QString("不存在书籍 %1 \n删除失败").arg(LEBookNumber));
        return;
    }
    //判断是否存在未归还书籍//
    query.exec(QString("select * from rent where BookNumber = '%1'").arg(LEBookNumber));
    if(query.next())
    {
        QMessageBox::information(this,"错误",QString("存在未归还书籍 %1 \n删除失败").arg(LEBookNumber));
        return;
    }
    //删除书籍//
    query.exec(QString("delete from book where BookNumber = '%1'").arg(LEBookNumber));
    QMessageBox::information(this,"成功",QString("删除书籍 %1 成功").arg(LEBookNumber));

    on_pushButton_1_Search_clicked();
}

void MainWindow::on_pushButton_2_Search_clicked()
{
    ui->pushButton_2_return->setEnabled(false);

    QSqlQuery query;
    QSqlQueryModel *model = new QSqlQueryModel(ui->tableView_2);
    //判断用户是否存在借书记录//
    query.exec(QString("select * from rent where Username = '%1'").arg(zhanghao));
    if(!query.next())
    {
        QMessageBox::information(this,"错误",QString("用户 %1 无借书记录").arg(zhanghao));
        return;
    }
    //搜索用户借书记录//
    model->setQuery(QString("select ROW_NUMBER() over(order by rent.BookNumber) as Num,Username,rent.BookNumber,Name,RentNumber,RentDate from rent left join book on rent.BookNumber = book.BookNumber where Username = '%1'").arg(zhanghao));

    model->setHeaderData(0, Qt::Horizontal, tr(""));
    model->setHeaderData(1, Qt::Horizontal, tr("用户"));
    model->setHeaderData(2, Qt::Horizontal, tr("编号"));
    model->setHeaderData(3, Qt::Horizontal, tr("书名"));
    model->setHeaderData(4, Qt::Horizontal, tr("数量"));
    model->setHeaderData(5, Qt::Horizontal, tr("借阅日期"));

    ui->tableView_2->setModel(model);
}

void MainWindow::on_tableView_2_clicked()
{
    ui->pushButton_2_return->setEnabled(true);
}

void MainWindow::on_pushButton_2_return_clicked()
{
    QAbstractItemModel *model = ui->tableView_2->model();
    QModelIndex index;
    QString Qindex;
    int row = ui->tableView_2->currentIndex().row();
    //读取选中的书籍的编号//
    index = model->index(row,2);
    Qindex = model->data(index).toString();
    //确认是否归还书籍//
    QMessageBox messageBox(QMessageBox::NoIcon,"归还",QString("是否归还书籍 %1 ").arg(Qindex),QMessageBox::Yes|QMessageBox::No);
    int result=messageBox.exec();
    switch (result)
    {
    case QMessageBox::Yes:
    {
        QSqlQuery query;
        query.exec(QString("delete from rent where Username = '%1' and BookNumber = '%2'").arg(zhanghao,Qindex));
        query.exec(QString("update book set Number = Number + 1 where BookNumber = '%1'").arg(Qindex));
        QMessageBox::information(this,"成功","归还成功");
        ui->pushButton_2_return->setEnabled(false);
        break;
    }
    case QMessageBox::No:
    {
        return;
    }
    default:break;
    }

    on_pushButton_2_Search_clicked();
}

void MainWindow::on_pushButton_3_Search_clicked()
{
    ui->pushButton_3_Update->setEnabled(false);
    ui->pushButton_3_Delete->setEnabled(false);
    ui->pushButton_3_Previous->setEnabled(false);
    ui->pushButton_3_Next->setEnabled(false);
    ui->groupBox_3_Update->hide();

    QSqlQuery query;
    QSqlQueryModel *model = new QSqlQueryModel(ui->tableView_3);
    QString LESearch = ui->lineEdit_3_Search->text();
    //判断是否根据用户账号搜索//
    if(LESearch.length() == 0)
    {
        //搜索所有用户前20条数据//
        model->setQuery("select * from (select ROW_NUMBER() over(order by Username) as Num,* from [user]) as t where Num >= 1 AND Num <= 20");
        //判断是否存在第20条后的数据//
        query.exec("select * from (select ROW_NUMBER() over(order by Username) as Num,* from [user]) as t where Num >= 21 AND Num <= 40");
        if(query.next())
        {
            ui->pushButton_3_Next->setEnabled(true);
        }
    }
    else
    {
        //判断用户是否存在//
        query.exec(QString("select * from [user] where Username = '%1'").arg(LESearch));
        if(!query.next())
        {
            QMessageBox::information(this,"错误",QString("未找到用户 %1 ").arg(LESearch));
            return;
        }
        //搜索用户//
        model->setQuery(QString("select ROW_NUMBER() over(order by Username) as Num,* from [user] where Username = '%1'").arg(LESearch));
    }

    model->setHeaderData(0, Qt::Horizontal, tr(""));
    model->setHeaderData(1, Qt::Horizontal, tr("账号"));
    model->setHeaderData(2, Qt::Horizontal, tr("密码"));

    ui->tableView_3->setModel(model);
}

void MainWindow::on_tableView_3_clicked()
{
    ui->pushButton_3_Update->setEnabled(true);
    ui->pushButton_3_Delete->setEnabled(true);
    ui->groupBox_3_Update->hide();

    QAbstractItemModel *model = ui->tableView_3->model();
    QModelIndex index;
    QString Qindex;
    int row = ui->tableView_3->currentIndex().row();
    //将选中的用户信息读到修改框//
    index = model->index(row,1);
    Qindex = model->data(index).toString();
    ui->groupBox_3_Update->setTitle(Qindex);
    index = model->index(row,2);
    Qindex = model->data(index).toString();
    ui->lineEdit_3_Update_Password->setText(Qindex);
}

void MainWindow::on_pushButton_3_Update_clicked()
{
    ui->pushButton_3_Update->setEnabled(false);
    ui->pushButton_3_Delete->setEnabled(false);
    ui->groupBox_3_Update->show();
}

void MainWindow::on_pushButton_3_Update_Save_clicked()
{
    QSqlQuery query;
    QString GBUsername = ui->groupBox_3_Update->title();
    QString LEPassword = ui->lineEdit_3_Update_Password->text();
    //检查输入信息是否存在问题//
    if(LEPassword.length() < 6)
    {
        QMessageBox::information(this,"错误","密码长度不可小于 6 位\n修改失败");
        return;
    }
    //修改用户//
    query.exec(QString("update [user] set Password = '%1' where Username = '%2'").arg(LEPassword,GBUsername));
    QMessageBox::information(this,"成功",QString("修改用户 %1 信息成功").arg(GBUsername));

    on_pushButton_3_Search_clicked();

    ui->groupBox_3_Update->hide();
}

void MainWindow::on_pushButton_3_Update_Cancel_clicked()
{
    ui->groupBox_3_Update->hide();
}

void MainWindow::on_pushButton_3_Delete_clicked()
{
    ui->pushButton_3_Update->setEnabled(false);
    ui->pushButton_3_Delete->setEnabled(false);

    QSqlQuery query;
    QString GBUsername = ui->groupBox_3_Update->title();
    //判断用户是否存在未归还书籍//
    query.exec(QString("select * from rent where Username = '%1'").arg(GBUsername));
    if(query.next())
    {
        QMessageBox::information(this,"错误",QString("用户 %1 存在未归还书籍\n删除失败").arg(GBUsername));
        return;
    }
    //删除用户//
    query.exec(QString("delete from [user] where Username = '%1'").arg(GBUsername));
    QMessageBox::information(this,"成功",QString("删除用户 %1 信息成功").arg(GBUsername));

    on_pushButton_3_Search_clicked();
}

void MainWindow::on_pushButton_3_Previous_clicked()
{
    ui->pushButton_3_Update->setEnabled(false);
    ui->pushButton_3_Delete->setEnabled(false);
    ui->pushButton_3_Previous->setEnabled(false);
    ui->pushButton_3_Next->setEnabled(true);

    QSqlQuery query;
    QSqlQueryModel *model = new QSqlQueryModel(ui->tableView_3);
    QAbstractItemModel *index = ui->tableView_3->model();
    //设置查找范围//
    int HeadN = index->data(index->index(0,0)).toInt() - 20;
    int TailN = HeadN + 19;
    //根据设置的查找范围进行搜索数据//
    model->setQuery(QString("select * from (select ROW_NUMBER() over(order by Username) as Num,* from [user]) as t where Num >= %1 AND Num <= %2").arg(QString::number(HeadN),QString::number(TailN)));
    //判断是否存在查找范围前的数据//
    query.exec(QString("select * from (select ROW_NUMBER() over(order by Username) as Num,* from [user]) as t where Num >= %1 AND Num <= %2").arg(QString::number(HeadN - 20),QString::number(TailN - 20)));
    if(query.next())
    {
        ui->pushButton_3_Previous->setEnabled(true);
    }

    model->setHeaderData(0, Qt::Horizontal, tr(""));
    model->setHeaderData(1, Qt::Horizontal, tr("账号"));
    model->setHeaderData(2, Qt::Horizontal, tr("密码"));

    ui->tableView_3->setModel(model);
}

void MainWindow::on_pushButton_3_Next_clicked()
{
    ui->pushButton_3_Update->setEnabled(false);
    ui->pushButton_3_Delete->setEnabled(false);
    ui->pushButton_3_Previous->setEnabled(true);
    ui->pushButton_3_Next->setEnabled(false);

    QSqlQuery query;
    QSqlQueryModel *model = new QSqlQueryModel(ui->tableView_3);
    QAbstractItemModel *index = ui->tableView_3->model();
    //设置查找范围//
    int HeadN = index->data(index->index(0,0)).toInt() + 20;
    int TailN = HeadN + 19;
    //根据设置的查找范围进行搜索数据//
    model->setQuery(QString("select * from (select ROW_NUMBER() over(order by Username) as Num,* from [user]) as t where Num >= %1 AND Num <= %2").arg(QString::number(HeadN),QString::number(TailN)));
    //判断是否存在查找范围后的数据//
    query.exec(QString("select * from (select ROW_NUMBER() over(order by Username) as Num,* from [user]) as t where Num >= %1 AND Num <= %2").arg(QString::number(HeadN + 20),QString::number(TailN + 20)));
    if(query.next())
    {
        ui->pushButton_3_Next->setEnabled(true);
    }

    model->setHeaderData(0, Qt::Horizontal, tr(""));
    model->setHeaderData(1, Qt::Horizontal, tr("账号"));
    model->setHeaderData(2, Qt::Horizontal, tr("密码"));

    ui->tableView_3->setModel(model);
}

void MainWindow::on_pushButton_4_Manager_Update_clicked()
{
    //点击修改密码唤醒操作框//
    if(ui->pushButton_4_Manager_Update->text() == "修改密码")
    {
        ui->pushButton_4_Manager_Update->setText("确认修改");
        ui->pushButton_4_Manager_Delete->hide();
        ui->groupBox_4_Manager_Operation->show();
        return;
    }

    QSqlQuery query;
    QString LEPassword = ui->lineEdit_4_Manager_Operation_Password->text();
    QString LEPasswordUpdate = ui->lineEdit_4_Manager_Operation_Password_Update->text();
    QString LEPasswordUpdate2 = ui->lineEdit_4_Manager_Operation_Password_Update_2->text();
    //检查输入信息是否存在问题//
    if(LEPassword.length() == 0 || LEPasswordUpdate.length() == 0 || LEPasswordUpdate2.length() == 0)
    {
        QMessageBox::information(this,"错误","存在信息未填\n修改失败");
        return;
    }
    if(LEPasswordUpdate.length() < 6)
    {
        QMessageBox::information(this,"错误","密码长度不可小于 6 位");
        return;
    }
    if(LEPasswordUpdate != LEPasswordUpdate2)
    {
        QMessageBox::information(this,"错误","密码输入不一致");
        return;
    }
    //判断密码是否错误//
    query.exec(QString("select * from manager where Username = '%1' and Password = '%2'").arg(zhanghao,LEPassword));
    if(!query.next())
    {
        QMessageBox::information(this,"错误","密码错误");
        return;
    }
    //修改密码//
    query.exec(QString("update manager set Password = '%1' where Username = '%2'").arg(LEPasswordUpdate,zhanghao));
    QMessageBox::information(this,"成功",QString("修改用户 %1 密码成功").arg(zhanghao));

    on_pushButton_4_Manager_Operation_Cancel_clicked();
    on_pushButton_Login_clicked();
}

void MainWindow::on_pushButton_4_Manager_Delete_clicked()
{
    //点击删除此账号唤醒操作框//
    if(ui->pushButton_4_Manager_Delete->text() == "删除此账号")
    {
        ui->pushButton_4_Manager_Delete->setText("确认删除");
        ui->pushButton_4_Manager_Update->hide();
        ui->lineEdit_4_Manager_Operation_Password_Update->hide();
        ui->lineEdit_4_Manager_Operation_Password_Update_2->hide();
        ui->groupBox_4_Manager_Operation->show();
        return;
    }

    QSqlQuery query;
    QString LEPassword = ui->lineEdit_4_Manager_Operation_Password->text();
    //检查输入信息是否存在问题//
    if(LEPassword.length() == 0)
    {
        QMessageBox::information(this,"错误","存在信息未填\n删除失败");
        return;
    }
    //判断密码是否错误//
    query.exec(QString("select * from manager where Username = '%1' and Password = '%2'").arg(zhanghao,LEPassword));
    if(!query.next())
    {
        QMessageBox::information(this,"错误","密码错误");
        return;
    }
    //判断是否为只有一位管理员//
    query.exec(QString("select * from manager where Username <> '%1'").arg(zhanghao));
    if(!query.next())
    {
        QMessageBox::information(this,"错误","不能删除最后一位管理员");
        return;
    }
    //删除用户//
    query.exec(QString("delete from manager where Username = '%1'").arg(zhanghao));
    QMessageBox::information(this,"成功",QString("删除用户 %1 信息成功").arg(zhanghao));

    on_pushButton_4_Manager_Operation_Cancel_clicked();
    on_pushButton_Login_clicked();
}

void MainWindow::on_pushButton_4_Manager_Operation_Cancel_clicked()
{
    ui->pushButton_4_Manager_Update->setText("修改密码");
    ui->pushButton_4_Manager_Delete->setText("删除此账号");
    ui->pushButton_4_Manager_Update->show();
    ui->pushButton_4_Manager_Delete->show();
    ui->lineEdit_4_Manager_Operation_Password_Update->show();
    ui->lineEdit_4_Manager_Operation_Password_Update_2->show();
    ui->groupBox_4_Manager_Operation->hide();
}

void MainWindow::on_pushButton_4_Data_Backup_clicked()
{
    QSqlQuery query;
    QFileDialog QFD;
    //选择保存路径//
    QString BackupName = QFD.getSaveFileName(this,"文件保存","C:\\","*.bak");
    //判断是否取消//
    if(BackupName.length() == 0)
    {
        return;
    }
    //备份数据库//
    if(!query.exec("backup database Library to disk = '" + BackupName + "'"))
    {
        QMessageBox::information(this,"错误",QString("数据库错误\n%1").arg(query.lastError().text()));
    }
    else
    {
        QMessageBox::information(this,"成功","备份成功");
    }
}

void MainWindow::on_pushButton_4_Data_Restore_clicked()
{
    QSqlQuery query;
    QFileDialog QFD;
    //选择打开路径//
    QString BackupName = QFD.getOpenFileName(this,"文件打开","C:\\","*.bak");
    //判断是否取消//
    if(BackupName.length() == 0)
    {
        return;
    }
    //还原数据库//
    if(!query.exec("use master restore database Library from disk = '" + BackupName + "' WITH REPLACE"))
    {
        QMessageBox::information(this,"错误",QString("数据库错误\n%1").arg(query.lastError().text()));
    }
    else
    {
        QMessageBox::information(this,"成功","恢复成功 即将重启");
        //重启//
        qApp->exit(888);
    }
}

void MainWindow::on_pushButton_4_Software_About_clicked()
{
    QMessageBox::about(this, "关于", "<font color='red'>______V0.6-190617______</font>");
}

void MainWindow::on_pushButton_Statistics_clicked()
{
    if(ui->pushButton_Statistics->text() == "书籍数据统计图")
    {
        ui->tabWidget->hide();
        ui->tabWidget->setStyleSheet("QTabBar::tab:disabled {width: 0; color: transparent;}");
        ui->label_Statistics_Background->show();
        ui->pushButton_Statistics->setText("关闭数据统计图");
        tu = new statistics(this);
        tu->SetData(ReadData());
        tu->show();
    }
    else
    {
        ui->tabWidget->show();
        ui->tabWidget->setStyleSheet("QTabBar::tab:disabled {width: 0; color: transparent;}");
        ui->label_Statistics_Background->hide();
        ui->pushButton_Statistics->setText("书籍数据统计图");
        tu->close();
    }
}

QByteArray MainWindow::ReadData()   //串行化
{
    QSqlQuery query;


    QByteArray block; //用于暂存我们要发送的数据
    QDataStream out(&block,QIODevice::WriteOnly);     //使用数据流写入数据
    out.setVersion(QDataStream::Qt_5_12);      //设置数据流的版本，客户端和服务器端使用的版本要相同
    out<<(quint16) 0;
    QStringList lis1;
    QList<double> lis2;
    lis1<<"军事"<<"社会科学总论"<<"天文学、地理科学"<<"艺术"<<"语言、文字"<<"计算机科学"<<"综合性图书"<<"哲学"<<"政治、法律";

    query.exec(QString("select sum(Number) from book where BookNumber like '%%1%'").arg("js"));
    query.seek(0);
    lis2<<query.value(0).toDouble();
    query.exec(QString("select sum(Number) from book where BookNumber like '%%1%'").arg("sh"));
    query.seek(0);
    lis2<<query.value(0).toDouble();
    query.exec(QString("select sum(Number) from book where BookNumber like '%%1%'").arg("tw"));
    query.seek(0);
    lis2<<query.value(0).toDouble();
    query.exec(QString("select sum(Number) from book where BookNumber like '%%1%'").arg("ys"));
    query.seek(0);
    lis2<<query.value(0).toDouble();
    query.exec(QString("select sum(Number) from book where BookNumber like '%%1%'").arg("yx"));
    query.seek(0);
    lis2<<query.value(0).toDouble();
    query.exec(QString("select sum(Number) from book where BookNumber like '%%1%'").arg("yy"));
    query.seek(0);
    lis2<<query.value(0).toDouble();
    query.exec(QString("select sum(Number) from book where BookNumber like '%%1%'").arg("zh"));
    query.seek(0);
    lis2<<query.value(0).toDouble();
    query.exec(QString("select sum(Number) from book where BookNumber like '%%1%'").arg("zx"));
    query.seek(0);
    lis2<<query.value(0).toDouble();
    query.exec(QString("select sum(Number) from book where BookNumber like '%%1%'").arg("zz"));
    query.seek(0);
    lis2<<query.value(0).toDouble();

    QString number,total;


    query.exec(QString("select sum(Number) from book"));
    query.seek(0);
    number=query.value(0).toString();
    query.exec(QString("select sum(Total) from book"));
    query.seek(0);
    total=query.value(0).toString();
    tu->SetNumberTotal(number,total);

    for(int i=0;i<lis1.size();i++)
    {
        out<<lis1.at(i)<<lis2.at(i);
    }
    out.device()->seek(0);
    out<<(quint16) (block.size() - sizeof(quint16));
    return block;
}
