#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QList>
#include "ClassInfo.h"

class SearchDialog : public QDialog {
    Q_OBJECT

public:
    SearchDialog(const QList<ClassInfo> &classes, QWidget *parent = nullptr); // 构造函数，初始化搜索对话框并传入类信息列表

signals:
    void searchCompleted(const QList<ClassInfo> &filteredClasses); // 发出搜索完成信号，并传递过滤后的类信息列表
    void cancelSearch(); // 发出取消搜索的信号

private slots:
    void onSearch(); // 处理搜索操作
    void onCancel(); // 处理取消操作
    void onDateTimeChange(); // 处理日期范围变化事件

private:
    QLineEdit *_idEdit; // 用于输入类id的编辑框
    QLineEdit *_nameEdit; // 用于输入类名称的编辑框
    QLineEdit *_authorEdit; // 用于输入作者名称的编辑框
    QComboBox *_dateRangeComboBox; // 用于选择日期范围的下拉框
    QDateTimeEdit *_startDateEdit; // 用于选择开始日期的日期时间编辑框
    QDateTimeEdit *_endDateEdit; // 用于选择结束日期的日期时间编辑框
    QPushButton *_searchButton; // 触发搜索操作的按钮
    QPushButton *_cancelButton; // 触发取消操作的按钮
    QList<ClassInfo> _classes; // 存储原始的类信息列表
};

#endif // SEARCHDIALOG_H
