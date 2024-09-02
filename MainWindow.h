#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTableView>
#include <QPushButton>
#include "ClassInfo.h"
#include "Enums.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow(){};
    bool removeById(const int id);
    ClassInfo& findClassById(QList<ClassInfo>& classes, const int id);

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
    void onSearchClicked();
    void handleSearchCompleted(const QList<ClassInfo> &filteredClasses);
    void onCancelSearch();
private:
    void updateUI();
    QList<ClassInfo> _classes;
    QList<ClassInfo> _classes_T;
    void setupToolbar();
    void displayClasses();
    void displayMembers(const ClassInfo& classInfo);
    QTableView* _tableView;
    bool _searchMode = false;
    QPushButton *_searchButton;
};
#endif // MAINWINDOW_H
