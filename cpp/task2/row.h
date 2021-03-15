#ifndef ROW_H
#define ROW_H

#include <QLabel>
#include <QWidget>
#include <qlineedit.h>
#include <QValidator>
#include <QVBoxLayout>

class row : public QWidget {
    Q_OBJECT
private:
    QLabel* _lab;
    QLineEdit* _row;

public:
    explicit row(const QString &lab_str, const QString &row_str, QWidget *parent = nullptr)
        : QWidget(parent)
        , _lab(new QLabel(lab_str))
        , _row(new QLineEdit(row_str))
    {
        QVBoxLayout* box = new QVBoxLayout();
        box->addWidget(_lab);
        box->addWidget(_row);
        box->setSpacing(0);
        box->setContentsMargins(0, 0, 0, 0);
        this->setLayout(box);
        this->setContentsMargins(0, 0, 0, 0);
    }

    void setValidator(const QValidator* val) {
        _row->setValidator(val);
    }

    QString text() {
        return _row->text();
    }
};

#endif // ROW_H
