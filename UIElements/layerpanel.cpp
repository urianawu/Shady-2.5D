#include "layerpanel.h"


LayerPanel::LayerPanel(QWidget *parent, GLWidget *program)
{


    setWindowTitle("Layer Panel");
    QVBoxLayout *mainLayout = new QVBoxLayout();

    tbWidget->verticalHeader()->setSectionsMovable(true);


    //initialize drag icon
    draggableIcon = new QTableWidgetItem(QIcon(tr(".//../Shady-2.5D/UIElements/icons/DragDrop.png")),NULL);
    this->addRowAfter("Foreground",0,true);
    this->addRowAfter("Background",1,true);


    //set layout
    mainLayout->addWidget(tbWidget);
    setLayout(mainLayout);
}

void LayerPanel::addRowAfter(QString rowName, int rowNum, bool canImport)
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
    tbWidget->setVerticalHeaderItem ( rowNum, draggableIcon);

}

void LayerPanel::getFile(QPushButton* btn, QString rowName)
{
    if(rowName == "Background")
    {
        connect(btn, SIGNAL(clicked()), this, SLOT(GetBG()));
    }
    if (rowName == "Foreground")
    {
        connect(btn, SIGNAL(clicked()), this, SLOT(GetEnv()));
    }
}

void LayerPanel::GetBG()
{

    QString fname = QFileDialog::getOpenFileName(this,"Choose Background Image");
    Session::get()->setBG(fname.toUtf8().constData());
}

void LayerPanel::GetEnv()
{
    //possibly defected
    QString fileName = QFileDialog::getOpenFileName(this,
    QPushButton::tr("Open Image"), "/home/jana", QPushButton::tr("Image Files (*.png *.jpg *.bmp)"));
    QImage LoadedImage;
    if(LoadedImage.load(fileName))
    {
        ShaderProgram *ShaderP = Session::get()->glWidget()->getRShader();
        QImage GLImage = Session::get()->glWidget()->convertToGLFormat(LoadedImage);
        if(ShaderP&&ShaderP->isInitialized())
        {
            ShaderP->bind();
            ShaderP->LoadEnvImage(GLImage.bits(),GLImage.width(),GLImage.height());
        }
        Session::get()->glWidget()->updateGL();
    }
}
