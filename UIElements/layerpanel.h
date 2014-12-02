#ifndef LAYERPANEL_H
#define LAYERPANEL_H

#include <QWidget>
#include <QTableWidget>
#include <QtGui>
#include <QString>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QCheckBox>
#include <QVector>


#include "../glwidget.h"
#include "../UIElements/eyecheckbox.h"
#include "../UIElements/previewicon.h"


class LayerPanel : public QWidget
{
    Q_OBJECT
public:
    LayerPanel(QWidget *parent = 0, GLWidget *program = NULL);
    
    void addRow(QString rowName);//arg: shape list number
    bool deleteRow();//arg: shape list number

    QCheckBox* getCheckBoxAtRow(int rowNum){return checkBoxes.value(rowNum)->getCheckBox();}
    void setIconAtRow(int rowNum, QString &filePath){icons.value(rowNum)->setIcon(filePath);}

private:
    QTableWidget *tbWidget;
    int shapeNum;

    QTableWidgetItem * draggableIcon;
    QVector<EyeCheckBox*> checkBoxes;
    QVector<PreviewIcon*> icons;

signals:
    
public slots:
    
};

#endif // LAYERPANEL_H
