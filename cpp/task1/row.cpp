#include "row.h"

row::row(const QString &lab_str, const QString &row_str, QWidget *parent)
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

void row::setValidator(const QValidator *val) {
    _row->setValidator(val);
}

QString row::text() {
    return _row->text();
}
