#ifndef LAYERPANEL_H
#define LAYERPANEL_H


#include <QtGui>

#include <QPushButton>
#include <QFileDialog>

#include <QDebug>
#include "../UIElements/tbwidgetpanel.h"



class LayerPanel : public TbWidgetPanel
{
    Q_OBJECT
public:
	LayerPanel(QWidget *parent = 0, GLWidget *program = NULL);
    void addRowAfter(QString rowName, int rowNum, bool canImport);

private:
    GLWidget *m_RenderWindow;
    QVector<QPushButton*> btns;
    QTableWidgetItem* draggableIcon;

    void getFile(QPushButton* btn, QString rowName);
signals:
    
public slots:
    void getItems();

    void GetBG();
    void GetEnv();
//    void GetSM();
//    void GetBr();
//    void GetDa();
//    void SetCurTex(int, int);
};

#endif // LAYERPANEL_H
