#ifndef INDICATOR_H
#define INDICATOR_H

#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>

class indicator : public QWidget
{
    Q_OBJECT
private:
    QLabel *_lab;

public:
    explicit indicator(QWidget *parent = nullptr)
        : QWidget(parent)
        , _lab(new QLabel)
    {
        QVBoxLayout* box = new QVBoxLayout();
        box->addWidget(_lab);
        box->setSpacing(0);
        box->setContentsMargins(0, 0, 0, 0);
        this->setLayout(box);
        this->setContentsMargins(0, 0, 0, 0);
        this->setAutoFillBackground(true);
    }


    void set(const QString& str, QColor color) {
        this->setPalette(QPalette(QPalette::Background, color));
        _lab->setText(str);
        this->repaint();
    }

};

#endif // INDICATOR_H
