#include "SearchDialog.h"
#include <QDateTimeEdit>
#include <QDateTime>
#include <QLabel>

SearchDialog::SearchDialog(const QList<ClassInfo> &classes, QWidget *parent)
    : QDialog(parent), _classes(classes) {
    // 移除问号
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    setWindowTitle(tr("Search Class Info"));

    _nameEdit = new QLineEdit(this);
    _authorEdit = new QLineEdit(this);
    _dateRangeComboBox = new QComboBox(this);
    _searchButton = new QPushButton(tr("Search"), this);
    _cancelButton = new QPushButton(tr("Cancel"), this);
    _searchButton->setShortcut(QKeySequence(Qt::Key_Return));
    _dateRangeComboBox->addItem(tr("Today"));
    _dateRangeComboBox->addItem(tr("Last Week"));
    _dateRangeComboBox->addItem(tr("Last Month"));
    _dateRangeComboBox->addItem(tr("Custom Range"));
    _startDateEdit = new QDateTimeEdit(this);
    _startDateEdit->setCalendarPopup(true);
    _endDateEdit = new QDateTimeEdit(this);
    _endDateEdit->setCalendarPopup(true);


    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->addWidget(new QLabel(tr("Name:")));
    nameLayout->addWidget(_nameEdit);
    mainLayout->addLayout(nameLayout);

    QHBoxLayout *authorLayout = new QHBoxLayout();
    authorLayout->addWidget(new QLabel(tr("Author:")));
    authorLayout->addWidget(_authorEdit);
    mainLayout->addLayout(authorLayout);

    QHBoxLayout *dateLayout = new QHBoxLayout();
    dateLayout->addWidget(new QLabel(tr("Date Range:")));
    dateLayout->addWidget(_dateRangeComboBox);
    mainLayout->addLayout(dateLayout);

    QHBoxLayout *startDateLayout = new QHBoxLayout();
    startDateLayout->addWidget(new QLabel(tr("Start Date:")));
    startDateLayout->addWidget(_startDateEdit);
    QHBoxLayout *endDateLayout = new QHBoxLayout();
    endDateLayout->addWidget(new QLabel(tr("End Date:")));
    endDateLayout->addWidget(_endDateEdit);
    mainLayout->addLayout(startDateLayout);
    mainLayout->addLayout(endDateLayout);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(_searchButton);
    buttonLayout->addWidget(_cancelButton);
    mainLayout->addLayout(buttonLayout);

    connect(_searchButton, &QPushButton::clicked, this, &SearchDialog::onSearch);
    connect(_cancelButton, &QPushButton::clicked, this, &SearchDialog::onCancel);
    connect(_dateRangeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SearchDialog::onDateTimeChange);
    onDateTimeChange();
}

void SearchDialog::onSearch() {
    QList<ClassInfo> filteredClasses;

    QString nameFilter = _nameEdit->text();
    QString authorFilter = _authorEdit->text();

    QDateTime startDate = _startDateEdit->dateTime();
    QDateTime endDate = _endDateEdit->dateTime();


    // 过滤逻辑
    for (const ClassInfo &classInfo : _classes) {
        bool matches = true;

        if (!nameFilter.isEmpty() && !classInfo.name().contains(nameFilter, Qt::CaseInsensitive)) {
            matches = false;
        }

        if (!authorFilter.isEmpty() && !classInfo.author().contains(authorFilter, Qt::CaseInsensitive)) {
            matches = false;
        }

        if (startDate.isValid() && endDate.isValid()) {
            if (classInfo.creationDate() < startDate || classInfo.creationDate() > endDate) {
                matches = false;
            }
        }

        if (matches) {
            filteredClasses.append(classInfo);
        }
    }

    emit searchCompleted(filteredClasses);
    accept(); // 关闭对话框
}

void SearchDialog::onCancel() {
    emit cancelSearch(); // 发出取消查询信号
    reject(); // 关闭对话框
}


void SearchDialog::onDateTimeChange() {
    QDateTime startDate;
    QDateTime endDate;
    switch (_dateRangeComboBox->currentIndex()) {
        case 0: // 今天
            startDate = QDateTime(QDate::currentDate());
            endDate = QDateTime(QDate::currentDate(), QTime(23, 59, 59));
            break;
        case 1: {
            QDate lastWeekStart = QDate::currentDate().addDays(-7);
            startDate = QDateTime(lastWeekStart);
            endDate = QDateTime(QDate::currentDate(), QTime(23, 59, 59));
            break;
        }
        case 2: {
            QDate lastMonthStart = QDate::currentDate().addMonths(-1);
            startDate = QDateTime(lastMonthStart);
            endDate = QDateTime(QDate::currentDate(), QTime(23, 59, 59));
            break;
        }
        case 3: // 自定义
            startDate = _startDateEdit->dateTime();
            endDate = _endDateEdit->dateTime();
            break;
        default:
            return; // 无效选择
    }
    _startDateEdit->setDateTime(startDate);
    _endDateEdit->setDateTime(endDate);

    _startDateEdit->setEnabled(_dateRangeComboBox->currentIndex() == 3);
    _endDateEdit->setEnabled(_dateRangeComboBox->currentIndex() == 3);


}
