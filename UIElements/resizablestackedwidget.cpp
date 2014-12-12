#include "resizablestackedwidget.h"
#include<QDebug>
ResizableStackedWidget::ResizableStackedWidget(QWidget *parent) :
    QStackedWidget(parent)
{

    connect(this, SIGNAL(currentChanged(int)), this, SLOT(onCurrentChanged(int)) );

}

int ResizableStackedWidget::addWidget(QWidget* pWidget)
{
    pWidget->setMinimumWidth(350);
    pWidget->setMaximumWidth(350);
   pWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
   int id = QStackedWidget::addWidget(pWidget);
   return id;
}

void ResizableStackedWidget::onCurrentChanged(int index)
{
   QWidget* pWidget = widget(index);
   Q_ASSERT(pWidget);
   pWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
   pWidget->adjustSize();
   setMaximumHeight(pWidget->height());
}
