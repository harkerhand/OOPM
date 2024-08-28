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
#include <QHeaderView>
#include "CreateClassDialog.h"
#include "MemberModel.h"
#include "MembersDetailDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _classes() {
    // 界面设置
    QToolBar *toolbar = addToolBar("File");
    toolbar->setFixedHeight(50);
    QAction *loadAction = new QAction(tr("Load Data"), this);
    QAction *saveAction = new QAction(tr("Save Data"), this);
    QAction *addAction = new QAction(tr("Add Class"), this);

    loadAction->setShortcut(QKeySequence::Open);
    saveAction->setShortcut(QKeySequence::Save);
    addAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    toolbar->addAction(loadAction);
    toolbar->addAction(saveAction);
    toolbar->addAction(addAction);

    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoadData);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveData);
    connect(addAction, &QAction::triggered, this, &MainWindow::onAddClass);



    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    _tableView = new QTableView(this);

    //自动调整
    _tableView->horizontalHeader()->setStretchLastSection(true);
    _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    layout->addWidget(_tableView);

    setCentralWidget(centralWidget);
    setMinimumSize(1080, 720);
    displayClasses();

    connect(_tableView, &QTableView::clicked, this, &MainWindow::onCellClicked);

}

void MainWindow::onAddClass() {
    // 示例：添加一个类
    CreateClassDialog dialog(this, _classes);
    if (dialog.exec() == QDialog::Accepted) {
        ClassInfo newClass = dialog.getClassInfo();
        _classes.append(newClass);
        QMessageBox::information(this, tr("Class Created"), QString(tr("Class %1 created successfully!")).arg(newClass.name()));
    }
    displayClasses();
}



void MainWindow::onSaveData() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Data"), "", "Data Files (*.dat)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Error"), tr("Could not save data."));
        return;
    }

    QDataStream out(&file);
    out << _classes.size();
    for (const ClassInfo &classInfo : _classes) {
        classInfo.saveToStream(out);
    }

    file.close();
    QMessageBox::information(this, tr("Success"), tr("Data saved successfully."));
}

void MainWindow::onLoadData() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Data"), "", "Data Files (*.dat)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Error"), tr("Could not load data."));
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
    QMessageBox::information(this, tr("Success"), tr("Data loaded successfully."));
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
        msgBox.setText(tr("What do you want to do?"));
        QAbstractButton* modifyButton = msgBox.addButton(tr("Modify"), QMessageBox::YesRole);
        QAbstractButton* deleteButton = msgBox.addButton(tr("Delete"), QMessageBox::NoRole);
        msgBox.addButton(tr("Cancel"), QMessageBox::RejectRole);
        msgBox.exec();

        if(msgBox.clickedButton() == deleteButton) {
            _classes.removeAt(row);
            displayClasses();
        } else if(msgBox.clickedButton() == modifyButton) {
            CreateClassDialog dialog(this, _classes, _classes.at(row), true);
            if (dialog.exec() == QDialog::Accepted) {
                ClassInfo newClass = dialog.getClassInfo();
                _classes[row] = newClass;
                QMessageBox::information(this, tr("Class Created"), QString(tr("Class %1 modified successfully!")).arg(newClass.name()));
                displayClasses();
            }
        }
    } else if(column == 6) {
        QList<ClassMember> tempMembers = _classes.at(row).members();
        MembersDetailDialog detailsDialog(tempMembers, this);
        detailsDialog.exec();
        _classes[row].setMembers(detailsDialog.getMembers());
        displayClasses();
    } else if(column == 4) {
        QString dateTime = _classes.at(row).creationDate().toString("yyyy-MM-dd HH:mm ddd");
        SimpleTextDialog(dateTime).exec();
    } else {
        QVariant data = _tableView->model()->data(index);
        QString cellText = data.toString();

        // Show the data in a message box
        SimpleTextDialog(cellText).exec();
    }
}


