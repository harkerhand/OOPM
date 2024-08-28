#ifndef SIMPLETEXTDIALOG_H
#define SIMPLETEXTDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>

class SimpleTextDialog : public QDialog {
    Q_OBJECT

public:
    SimpleTextDialog(const QString &text, QWidget *parent = nullptr)
        : QDialog(parent) {

        // 移除问号
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

        QVBoxLayout *layout = new QVBoxLayout(this);
        QLabel *label = new QLabel(text, this);
        label->setWordWrap(true);  // 自动换行

        layout->addWidget(label);
        setLayout(layout);

        // 调整对话框大小以适应其内容
        label->adjustSize();
        adjustSize();
        setMinimumWidth(300);

        setWindowTitle(tr("Detail"));
    }
};



#endif // SIMPLETEXTDIALOG_H
