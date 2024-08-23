#include "MainWindow.h"
#include "ClassModel.h"
#include <QTableView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QDataStream>
#include <QFile>
#include "CreateClassDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _classes() {
    // 界面设置
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    _tableView = new QTableView(this);
    layout->addWidget(_tableView);

    QPushButton *addClassButton = new QPushButton("Add Class", this);
    layout->addWidget(addClassButton);

    QPushButton *deleteClassButton = new QPushButton("Delete Class", this);
    layout->addWidget(deleteClassButton);

    QPushButton *saveDataButton = new QPushButton("Save Data", this);
    layout->addWidget(saveDataButton);

    QPushButton *loadDataButton = new QPushButton("Load Data", this);
    layout->addWidget(loadDataButton);

    setCentralWidget(centralWidget);
    resize(1280, 720);
    displayClasses();

    // 信号与槽连接
    connect(addClassButton, &QPushButton::clicked, this, &MainWindow::onAddClass);
    connect(deleteClassButton, &QPushButton::clicked, this, &MainWindow::onDeleteClass);
    connect(saveDataButton, &QPushButton::clicked, this, &MainWindow::onSaveData);
    connect(loadDataButton, &QPushButton::clicked, this, &MainWindow::onLoadData);
    connect(_tableView, &QTableView::clicked, this, &MainWindow::onCellClicked);

}

void MainWindow::onAddClass() {
    // 示例：添加一个类
    CreateClassDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        ClassInfo newClass = dialog.getClassInfo();
        _classes.append(newClass);
        QMessageBox::information(this, "Class Created", QString("Class %1 created successfully!").arg(newClass.name()));
    }
    displayClasses();
}

void MainWindow::onDeleteClass() {
    // 示例：删除最后一个类
    if (!_classes.isEmpty()) {
        _classes.removeLast();
    }
    displayClasses();
}

void MainWindow::onSaveData() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Data", "", "Data Files (*.dat)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Error", "Could not save data.");
        return;
    }

    QDataStream out(&file);
    out << _classes.size();
    for (const ClassInfo &classInfo : _classes) {
        classInfo.saveToStream(out);
    }

    file.close();
    QMessageBox::information(this, "Success", "Data saved successfully.");
}

void MainWindow::onLoadData() {
    QString fileName = QFileDialog::getOpenFileName(this, "Load Data", "", "Data Files (*.dat)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Could not load data.");
        return;
    }

    QDataStream in(&file);
    int classCount;
    in >> classCount;

    _classes.clear();
    for (int i = 0; i < classCount; ++i) {
        ClassInfo classInfo;
        classInfo.loadFromStream(in);
        _classes.append(classInfo);
    }

    file.close();
    displayClasses();
    QMessageBox::information(this, "Success", "Data loaded successfully.");
}

void MainWindow::displayClasses() {
    // 示例：显示所有类的信息
    // 可以使用 QStandardItemModel 或其他方式展示
    ClassModel *model = new ClassModel(_classes, this);
    _tableView->setModel(model);

}

void MainWindow::displayMembers(const ClassInfo& classInfo) {
    // 示例：显示特定类的成员信息
    // 可以使用 QStandardItemModel 或其他方式展示
}



void MainWindow::onCellClicked(const QModelIndex &index) {
    if (!index.isValid())
        return;

    // Get the data from the clicked cell
    QVariant data = _tableView->model()->data(index);
    QString cellText = data.toString();

    // Show the data in a message box
    QMessageBox::information(this, "Cell Clicked", QString("Clicked cell data: %1").arg(cellText));
}


