#include "widget.h"

#include "DupCheck.h"

#include<QApplication>

#include<QLabel>

#include<QPushButton>

#include<QComboBox>

#include<QTextEdit>

#include<QGroupBox>

#include<QTabWidget>

#include<QVBoxLayout>

#include<QHBoxLayout>

#include<QFileDialog>

void MyWidget::edit_change_1()
{
    edit1->setText("<table border = 1><tr><th>code1</th></tr>"

                  "<tr><td>"

                  +fileName1+

                  "</td></tr></table>");

    edit_res->setTextColor("green");

    edit_res->setText("0.0%");
}
void MyWidget::edit_change_2()
{
    edit2->setText("<table border = 1><tr><th>code2</th></tr>"

                  "<tr><td>"

                  +fileName2+

                  "</td></tr></table>");
}

void MyWidget::on_pushButton_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this,

                                                        tr("Adding Files to check..."),

                                                        "D:\\",

                                                        tr("程序文件(*cpp *c *java *py);;"

                                                       "本本文件(*txt)"));

    this->fileName1 = fileNames[0];
    this->fileName2 = fileNames[1];

    connect(this, SIGNAL(file_choose()), this, SLOT(edit_change_1()));
    connect(this, SIGNAL(file_choose()), this, SLOT(edit_change_2()));

    emit file_choose();
}

void MyWidget::duplicate_check()
{
    DupCheck S = DupCheck(fileName1.toStdString(), fileName2.toStdString());

    vector <FRE> keyVector1, keyVector2;

    double similarity = S.duplicate_checking(keyVector1, keyVector2);

    QString ans = QString::number(similarity * 100);

    if (similarity > 0.8)
        edit_res->setTextColor("red");

    else if(similarity > 0.5)
        edit_res->setTextColor("orange");

    else
        edit_res->setTextColor("green");

    edit_res->setText(ans + "%");

    for (auto i : keyVector1)
    {
        edit1->append(QString::fromStdString(i.word) + "              " + QString::number(i.fre, 10, 6));
    }

    for (auto i : keyVector2)
    {
        edit2->append(QString::fromStdString(i.word) + "              " + QString::number(i.fre, 10, 6));
    }

}

//MyWidget类的构造函数
MyWidget::MyWidget(QWidget *parent) : QWidget(parent)

{
    //初始化成员变量的值
    fileName1 = "";

    fileName2 = "";

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label;          //单独显示一行文字

    QPushButton *button;    //添加文件的按钮

    QPushButton *start_button;      //开始查重的按钮

    QPushButton *close_button;      //程序关闭的按钮

    QGroupBox *groupbox1;       //第一个组件群，包括显示为“menu...”的label,start_button和提示按钮作用的label

    QGroupBox *groupbox2;       //第一个组件群，包括显示为“result”的label,close_button和res_edit



    //第一个按钮，用于添加文件
    layout->addWidget(button = new QPushButton("Add files here"));

    connect(button, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));


    //第一行label，用于提示以下窗口显示词频统计结果
    layout->addWidget(label = new QLabel("word frequency statistics"));



    //第一个窗口，显示文件1的词频统计结果
    layout->addWidget(edit1 = new QTextEdit);


    //初始化成员变量edit1
    edit1->setText("<table border = 1><tr><th>code1</th></tr>"

                  "<tr><td>"

                  +fileName1+

                  "</td></tr></table>");

    //竖着的滚动条当需要时有，不需要时就没有
    edit1->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    //横着的滚动条设置为没有
    edit1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    //第二个窗口，显示文件2的词频统计结果
    layout->addWidget(edit2 = new QTextEdit);

    //初始化成员变量edit1
    edit2->setText("<table border = 1><tr><th>code2</th></tr>"

                  "<tr><td>"

                  +fileName2+

                  "</td></tr></table>");

    //竖着的滚动条当需要时有，不需要时就没有
    edit2->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    //横着的滚动条设置为没有
    edit2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);



    //第一个组件群
    layout->addWidget(groupbox1 = new QGroupBox("menu..."));

    QHBoxLayout * hboxlay1;

    groupbox1->setLayout(hboxlay1 = new QHBoxLayout);

    //设置start_button
    hboxlay1->addWidget(start_button = new QPushButton("Start"));

    //设置提示label
    hboxlay1->addWidget(new QLabel("Push it to start duplicating check"));


    //第二个组件群
    layout->addWidget(groupbox2 = new QGroupBox("result..."));

    QHBoxLayout * hboxlay2;

    groupbox2->setLayout(hboxlay2 = new QHBoxLayout);

    //初始化res_edit
    hboxlay2->addWidget(edit_res = new QTextEdit());

    //初始化res_edit的状态
    edit_res->setTextColor("green");

    edit_res->setText("0.0%");

    //竖着的滚动条设置为没有
    edit_res->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //横着的滚动条设置为没有
    edit_res->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //连接start_button和duplicate_check
    connect(start_button, SIGNAL(clicked()), this, SLOT(duplicate_check()));

    //添加关闭按钮
    layout->addWidget(close_button = new QPushButton("Close"));

    //连接close_button和close
    connect(close_button, SIGNAL(clicked()), this, SLOT(close()));

}

//访问私有成员变量
QString MyWidget::get_file_name1()
{
    return this->fileName1;
}

QString MyWidget::get_file_name2()
{
    return  this->fileName2;
}
