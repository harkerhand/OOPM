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
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTranslator>
#include <QApplication>
#include <QToolButton>
#include <QMenu>
#include "LoginWindow.h"
#include "SearchDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _classes() {
    setupToolbar();

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    _tableView = new QTableView(this);

    //自动调整
    _tableView->horizontalHeader()->setStretchLastSection(true);
    _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    _tableView->setStyleSheet(
            "QTableView {"
            "    background-image: url(:/imgs/bgtext.png);"
            "    background-repeat: no-repeat;"
            "    background-position: center;"
            "}"
        );
    layout->addWidget(_tableView);

    setCentralWidget(centralWidget);
    setMinimumSize(1200, 720);
    displayClasses();

    connect(_tableView, &QTableView::clicked, this, &MainWindow::onCellClicked);
    _classes = {};
}

void MainWindow::setupToolbar() {
    QToolBar *toolbar = addToolBar("File");
    toolbar->setFixedHeight(50);

    QAction *loadDataAction = new QAction(tr("Load Data"), this);
    QAction *saveDataAction = new QAction(tr("Save Data"), this);
    QAction *addAction = new QAction(tr("Add Class"), this);
    QAction *saveXMLAction = new QAction(tr("Save XML"), this);
    QAction *loadXMLAction = new QAction(tr("Load XML"), this);
    QAction *saveJSONAction = new QAction(tr("Save JSON"), this);
    QAction *loadJSONAction = new QAction(tr("Load JSON"), this);
    QAction *backToLogin = new QAction(tr("Back Login"), this);
    _searchButton = new QPushButton(tr("Search"), this);

    loadDataAction->setShortcut(QKeySequence::Open);
    saveDataAction->setShortcut(QKeySequence::Save);
    addAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    saveXMLAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
    loadXMLAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    saveJSONAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_J));
    loadJSONAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_K));
    backToLogin->setShortcut(QKeySequence(Qt::Key_Escape));
    _searchButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));

    QToolButton *dataButton = new QToolButton(this);
    dataButton->setText(tr("Data"));
    dataButton->setPopupMode(QToolButton::InstantPopup);
    QMenu *dataMenu = new QMenu(dataButton);
    dataButton->setMenu(dataMenu);
    dataMenu->addAction(loadDataAction);
    dataMenu->addAction(saveDataAction);

    QToolButton *xmlButton = new QToolButton(this);
    xmlButton->setText(tr("XML"));
    xmlButton->setPopupMode(QToolButton::InstantPopup);
    QMenu *xmlMenu = new QMenu(xmlButton);
    xmlButton->setMenu(xmlMenu);
    xmlMenu->addAction(loadXMLAction);
    xmlMenu->addAction(saveXMLAction);

    QToolButton *jsonButton = new QToolButton(this);
    jsonButton->setText(tr("JSON"));
    jsonButton->setPopupMode(QToolButton::InstantPopup);
    QMenu *jsonMenu = new QMenu(jsonButton);
    jsonButton->setMenu(jsonMenu);
    jsonMenu->addAction(loadJSONAction);
    jsonMenu->addAction(saveJSONAction);


    toolbar->addWidget(dataButton);
    toolbar->addWidget(xmlButton);
    toolbar->addWidget(jsonButton);
    toolbar->addAction(addAction);
    toolbar->addAction(backToLogin);
    toolbar->addWidget(_searchButton);


    connect(loadDataAction, &QAction::triggered, this, &MainWindow::onLoadData);
    connect(saveDataAction, &QAction::triggered, this, &MainWindow::onSaveData);
    connect(addAction, &QAction::triggered, this, &MainWindow::onAddClass);
    connect(saveXMLAction, &QAction::triggered, this, &MainWindow::onSaveDataXML);
    connect(loadXMLAction, &QAction::triggered, this, &MainWindow::onLoadDataXML);
    connect(saveJSONAction, &QAction::triggered, this, &MainWindow::onSaveDataJSON);
    connect(loadJSONAction, &QAction::triggered, this, &MainWindow::onLoadDataJSON);
    connect(backToLogin, &QAction::triggered, this, &MainWindow::onBackToLogin);
    connect(_searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
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
    _tableView->clearSpans();
    if(_searchMode) {
        ClassModel *model = new ClassModel(_classes_T, this);
        _tableView->setModel(model);
    } else {
        ClassModel *model = new ClassModel(_classes, this);
        _tableView->setModel(model);
    }

}

