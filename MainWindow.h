#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTableView>
#include "ClassInfo.h"
#include "Enums.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow(){};

private slots:
    void onAddClass();
    void onDeleteClass();
    void onSaveData();
    void onLoadData();
    void onCellClicked(const QModelIndex &index);

private:
    QList<ClassInfo> _classes;
    void displayClasses();
    void displayMembers(const ClassInfo& classInfo);
    QTableView* _tableView;
};
#endif // MAINWINDOW_H
