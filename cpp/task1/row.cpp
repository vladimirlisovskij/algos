#include "row.h"

row::row(QString str, QWidget *parent)
    : QWidget(parent)
    , _lab(new QLabel(str))
    , _row(new QLineEdit)
{
    QVBoxLayout* box = new QVBoxLayout();
    box->addWidget(_lab);
    box->addWidget(_row);
    box->setSpacing(0);
    box->setContentsMargins(0, 0, 0, 0);
    this->setLayout(box);
    this->setContentsMargins(0, 0, 0, 0);
}

void row::setValidator(const QValidator *val)
{
    _row->setValidator(val);
}

QString row::text()
{
    return _row->text();
}
