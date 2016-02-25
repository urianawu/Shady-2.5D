#include "channelPanel.h"



channelPanel::channelPanel(QWidget *parent, GLWidget *program) :
    TbWidgetPanel(parent)
{
    m_RenderWindow = program;
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
	Session::get()->setChannel(NORMAL_CHANNEL);
	LoadTextureImage();
}

void channelPanel::GetBr()
{
	Session::get()->setChannel(BRIGHT_CHANNEL);
	LoadTextureImage();
}

void channelPanel::GetDa()
{
	Session::get()->setChannel(DARK_CHANNEL);
	LoadTextureImage();
}

void channelPanel::LoadTextureImage()
{
	QString fileName = QFileDialog::getOpenFileName(this, QPushButton::tr("Open Image"), "/home/", QPushButton::tr("Image Files (*.png *.jpg *.bmp)"));
	int channel = (Session::get()->channel() <  ACTIVE_CHANNELS) ? Session::get()->channel() : (ACTIVE_CHANNELS-1);
	curImageShape->m_fileName[channel] = fileName;
	curImageShape->m_texUpdate  = 1 << channel;
	//Session::get()->glWidget()->updateGL();
}
