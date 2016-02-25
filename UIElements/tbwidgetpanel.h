#ifndef TBWIDGETPANEL_H
#define TBWIDGETPANEL_H

#include <QtGui>
#include <QWidget>
#include <QTableWidget>
#include <QString>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QCheckBox>
#include <QVector>
#include <QPushButton>

#include "../glwidget.h"
#include "../UIElements/eyecheckbox.h"
#include "../UIElements/previewicon.h"

#include "../Renderer/shaderprogram.h"
#include "../Renderer/imageshape.h"

class TbWidgetPanel : public QWidget
{

public:
    TbWidgetPanel(QWidget *parent = 0);

    bool deleteRow();//arg: shape list number

    QCheckBox* getCheckBoxAtRow(int rowNum){return checkBoxes.value(rowNum)->getCheckBox();}
    void setIconAtRow(int rowNum, QString &filePath){icons.value(rowNum)->setIcon(filePath);}

    void setMS(ImageShape* pMS){m_imgShape = pMS; MSset = true;}
    bool isMSset(){return MSset;}

protected:
    void init();
    void addRowAfter(QString rowName, int rowNum);
    int totalRows, totalCols;

    bool MSset;

    ImageShape* m_imgShape;

    QTableWidget *tbWidget;
    QVector<EyeCheckBox*> checkBoxes;
    QVector<PreviewIcon*> icons;
};

#endif // TBWIDGETPANEL_H
