
#ifndef MYWIDGET_H

#define MYWIDGET_H



#include <QWidget>

#include<QTextEdit>

#include <QTextEdit>


class MyWidget : public QWidget

{
    QString fileName1;      //文件名1
    QString fileName2;      //文件名2
    QTextEdit *edit1;       //代码1词频统计结果显示窗口
    QTextEdit *edit2;       //代码2词频统计结果显示窗口
    QTextEdit *edit_res;    //查重结果显示窗口
    Q_OBJECT

public:

    explicit MyWidget(QWidget *parent = 0);

    QTextEdit* myedit;

    QString get_file_name1();   //获取私有变量fileName1
    QString get_file_name2();   //获取私有变量fileName2

signals:

    //自定义信号，当文件选择完毕后发送次信号
    void file_choose();

public slots:

    //自定义槽

    void on_pushButton_clicked();   //Adding files here按钮按下后触发的槽
    void edit_change_1();       //信号file_choose发出后触发的槽，改变查重结果和词频统计结果1
    void edit_change_2();       //信号file_choose发出后触发的槽，改变查重结果和词频统计结果2
    void duplicate_check();     //Start按钮按下后触发的槽，调用查重程序

};



#endif // MYWIDGET_H
