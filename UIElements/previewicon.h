#ifndef PREVIEWICON_H
#define PREVIEWICON_H

#include <QWidget>
#include <QHBoxLayout>
#include <QString>

#include "clickablelabel.h"

class PreviewIcon : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewIcon(QWidget *parent = 0);
    void setIcon(QString &iconPath);
    QWidget* getCellWidget(){return cellWidget;}

private:
    QWidget* cellWidget;
    QPixmap pix;
    ClickableLabel *lbl;
signals:
    
public slots:
    
};

#endif // PREVIEWICON_H
