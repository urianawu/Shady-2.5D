#include "channelPanel.h"



channelPanel::channelPanel(QWidget *parent, GLWidget *program)
{
    setWindowTitle("Channel Panel");

    QVBoxLayout *mainLayout = new QVBoxLayout();

    tbWidget->verticalHeader()->hide();

    this->addRowAfter("Bright Image",0,true);
    this->addRowAfter("Dark Image",1,true);
    this->addRowAfter("Shape Map",2,true);

    //set layout
    mainLayout->addWidget(tbWidget);
    setLayout(mainLayout);

}

void channelPanel::addRowAfter(QString rowName, int rowNum, bool canImport)
{
    TbWidgetPanel::addRowAfter(rowName, rowNum);
    QPushButton* btn = new QPushButton(tr("..."));
    btns.append(btn);
    if (canImport){
        tbWidget->setCellWidget(rowNum, 3, btn);
        getFile(btn, rowName);
    }else {
        btn->setDisabled(true);
        tbWidget->setCellWidget(rowNum, 3, btn);
    }

}

void channelPanel::getFile(QPushButton* btn, QString rowName)
{
    if(rowName == "Bright Image")
    {
        connect(btn, SIGNAL(clicked()), this, SLOT(GetBr()));
    }
    else if (rowName == "Dark Image")
    {
        connect(btn, SIGNAL(clicked()), this, SLOT(GetDa()));
    }
    else if (rowName == "Shape Map")
    {
        connect(btn, SIGNAL(clicked()), this, SLOT(GetSM()));
    }
}

//void channelPanel::SetCurTex(int row, int column)
//{
//    int index = 0;
//    checkbox_rendered->setChecked(false);
//    checkbox_fg->setChecked(false);
//    checkbox_br->setChecked(false);
//    checkbox_da->setChecked(false);
//    checkbox_bg->setChecked(false);
//    checkbox_sm->setChecked(false);

//    if(column == 2){
//        if(row == 0){//rendered image
//            index = 0;
//            checkbox_rendered->setChecked(true);
//            checkbox_fg->setChecked(true);
//            checkbox_br->setChecked(true);
//            checkbox_da->setChecked(true);
//            checkbox_bg->setChecked(true);
//            checkbox_sm->setChecked(true);
//        }else if(row == 1){//foreground
//            index = 7;
//            checkbox_fg->setChecked(true);
//        }else if(row == 2){//bright image
//            index = 3;
//            checkbox_br->setChecked(true);
//        }else if(row == 3){//dark image
//            index = 2;
//            checkbox_da->setChecked(true);
//        }else if(row == 4){//background
//            index = 4;
//            checkbox_bg->setChecked(true);
//        }else if(row == 5){//shape map
//            index = 1;
//            checkbox_sm->setChecked(true);
//        }
//    }

//    ShaderProgram *ShaderP = m_RenderWindow->getCShader();
//    if(ShaderP&&ShaderP->isInitialized())
//    {
//        ShaderP->bind();
//        ShaderP->SetCurTex(index);
//    }
//    m_RenderWindow->updateGL();
//}



void channelPanel::GetSM()
{
//    QString fileName = QFileDialog::getOpenFileName(this,
//                                                    QPushButton::tr("Open Image"), "/home/ShapeMap", QPushButton::tr("Image Files (*.png *.jpg *.bmp)"));
//    qDebug() << "file name:" << fileName;
//    if(fileName != NULL || fileName != ""){
//        //m_imgShape->m_SMFile = fileName;
//        m_imgShape->m_texUpdate = ImageShape::UPDATE_SM;
//        pix_sm.load(fileName);
//        pix_sm = pix_sm.scaled(25,25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//        lbl_sm->setPixmap(pix_sm);
//        //m_RenderWindow->reloadShader();
//        m_RenderWindow->reloadShader();

//    }
}

void channelPanel::GetBr()
{
//    QString fileName = QFileDialog::getOpenFileName(this,
//                                                    QPushButton::tr("Open Image"), "/home/BrightImage", QPushButton::tr("Image Files (*.png *.jpg *.bmp)"));

//    if(fileName != NULL || fileName != ""){
//        //m_imgShape->m_BrightFile = fileName;
//        m_imgShape->m_texUpdate = ImageShape::UPDATE_BRIGHT;
//        pix_br.load(fileName);
//        pix_br = pix_br.scaled(25,25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//        lbl_br->setPixmap(pix_br);
//        m_RenderWindow->reloadShader();



//    }
}

void channelPanel::GetDa()
{
//    QString fileName = QFileDialog::getOpenFileName(this,
//                                                    QPushButton::tr("Open Image"), "/home/DarkImage", QPushButton::tr("Image Files (*.png *.jpg *.bmp)"));
//    if(fileName != NULL || fileName != ""){
//        //m_imgShape->m_DarkFile = fileName;
//        m_imgShape->m_texUpdate = ImageShape::UPDATE_DARK;
//        pix_da.load(fileName);
//        pix_da = pix_da.scaled(25,25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//        lbl_da->setPixmap(pix_da);
//        m_RenderWindow->reloadShader();

//    }
}
