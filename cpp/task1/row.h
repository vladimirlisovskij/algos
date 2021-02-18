#ifndef ROW_H
#define ROW_H

#include <QLabel>
#include <QWidget>
#include <qlineedit.h>
#include <QValidator>
#include <QVBoxLayout>

class row : public QWidget
{
    Q_OBJECT
private:
    QLabel* _lab;
    QLineEdit* _row;

public:
    explicit row(QString str, QWidget *parent = nullptr);
    void setValidator(const QValidator* val);
    QString text();

};

#endif // ROW_H
