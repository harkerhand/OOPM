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
    // 根据 ID 移除类
    // @param id 要移除的类的 ID
    // @return 如果成功移除返回 true，否则返回 false
    bool removeById(const int id);
    // 根据 ID 查找类
    // @param classes 要搜索的 ClassInfo 对象列表
    // @param id 要查找的类的 ID
    // @return 查找到的 ClassInfo 对象的引用
    ClassInfo& findClassById(QList<ClassInfo>& classes, const int id);
private slots:
    void onAddClass(); // 处理添加新类的操作
    void onSaveData(); // 保存类数据为 Dat 格式
    void onLoadData(); // 加载类数据为 Dat 格式
    void onSaveDataXML();// 保存类数据为 XML 格式
    void onLoadDataXML(); // 从 XML 文件加载类数据
    void onSaveDataJSON(); // 保存类数据为 JSON 格式
    void onLoadDataJSON(); // 从 JSON 文件加载类数据
    // 处理表格单元格点击事件
    // @param index 被点击单元格的索引
    void onCellClicked(const QModelIndex &index);
    void onBackToLogin(); // 返回登录窗口
    void onSearchClicked(); // 处理搜索按钮点击事件
    // 处理搜索完成事件
    // @param filteredClasses 符合搜索条件的类列表
    void handleSearchCompleted(const QList<ClassInfo> &filteredClasses);
    void onCancelSearch(); // 处理搜索取消事件
private:
    void updateUI(); // 更新 UI 元素
    QList<ClassInfo> _classes; // 管理的 ClassInfo 对象列表
    QList<ClassInfo> _classes_T; // 临时存储的 ClassInfo 对象列表，用于搜索等操作
    void setupToolbar(); // 设置工具栏
    void displayClasses(); // 显示类列表
    // 显示选定类的成员
    // @param classInfo 要显示成员的 ClassInfo 对象
    void displayMembers(const ClassInfo& classInfo);
    QTableView* _tableView; // 表格视图控件
    bool _searchMode = false; // 标识是否处于搜索模式
    QPushButton *_searchButton; // 搜索按钮
};
#endif // MAINWINDOW_H
