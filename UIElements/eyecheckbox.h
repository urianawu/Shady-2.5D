#ifndef EYECHECKBOX_H
#define EYECHECKBOX_H

#include <QWidget>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QString>

class EyeCheckBox : public QWidget
{
    Q_OBJECT
public:
    explicit EyeCheckBox(QWidget *parent = 0);
    QWidget* getCellWidget(){return cellWidget;}
    QCheckBox* getCheckBox(){return checkBox;}
private:
    QWidget* cellWidget;
    QCheckBox* checkBox;


signals:
    
public slots:
    
};

#endif // EYECHECKBOX_H
