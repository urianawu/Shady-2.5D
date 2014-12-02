#include "layerpanel.h"

#define LAYER_COLUMNS 3
#define LAYER_ROWS 0

LayerPanel::LayerPanel(QWidget *parent, GLWidget *program) :
    QWidget(parent)
{
    setWindowTitle("Layer Panel");
    QVBoxLayout *mainLayout = new QVBoxLayout();
    tbWidget = new QTableWidget(LAYER_ROWS,LAYER_COLUMNS,this);

    //initialize shape number
    shapeNum = 0;

    //set column dynamic size
    QHeaderView *headerView = tbWidget->horizontalHeader();
    headerView->setSectionResizeMode(0, QHeaderView::Interactive);
    headerView->setSectionResizeMode(1, QHeaderView::Interactive);
    headerView->setSectionResizeMode(2, QHeaderView::Stretch);

    //set table column width
    tbWidget->setColumnWidth(0, 22);
    tbWidget->setColumnWidth(1, 30);

    //hide horizontal header
    headerView->hide();
    tbWidget->verticalHeader()->setSectionsMovable(true);

    //initialize drag icon
    draggableIcon = new QTableWidgetItem(QIcon(tr(".//../Shady-2.5D/icons/DragDrop.png")),NULL);



    //icon
//    QPixmap pix;
//    pix.load(".//../Shady-2.5D/icons/transparent.jpg");
//    pix = pix.scaled(25,25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    ClickableLabel *lbl = new ClickableLabel;
//    lbl->setPixmap(pix);
//    lbl->setAlignment(Qt::AlignCenter);
//    lbl->setStyleSheet(
//                "border:1px solid grey;"
//               );

//    QWidget* pic = new QWidget;
//    QHBoxLayout* pic_layout = new QHBoxLayout(pic);
//    pic_layout->setMargin(0);
//    pic_layout->addWidget(lbl);
//    pic_layout->setAlignment( Qt::AlignCenter );
//    pic->setLayout(pic_layout);

    //add row to table

    addRow(tr("Image Shape 1"));

    //set layout
    mainLayout->addWidget(tbWidget);
    setLayout(mainLayout);
}

void LayerPanel::addRow(QString rowName)
{
    //check box
    EyeCheckBox* checkbox = new EyeCheckBox();
    checkBoxes.append(checkbox);
    PreviewIcon* icon = new PreviewIcon();
    icons.append(icon);

    shapeNum++;
    tbWidget->insertRow( 0 );
    tbWidget->setCellWidget(0, 0, checkBoxes.value(shapeNum - 1)->getCellWidget() );
    tbWidget->setCellWidget(0, 1, icons.value(shapeNum - 1)->getCellWidget());
    tbWidget->setItem(0, 2, new QTableWidgetItem(rowName));

    tbWidget->setVerticalHeaderItem ( 0, draggableIcon);

}