bool MainWindow::removeById(const int id) {
    if(_searchMode) {
        for(int i = 0; i < _classes_T.size(); i++) {
            if(_classes_T.at(i).id() == id) {
                _classes_T.removeAt(i);
            }
        }
    }
    for(int i = 0; i < _classes.size(); i++) {
        if(_classes.at(i).id() == id) {
            _classes.removeAt(i);
            return true;
        }
    }
    return false;
}

ClassInfo& MainWindow::findClassById(QList<ClassInfo>& classes, const int id) {
    for(int i = 0; i < classes.size(); i++) {
        if(classes.at(i).id() == id) {
            return classes[i];
        }
    }
    return classes[0];
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
        QVariant data = _tableView->model()->data(index);
        int tempId = data.toInt();
        if(msgBox.clickedButton() == deleteButton) {
            removeById(tempId);
            QMessageBox::information(this, tr("Class Deleted"), QString(tr("Class %1 deleted successfully!")).arg(findClassById(_classes, tempId).name()));
            displayClasses();
        } else if(msgBox.clickedButton() == modifyButton) {
            ClassInfo &tClass = findClassById(_classes ,tempId);
            CreateClassDialog dialog(this, _classes, tClass, true);
            if (dialog.exec() == QDialog::Accepted) {
                ClassInfo newClass = dialog.getClassInfo();
                tClass = newClass;
                if(_searchMode) {
                    findClassById(_classes_T ,tempId) = newClass;
                }
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

void MainWindow::onSaveDataXML() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Data as XML"), "", "XML Files (*.xml)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Error"), tr("Could not save data."));
        return;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("Classes");

    for (const ClassInfo &classInfo : _classes) {
        xmlWriter.writeStartElement("Class");
        xmlWriter.writeTextElement("ID", QString::number(classInfo.id()));
        xmlWriter.writeTextElement("Name", classInfo.name());
        xmlWriter.writeTextElement("BaseClassName", classInfo.baseClassName());
        xmlWriter.writeTextElement("Function", classInfo.function());
        xmlWriter.writeTextElement("CreationDate", classInfo.creationDate().toString(Qt::ISODate));
        xmlWriter.writeTextElement("Author", classInfo.author());

        xmlWriter.writeStartElement("Members");
        for (const ClassMember &member : classInfo.members()) {
            xmlWriter.writeStartElement("Member");
            xmlWriter.writeTextElement("MemberID", QString::number(member.memberId()));
            xmlWriter.writeTextElement("MemberName", member.memberName());
            xmlWriter.writeTextElement("MemberType", memberTypeToString(member.memberType()));
            xmlWriter.writeTextElement("MemorySize", QString::number(member.memorySize()));
            xmlWriter.writeTextElement("DataType", dataTypeToString(member.dataType()));
            xmlWriter.writeTextElement("Accessibility", accessibilityToString(member.accessibility()));
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    file.close();
    QMessageBox::information(this, tr("Success"), tr("Data saved successfully."));
}

void MainWindow::onLoadDataXML() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Data from XML"), "", "XML Files (*.xml)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Error"), tr("Could not load data."));
        return;
    }

    QXmlStreamReader xmlReader(&file);
    _classes.clear();

    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if (token == QXmlStreamReader::StartElement) {
            if (xmlReader.name() == "Class") {
                ClassInfo classInfo;
                while (!(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "Class")) {
                    if (xmlReader.tokenType() == QXmlStreamReader::StartElement) {
                        if (xmlReader.name() == "ID") {
                            classInfo.setId(xmlReader.readElementText().toInt());
                        } else if (xmlReader.name() == "Name") {
                            classInfo.setName(xmlReader.readElementText());
                        } else if (xmlReader.name() == "BaseClassName") {
                            classInfo.setBaseClassName(xmlReader.readElementText());
                        } else if (xmlReader.name() == "Function") {
                            classInfo.setFunction(xmlReader.readElementText());
                        } else if (xmlReader.name() == "CreationDate") {
                            classInfo.setCreationDate(QDateTime::fromString(xmlReader.readElementText(), Qt::ISODate));
                        } else if (xmlReader.name() == "Author") {
                            classInfo.setAuthor(xmlReader.readElementText());
                        } else if (xmlReader.name() == "Members") {
                            while (!(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "Members")) {
                                if (xmlReader.tokenType() == QXmlStreamReader::StartElement && xmlReader.name() == "Member") {
                                    ClassMember member;
                                    while (!(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "Member")) {
                                        if (xmlReader.tokenType() == QXmlStreamReader::StartElement) {
                                            if (xmlReader.name() == "MemberID") {
                                                member.setMemberId(xmlReader.readElementText().toInt());
                                            } else if (xmlReader.name() == "MemberName") {
                                                member.setMemberName(xmlReader.readElementText());
                                            } else if (xmlReader.name() == "MemberType") {
                                                QString type = xmlReader.readElementText();
                                                member.setMemberType(
                                                    type == "Data" ? MemberType::Data : MemberType::Function
                                                );
                                            } else if (xmlReader.name() == "MemorySize") {
                                                member.setMemorySize(xmlReader.readElementText().toInt());
                                            } else if (xmlReader.name() == "DataType") {
                                                QString type = xmlReader.readElementText();
                                                member.setDataType(
                                                    type == "Int" ? DataType::Int :
                                                    type == "Float" ? DataType::Float :
                                                    type == "Double" ? DataType::Double :
                                                    type == "Char" ? DataType::Char :
                                                    type == "String" ? DataType::String :
                                                    DataType::Custom
                                                );
                                            } else if (xmlReader.name() == "Accessibility") {
                                                QString access = xmlReader.readElementText();
                                                member.setAccessibility(
                                                    access == "Public" ? Accessibility::Public :
                                                    access == "Private" ? Accessibility::Private :
                                                    Accessibility::Protected
                                                );
                                            }
                                        }
                                        xmlReader.readNext();
                                    }
                                    classInfo.addMember(member);
                                }
                                xmlReader.readNext();
                            }
                        }
                    }
                    xmlReader.readNext();
                }
                _classes.append(classInfo);
            }
        }
    }

    if (xmlReader.hasError()) {
        QMessageBox::critical(this, tr("Error"), tr("XML error: %1").arg(xmlReader.errorString()));
    }

    file.close();
    displayClasses();
    QMessageBox::information(this, tr("Success"), tr("Data loaded successfully."));
}

void MainWindow::onSaveDataJSON() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Data as JSON"), "", "JSON Files (*.json)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Error"), tr("Could not save data."));
        return;
    }

    QJsonArray classArray;
    for (const ClassInfo &classInfo : _classes) {
        QJsonObject classObject;
        classObject["ID"] = classInfo.id();
        classObject["Name"] = classInfo.name();
        classObject["BaseClassName"] = classInfo.baseClassName();
        classObject["Function"] = classInfo.function();
        classObject["CreationDate"] = classInfo.creationDate().toString(Qt::ISODate);
        classObject["Author"] = classInfo.author();

        QJsonArray membersArray;
        for (const ClassMember &member : classInfo.members()) {
            QJsonObject memberObject;
            memberObject["MemberID"] = member.memberId();
            memberObject["MemberName"] = member.memberName();
            memberObject["MemberType"] = memberTypeToString(member.memberType());
            memberObject["MemorySize"] = member.memorySize();
            memberObject["DataType"] = dataTypeToString(member.dataType());
            memberObject["Accessibility"] = accessibilityToString(member.accessibility());
            membersArray.append(memberObject);
        }
        classObject["Members"] = membersArray;
        classArray.append(classObject);
    }

    QJsonDocument jsonDoc(classArray);
    file.write(jsonDoc.toJson());

    file.close();
    QMessageBox::information(this, tr("Success"), tr("Data saved successfully."));
}

