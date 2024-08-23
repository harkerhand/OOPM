#include "MainWindow.h"
#include "ClassModel.h"
#include "SimpleTextDialog.h"
#include <QTableView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QDataStream>
#include <QFile>
#include <QAction>
#include <QToolBar>
#include "CreateClassDialog.h"
#include "MemberModel.h"
#include "MembersDetailDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _classes() {
    // 界面设置
    QToolBar *toolbar = addToolBar("File");
    QAction *loadAction = new QAction("Load Data", this);
    loadAction->setShortcut(QKeySequence::Open);
    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoadData);
    toolbar->addAction(loadAction);
    QAction *saveAction = new QAction("Save Data", this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveData);
    toolbar->addAction(saveAction);
    QAction *addAction = new QAction("Add Class", this);
    addAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    connect(addAction, &QAction::triggered, this, &MainWindow::onAddClass);
    toolbar->addAction(addAction);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    _tableView = new QTableView(this);
    layout->addWidget(_tableView);

    setCentralWidget(centralWidget);
    resize(960, 600);
    displayClasses();

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
    _tableView->clearSpans();
    ClassModel *model = new ClassModel(_classes, this);
    _tableView->setModel(model);

}




void MainWindow::onCellClicked(const QModelIndex &index) {
    if (!index.isValid())
        return;

    int column = index.column();
    int row = index.row();
    if(column == 0){
        QMessageBox msgBox;
        msgBox.setText("你想要进行什么操作？");
        QAbstractButton* modifyButton = msgBox.addButton("修改", QMessageBox::YesRole);
        QAbstractButton* deleteButton = msgBox.addButton("删除", QMessageBox::NoRole);
        msgBox.addButton("取消", QMessageBox::RejectRole);
        msgBox.exec();

        if(msgBox.clickedButton() == deleteButton) {
            _classes.removeAt(row);
            displayClasses();
        } else if(msgBox.clickedButton() == modifyButton) {
            CreateClassDialog dialog(this, _classes.at(row));
            if (dialog.exec() == QDialog::Accepted) {
                ClassInfo newClass = dialog.getClassInfo();
                _classes[row] = newClass;
                QMessageBox::information(this, "Class Created", QString("Class %1 modified successfully!").arg(newClass.name()));
                displayClasses();
            }
        }
    } else if(column == 6) {
        QList<ClassMember> tempMembers = _classes.at(row).members();
        MembersDetailDialog detailsDialog(tempMembers, this);
        detailsDialog.exec();
        _classes[row].setMembers(detailsDialog.getMembers());
        displayClasses();
    } else {
        QVariant data = _tableView->model()->data(index);
        QString cellText = data.toString();

        // Show the data in a message box
        SimpleTextDialog(cellText).exec();
    }
}


