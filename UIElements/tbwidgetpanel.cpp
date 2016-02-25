#include "tbwidgetpanel.h"

TbWidgetPanel::TbWidgetPanel(QWidget *parent) :
    QWidget(parent)
{
    MSset = false;

    totalRows = 0;
    totalCols = 4;
    tbWidget = new QTableWidget(totalRows,totalCols,this);
    init();
}

void TbWidgetPanel::init()
{
    //set column dynamic size
    QHeaderView *headerView = tbWidget->horizontalHeader();
    headerView->setSectionResizeMode(0, QHeaderView::Interactive);
    headerView->setSectionResizeMode(1, QHeaderView::Interactive);
    headerView->setSectionResizeMode(2, QHeaderView::Stretch);
    headerView->setSectionResizeMode(3, QHeaderView::Interactive);

    //set table column width
    tbWidget->setColumnWidth(0, 22);
    tbWidget->setColumnWidth(1, 30);
    tbWidget->setColumnWidth(3, 30);

    //set table size
    tbWidget->setMaximumHeight(182);
    tbWidget->setMinimumHeight(182);
    tbWidget->setMinimumWidth(280);

    //hide horizontal header
    headerView->hide();
}

void TbWidgetPanel::addRowAfter(QString rowName, int rowNum)
{
    //check box
    EyeCheckBox* checkbox = new EyeCheckBox();
    checkBoxes.append(checkbox);
    PreviewIcon* icon = new PreviewIcon();
    icons.append(icon);


    totalRows++;
    tbWidget->insertRow( rowNum );
    tbWidget->setCellWidget(rowNum, 0, checkbox->getCellWidget() );
    tbWidget->setCellWidget(rowNum, 1, icon->getCellWidget());
    tbWidget->setItem(rowNum, 2, new QTableWidgetItem(rowName));


}

