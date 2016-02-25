#include "layerpanel.h"


LayerPanel::LayerPanel( QWidget *parent, GLWidget *program) :
    TbWidgetPanel(parent)
{

    m_RenderWindow = program;
    setWindowTitle("Layer Panel");
    QVBoxLayout *mainLayout = new QVBoxLayout();

    tbWidget->verticalHeader()->setSectionsMovable(true);


    //initialize drag icon
    draggableIcon = new QTableWidgetItem(QIcon(tr(".//../Shady-2.5D/UIElements/icons/DragDrop.png")),NULL);
    this->addRowAfter("Foreground",0,true);
    this->addRowAfter("Background",1,true);

    //set layout
    mainLayout->addWidget(tbWidget);

    //debugging......
    QPushButton* btn_items = new QPushButton(tr("Item"));
    connect(btn_items,SIGNAL(clicked()),this,SLOT(getItems()));
    mainLayout->addWidget(btn_items);

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
    if (rowName[0] == 'I')//image shape
    {
        
    }
}

void LayerPanel::GetBG()
{

    QString fileName = QFileDialog::getOpenFileName(this,
    QPushButton::tr("Open Image"), "/home/jana", QPushButton::tr("Image Files (*.png *.jpg *.bmp)"));
    QImage LoadedImage;
    if(LoadedImage.load(fileName))
    {
        ShaderProgram *ShaderP = m_RenderWindow->getRShader();
        QImage GLImage = m_RenderWindow->convertToGLFormat(LoadedImage);
        if(ShaderP&&ShaderP->isInitialized())
        {
            ShaderP->bind();
            ShaderP->LoadBGImage(GLImage.bits(),GLImage.width(),GLImage.height());
        }
        m_RenderWindow->updateGL();
    }
}

void LayerPanel::GetEnv()
{
    //possibly defected
    QString fileName = QFileDialog::getOpenFileName(this,
    QPushButton::tr("Open Image"), "/home/", QPushButton::tr("Image Files (*.png *.jpg *.bmp)"));
    QImage LoadedImage;
    if(LoadedImage.load(fileName))
    {
        ShaderProgram *ShaderP = m_RenderWindow->getRShader();
        QImage GLImage = m_RenderWindow->convertToGLFormat(LoadedImage);
        if(ShaderP&&ShaderP->isInitialized())
        {
            ShaderP->bind();
            ShaderP->LoadEnvImage(GLImage.bits(),GLImage.width(),GLImage.height());
        }
        m_RenderWindow->updateGL();
    }
}
void LayerPanel::getItems()
{
    qDebug()<<"DEBUGGING............";
    for(int i = 0;i<tbWidget->rowCount();i++){
        qDebug()<<tbWidget->item(i,2)->text();
    }
}
