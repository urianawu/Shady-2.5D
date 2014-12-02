#include "channelPanel.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>

#define ROW_NUM 6
#define COLUMN_NUM 4

channelPanel::channelPanel(QWidget *parent, GLWidget *program) :
    QWidget(parent)
{

    m_RenderWindow = program;
    MSset = false;

    setWindowTitle("Channel Panel");

    QVBoxLayout *mainLayout = new QVBoxLayout();


    QTableWidget *tbWidget = new QTableWidget(6,4,this);


    //set column dynamic size
    QHeaderView *headerView = tbWidget->horizontalHeader();
    headerView->setSectionResizeMode(0, QHeaderView::Interactive);
    headerView->setSectionResizeMode(1, QHeaderView::Interactive);
    headerView->setSectionResizeMode(2, QHeaderView::Stretch);
    headerView->setSectionResizeMode(3, QHeaderView::Interactive);

    //hide table headers
    tbWidget->horizontalHeader()->hide();
    tbWidget->verticalHeader()->hide();

    //set table column width
    tbWidget->setColumnWidth(0, 22);
    tbWidget->setColumnWidth(1, 30);
    tbWidget->setColumnWidth(3, 30);

    //set table size
    tbWidget->setMaximumHeight(182);
    tbWidget->setMinimumHeight(182);
    tbWidget->setMinimumWidth(280);


    //channels

    //connect channel name to channel image
    //connect(tbWidget,SIGNAL(cellClicked(int,int)), this, SLOT(SetCurTex(int,int)) );

    //Rendered Image
    QTableWidgetItem* channel_rendered = new QTableWidgetItem(tr("Rendered Image"));
    tbWidget->setItem(0, 2, channel_rendered);
    pix_rendered.load(".//../Shady-2.5D/icons/transparent.jpg");
    pix_rendered = pix_rendered.scaled(25,25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    lbl_rendered = new ClickableLabel;
    lbl_rendered->setPixmap(pix_rendered);
    lbl_rendered->setAlignment(Qt::AlignCenter);
    lbl_rendered->setStyleSheet(
                "border:1px solid grey;"
               );

    QWidget* pic_rendered = new QWidget;
    QHBoxLayout* pic_rendered_layout = new QHBoxLayout(pic_rendered);
    pic_rendered_layout->setMargin(0);
    pic_rendered_layout->addWidget(lbl_rendered);
    pic_rendered_layout->setAlignment( Qt::AlignCenter );
    pic_rendered->setLayout(pic_rendered_layout);
    tbWidget->setCellWidget(0, 1, pic_rendered);

    btn_rendered = new QPushButton(tr("..."));
    tbWidget->setCellWidget(0, 3, btn_rendered);
    btn_rendered->setEnabled(false);

    QWidget *pWidget_rendered = new QWidget();
    checkbox_rendered = new QCheckBox;
    QHBoxLayout *pLayout_rendered = new QHBoxLayout(pWidget_rendered);
    pLayout_rendered->addWidget(checkbox_rendered);
    pLayout_rendered->setAlignment(Qt::AlignCenter);
    pLayout_rendered->setContentsMargins(1,1,1,1);
    pWidget_rendered->setStyleSheet(
                "QWidget::indicator:unchecked {image: url(.//../Shady-2.5D/icons/eye_grey.png);}"
                "QWidget::indicator:checked {image: url(.//../Shady-2.5D/icons/eye.png);}"
                );
    pWidget_rendered->setLayout(pLayout_rendered);
    checkbox_rendered->setChecked(true);

    tbWidget->setCellWidget(0, 0, pWidget_rendered);


    //FG
    QTableWidgetItem* channel_fg = new QTableWidgetItem(tr("Foreground"));
    tbWidget->setItem(1, 2, channel_fg);

    pix_fg.load(".//../Shady-2.5D/icons/transparent.jpg");
    pix_fg = pix_fg.scaled(25,25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    lbl_fg = new ClickableLabel;
    lbl_fg->setPixmap(pix_fg);
    lbl_fg->setAlignment(Qt::AlignCenter);
    lbl_fg->setStyleSheet(
                "border:1px solid grey;");

    QWidget* pic_fg = new QWidget;
    QHBoxLayout* pic_fg_layout = new QHBoxLayout(pic_fg);
    pic_fg_layout->setMargin(0);
    pic_fg_layout->addWidget(lbl_fg);
    pic_fg_layout->setAlignment( Qt::AlignCenter );
    pic_fg->setLayout(pic_fg_layout);
    tbWidget->setCellWidget(1, 1, pic_fg);

    btn_fg = new QPushButton(tr("..."));
    tbWidget->setCellWidget(1, 3, btn_fg);

    QWidget *pWidget_fg = new QWidget();
    checkbox_fg = new QCheckBox;
    QHBoxLayout *pLayout = new QHBoxLayout(pWidget_fg);
    pLayout->addWidget(checkbox_fg);
    pLayout->setAlignment(Qt::AlignCenter);
    pLayout->setContentsMargins(1,1,1,1);
    pWidget_fg->setStyleSheet(
                "QWidget::indicator:unchecked {image: url(.//../Shady-2.5D/icons/eye_grey.png);}"
                "QWidget::indicator:checked {image: url(.//../Shady-2.5D/icons/eye.png);}"
                );
    pWidget_fg->setLayout(pLayout);
    checkbox_fg->setChecked(true);

    tbWidget->setCellWidget(1, 0, pWidget_fg);

    //connect(btn_fg, SIGNAL(clicked()), this, SLOT(GetEnv()));
    //connect(lbl_fg, SIGNAL(clicked()), this, SLOT(GetEnv()));

    //DI1
    QTableWidgetItem* channel_bright = new QTableWidgetItem(tr("Bright Image"));
    tbWidget->setItem(2, 2, channel_bright);

    pix_br.load(".//../Shady-2.5D/icons/transparent.jpg");
    pix_br = pix_br.scaled(25,25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    lbl_br = new ClickableLabel;
    lbl_br->setPixmap(pix_br);
    lbl_br->setAlignment(Qt::AlignCenter);
    lbl_br->setStyleSheet(
                "border:1px solid grey;");

    QWidget* pic_br = new QWidget;
    QHBoxLayout* pic_br_layout = new QHBoxLayout(pic_br);
    pic_br_layout->setMargin(0);
    pic_br_layout->addWidget(lbl_br);
    pic_br_layout->setAlignment( Qt::AlignCenter );
    pic_br->setLayout(pic_br_layout);
    tbWidget->setCellWidget(2, 1, pic_br);

    channel_bright->setSizeHint(QSize(channel_bright->sizeHint().width(), 30));

    btn_br = new QPushButton(tr("..."));
    tbWidget->setCellWidget(2, 3, btn_br);


    QWidget *pWidget_br = new QWidget();
    checkbox_br = new QCheckBox;
    checkbox_br->setChecked(true);

    QHBoxLayout *pLayout_br = new QHBoxLayout(pWidget_br);
    pLayout_br->addWidget(checkbox_br);
    pLayout_br->setAlignment(Qt::AlignCenter);
    pLayout_br->setContentsMargins(1,1,1,1);
    pWidget_br->setStyleSheet(
                "QWidget::indicator:unchecked {image: url(.//../Shady-2.5D/icons/eye_grey.png);}"
                "QWidget::indicator:checked {image: url(.//../Shady-2.5D/icons/eye.png);}"
                );
    pWidget_br->setLayout(pLayout_br);

    tbWidget->setCellWidget(2, 0, pWidget_br);

    //connect(btn_br, SIGNAL(clicked()), this, SLOT(GetBr()));
    //connect(lbl_br, SIGNAL(clicked()), this, SLOT(GetBr()));

    //DI0
    QTableWidgetItem* channel_dark = new QTableWidgetItem(tr("Dark Image"));
    tbWidget->setItem(3, 2, channel_dark);

    pix_da.load(".//../Shady-2.5D/icons/transparent.jpg");
    pix_da = pix_da.scaled(25,25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    lbl_da = new ClickableLabel;
    lbl_da->setPixmap(pix_da);
    lbl_da->setAlignment(Qt::AlignCenter);
    lbl_da->setStyleSheet(
                "border:1px solid grey;");

    QWidget* pic_da = new QWidget;
    QHBoxLayout* pic_da_layout = new QHBoxLayout(pic_da);
    pic_da_layout->setMargin(0);
    pic_da_layout->addWidget(lbl_da);
    pic_da_layout->setAlignment( Qt::AlignCenter );
    pic_da->setLayout(pic_da_layout);
    tbWidget->setCellWidget(3, 1, pic_da);

    channel_dark->setSizeHint(QSize(channel_dark->sizeHint().width(), 30));

    btn_da = new QPushButton(tr("..."));
    tbWidget->setCellWidget(3, 3, btn_da);


    QWidget *pWidget_da = new QWidget();
    checkbox_da = new QCheckBox;
    checkbox_da->setChecked(true);

    QHBoxLayout *pLayout_da = new QHBoxLayout(pWidget_da);
    pLayout_da->addWidget(checkbox_da);
    pLayout_da->setAlignment(Qt::AlignCenter);
    pLayout_da->setContentsMargins(1,1,1,1);
    pWidget_da->setStyleSheet(
                "QWidget::indicator:unchecked {image: url(.//../Shady-2.5D/icons/eye_grey.png);}"
                "QWidget::indicator:checked {image: url(.//../Shady-2.5D/icons/eye.png);}"
                );
    pWidget_da->setLayout(pLayout_da);

    tbWidget->setCellWidget(3, 0, pWidget_da);

    //connect(btn_da, SIGNAL(clicked()), this, SLOT(GetDa()));
    //connect(lbl_da, SIGNAL(clicked()), this, SLOT(GetDa()));

    //BG
    QTableWidgetItem* channel_bg = new QTableWidgetItem(tr("Background"));
    tbWidget->setItem(4, 2, channel_bg);

    pix_bg.load(".//../Shady-2.5D/icons/transparent.jpg");
    pix_bg = pix_bg.scaled(25,25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    lbl_bg = new ClickableLabel;
    lbl_bg->setPixmap(pix_bg);
    lbl_bg->setAlignment(Qt::AlignCenter);
    lbl_bg->setStyleSheet(
                "border:1px solid grey;");

    QWidget* pic_bg = new QWidget;
    QHBoxLayout* pic_bg_layout = new QHBoxLayout(pic_bg);
    pic_bg_layout->setMargin(0);
    pic_bg_layout->addWidget(lbl_bg);
    pic_bg_layout->setAlignment( Qt::AlignCenter );
    pic_bg->setLayout(pic_bg_layout);
    tbWidget->setCellWidget(4, 1, pic_bg);

    btn_bg = new QPushButton(tr("..."));
    tbWidget->setCellWidget(4, 3, btn_bg);


    QWidget *pWidget_bg = new QWidget();
    checkbox_bg = new QCheckBox;
    checkbox_bg->setChecked(true);

    QHBoxLayout *pLayout_bg = new QHBoxLayout(pWidget_bg);
    pLayout_bg->addWidget(checkbox_bg);
    pLayout_bg->setAlignment(Qt::AlignCenter);
    pLayout_bg->setContentsMargins(1,1,1,1);
    pWidget_bg->setStyleSheet(
                "QWidget::indicator:unchecked {image: url(.//../Shady-2.5D/icons/eye_grey.png);}"
                "QWidget::indicator:checked {image: url(.//../Shady-2.5D/icons/eye.png);}"
                );
    pWidget_bg->setLayout(pLayout_bg);

    tbWidget->setCellWidget(4, 0, pWidget_bg);

    //connect(lbl_bg, SIGNAL(clicked()), this, SLOT(GetBG()));
    //connect(btn_bg, SIGNAL(clicked()), this, SLOT(GetBG()));

    //SM
    QTableWidgetItem* channel_sm = new QTableWidgetItem(tr("Shape Map"));
    tbWidget->setItem(5, 2, channel_sm);


    pix_sm.load(".//../Shady-2.5D/icons/transparent.jpg");
    pix_sm = pix_sm.scaled(25,25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    lbl_sm = new ClickableLabel;
    lbl_sm->setPixmap(pix_sm);
    lbl_sm->setAlignment(Qt::AlignCenter);
    lbl_sm->setStyleSheet(
                "border:1px solid grey;");

    QWidget* pic_sm = new QWidget;
    QHBoxLayout* pic_sm_layout = new QHBoxLayout(pic_sm);
    pic_sm_layout->setMargin(0);
    pic_sm_layout->addWidget(lbl_sm);
    pic_sm_layout->setAlignment( Qt::AlignCenter );
    pic_sm->setLayout(pic_sm_layout);
    tbWidget->setCellWidget(5, 1, pic_sm);

    btn_sm = new QPushButton(tr("..."));
    tbWidget->setCellWidget(5, 3, btn_sm);


    QWidget *pWidget_sm = new QWidget();
    checkbox_sm = new QCheckBox;
    checkbox_sm->setChecked(true);

    //checkbox_sm->setChecked(true);
    //checkbox_sm->setEnabled(false);
    QHBoxLayout *pLayout_sm = new QHBoxLayout(pWidget_sm);
    pLayout_sm->addWidget(checkbox_sm);
    pLayout_sm->setAlignment(Qt::AlignCenter);
    pLayout_sm->setContentsMargins(1,1,1,1);
    pWidget_sm->setStyleSheet(
                "QWidget::indicator:unchecked {image: url(.//../Shady-2.5D/icons/eye_grey.png);}"
                "QWidget::indicator:checked {image: url(.//../Shady-2.5D/icons/eye.png);}"
                );
    pWidget_sm->setLayout(pLayout_sm);

    tbWidget->setCellWidget(5, 0, pWidget_sm);

    //connect(btn_sm, SIGNAL(clicked()), this, SLOT(GetSM()));
    //connect(lbl_sm, SIGNAL(clicked()), this, SLOT(GetSM()));



    //set layout
    mainLayout->addWidget(tbWidget);
    setLayout(mainLayout);

}

void channelPanel::SetCurTex(int row, int column)
{
    int index = 0;
    checkbox_rendered->setChecked(false);
    checkbox_fg->setChecked(false);
    checkbox_br->setChecked(false);
    checkbox_da->setChecked(false);
    checkbox_bg->setChecked(false);
    checkbox_sm->setChecked(false);

    if(column == 2){
        if(row == 0){//rendered image
            index = 0;
            checkbox_rendered->setChecked(true);
            checkbox_fg->setChecked(true);
            checkbox_br->setChecked(true);
            checkbox_da->setChecked(true);
            checkbox_bg->setChecked(true);
            checkbox_sm->setChecked(true);
        }else if(row == 1){//foreground
            index = 7;
            checkbox_fg->setChecked(true);
        }else if(row == 2){//bright image
            index = 3;
            checkbox_br->setChecked(true);
        }else if(row == 3){//dark image
            index = 2;
            checkbox_da->setChecked(true);
        }else if(row == 4){//background
            index = 4;
            checkbox_bg->setChecked(true);
        }else if(row == 5){//shape map
            index = 1;
            checkbox_sm->setChecked(true);
        }
    }

    ShaderProgram *ShaderP = m_RenderWindow->getCShader();
    if(ShaderP&&ShaderP->isInitialized())
    {
        ShaderP->bind();
        ShaderP->SetCurTex(index);
    }
    m_RenderWindow->updateGL();
}

void channelPanel::GetBG()
{
    //    QPixmap img = GetPixmap(m_FileWidget->currentRow(),m_BGFileName);
    //    if(!img.isNull())
    //        emit SetBG(img);
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    QPushButton::tr("Open Image"), "/home/Background", QPushButton::tr("Image Files (*.png *.jpg *.bmp)"));
    QImage LoadedImage;
    if(LoadedImage.load(fileName))
    {
        ShaderProgram *ShaderP = m_RenderWindow->getRShader();
        QImage GLImage = m_RenderWindow->convertToGLFormat(LoadedImage);
        if(ShaderP&&ShaderP->isInitialized())
        {
            ShaderP->bind();
            ShaderP->LoadBGImage(GLImage.bits(),GLImage.width(),GLImage.height());
            pix_bg.load(fileName);
            pix_bg = pix_bg.scaled(25,25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            lbl_bg->setPixmap(pix_bg);
        }
        m_RenderWindow->reloadShader();

    }
}

void channelPanel::GetEnv()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    QPushButton::tr("Open Image"), "/home/Foreground", QPushButton::tr("Image Files (*.png *.jpg *.bmp)"));
    QImage LoadedImage;
    if(LoadedImage.load(fileName))
    {
        ShaderProgram *ShaderP = m_RenderWindow->getRShader();
        QImage GLImage = m_RenderWindow->convertToGLFormat(LoadedImage);
        if(ShaderP&&ShaderP->isInitialized())
        {
            ShaderP->bind();
            ShaderP->LoadEnvImage(GLImage.bits(),GLImage.width(),GLImage.height());
            pix_fg.load(fileName);
            pix_fg = pix_fg.scaled(25,25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            lbl_fg->setPixmap(pix_fg);
        }
        m_RenderWindow->reloadShader();
    }
}

void channelPanel::GetSM()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    QPushButton::tr("Open Image"), "/home/ShapeMap", QPushButton::tr("Image Files (*.png *.jpg *.bmp)"));
    qDebug() << "file name:" << fileName;
    if(fileName != NULL || fileName != ""){
        //m_imgShape->m_SMFile = fileName;
        m_imgShape->m_texUpdate = ImageShape::UPDATE_SM;
        pix_sm.load(fileName);
        pix_sm = pix_sm.scaled(25,25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        lbl_sm->setPixmap(pix_sm);
        //m_RenderWindow->reloadShader();
        m_RenderWindow->reloadShader();

    }
}

void channelPanel::GetBr()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    QPushButton::tr("Open Image"), "/home/BrightImage", QPushButton::tr("Image Files (*.png *.jpg *.bmp)"));

    if(fileName != NULL || fileName != ""){
        //m_imgShape->m_BrightFile = fileName;
        m_imgShape->m_texUpdate = ImageShape::UPDATE_BRIGHT;
        pix_br.load(fileName);
        pix_br = pix_br.scaled(25,25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        lbl_br->setPixmap(pix_br);
        m_RenderWindow->reloadShader();



    }
}

void channelPanel::GetDa()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    QPushButton::tr("Open Image"), "/home/DarkImage", QPushButton::tr("Image Files (*.png *.jpg *.bmp)"));
    if(fileName != NULL || fileName != ""){
        //m_imgShape->m_DarkFile = fileName;
        m_imgShape->m_texUpdate = ImageShape::UPDATE_DARK;
        pix_da.load(fileName);
        pix_da = pix_da.scaled(25,25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        lbl_da->setPixmap(pix_da);
        m_RenderWindow->reloadShader();

    }
}
