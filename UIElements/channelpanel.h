#ifndef CHANNELPANEL_H
#define CHANNELPANEL_H

#include <QtGui>
#include <QWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>

#include "../glwidget.h"
#include "../Renderer/shaderprogram.h"
#include "../Renderer/imageshape.h"
#include "../UIElements/clickablelabel.h"

class channelPanel : public QWidget
{
    Q_OBJECT
public:
    channelPanel(QWidget *parent = 0, GLWidget *program = NULL);
    void SetGL(GLWidget *p){m_RenderWindow = p;}
    void setMS(ImageShape* pMS){m_imgShape = pMS; MSset = true;}
    bool isMSset(){return MSset;}


    QPushButton * btn_rendered;
    QPushButton * btn_fg;
    QPushButton * btn_da;
    QPushButton * btn_bg;
    QPushButton * btn_sm;

signals:
    
public slots:
    void GetBG();
    void GetEnv();
    void GetSM();
    void GetBr();
    void GetDa();
    void SetCurTex(int, int);

private:
    bool MSset;

    GLWidget *m_RenderWindow;
    ImageShape* m_imgShape;

    QPixmap pix_rendered;
    ClickableLabel *lbl_rendered;
    QCheckBox *checkbox_rendered;


    QPixmap pix_fg;
    ClickableLabel *lbl_fg;
    QCheckBox *checkbox_fg;

    QPixmap pix_br;
    ClickableLabel *lbl_br;
    QCheckBox *checkbox_br;
    QPushButton * btn_br;

    QPixmap pix_da;
    ClickableLabel *lbl_da;
    QCheckBox *checkbox_da;

    QPixmap pix_bg;
    ClickableLabel *lbl_bg;
    QCheckBox *checkbox_bg;

    QPixmap pix_sm;
    ClickableLabel *lbl_sm;
    QCheckBox *checkbox_sm;
};

#endif // CHANNELPANEL_H
