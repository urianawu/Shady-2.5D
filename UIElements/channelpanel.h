#ifndef CHANNELPANEL_H
#define CHANNELPANEL_H

#include <QtGui>

#include <QPushButton>
#include <QFileDialog>

#include "../UIElements/tbwidgetpanel.h"


class channelPanel : public TbWidgetPanel
{
    Q_OBJECT
public:
	channelPanel(QWidget *parent = 0, GLWidget *program = NULL);
    void addRowAfter(QString rowName, int rowNum, bool canImport);
	void LoadTextureImage();
	void setImageShape(ImageShape* img){curImageShape = img;}
signals:
    
private:

    GLWidget *m_RenderWindow;
	ImageShape* curImageShape;
    QVector<QPushButton*> btns;
    void getFile(QPushButton* btn, QString rowName);

public slots:

    void GetSM();
    void GetBr();
    void GetDa();
    //void SetCurTex(int, int);

};

#endif // CHANNELPANEL_H