void MainWindow::onLoadDataJSON() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Data from JSON"), "", "JSON Files (*.json)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Error"), tr("Could not load data."));
        return;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));
    QJsonArray classArray = jsonDoc.array();

    _classes.clear();
    for (const QJsonValue &value : classArray) {
        QJsonObject classObject = value.toObject();
        ClassInfo classInfo;
        classInfo.setId(classObject["ID"].toInt());
        classInfo.setName(classObject["Name"].toString());
        classInfo.setBaseClassName(classObject["BaseClassName"].toString());
        classInfo.setFunction(classObject["Function"].toString());
        classInfo.setCreationDate(QDateTime::fromString(classObject["CreationDate"].toString(), Qt::ISODate));
        classInfo.setAuthor(classObject["Author"].toString());

        QJsonArray membersArray = classObject["Members"].toArray();
        for (const QJsonValue &memberValue : membersArray) {
            QJsonObject memberObject = memberValue.toObject();
            ClassMember member;
            member.setMemberId(memberObject["MemberID"].toInt());
            member.setMemberName(memberObject["MemberName"].toString());
            QString type = memberObject["MemberType"].toString();
            member.setMemberType(
                type == "Data" ? MemberType::Data : MemberType::Function
            );
            member.setMemorySize(memberObject["MemorySize"].toInt());
            member.setDataType(
                memberObject["DataType"].toString() == "Int" ? DataType::Int :
                memberObject["DataType"].toString() == "Float" ? DataType::Float :
                memberObject["DataType"].toString() == "Double" ? DataType::Double :
                memberObject["DataType"].toString() == "Char" ? DataType::Char :
                memberObject["DataType"].toString() == "String" ? DataType::String :
                DataType::Custom
            );
            QString access = memberObject["Accessibility"].toString();
            member.setAccessibility(
                access == "Public" ? Accessibility::Public :
                access == "Private" ? Accessibility::Private :
                Accessibility::Protected
            );
            classInfo.addMember(member);
        }
        _classes.append(classInfo);
    }

    file.close();
    displayClasses();
    QMessageBox::information(this, tr("Success"), tr("Data loaded successfully."));
}

void MainWindow::onBackToLogin() {
    LoginWindow *login = new LoginWindow();
    login->show();
    this->close();
}

void MainWindow::onSearchClicked() {
    SearchDialog dialog(_classes, this);
    connect(&dialog, &SearchDialog::searchCompleted, this, &MainWindow::handleSearchCompleted);
    connect(&dialog, &SearchDialog::cancelSearch, this, &MainWindow::onCancelSearch);
    dialog.exec();
}

void MainWindow::handleSearchCompleted(const QList<ClassInfo> &filteredClasses) {
    _classes_T = filteredClasses; // 更新类列表
    _searchMode = true;
    _searchButton->setStyleSheet("color: red;");
    _searchButton->setText(tr("!Search state!"));
    displayClasses(); // 重新显示更新后的列表
}

void MainWindow::onCancelSearch() {
    // 处理取消查询的逻辑
    _classes_T.clear();
    _searchMode = false;
    _searchButton->setStyleSheet("");
    _searchButton->setText(tr("Search"));
    displayClasses();
}
