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
    void onSaveData();
    void onLoadData();
    void onSaveDataXML();
    void onLoadDataXML();
    void onSaveDataJSON();
    void onLoadDataJSON();
    void onCellClicked(const QModelIndex &index);
    void onBackToLogin();
private:
    void updateUI();
    QList<ClassInfo> _classes;
    void setupToolbar();
    void displayClasses();
    void displayMembers(const ClassInfo& classInfo);
    QTableView* _tableView;
};
#endif // MAINWINDOW_H
