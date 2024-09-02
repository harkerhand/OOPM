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
    SearchDialog(const QList<ClassInfo> &classes, QWidget *parent = nullptr);
    ~SearchDialog() {}

signals:
    void searchCompleted(const QList<ClassInfo> &filteredClasses);
    void cancelSearch();

private slots:
    void onSearch();
    void onCancel();
    void onDateTimeChange();

private:
    QLineEdit *_nameEdit;
    QLineEdit *_authorEdit;
    QComboBox *_dateRangeComboBox;
    QDateTimeEdit *_startDateEdit;
    QDateTimeEdit *_endDateEdit;
    QPushButton *_searchButton;
    QPushButton *_cancelButton;
    QList<ClassInfo> _classes; // 存储原始的类信息列表
};

#endif // SEARCHDIALOG_H
