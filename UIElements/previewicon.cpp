#include "previewicon.h"

PreviewIcon::PreviewIcon(QWidget *parent) :
    QWidget(parent)
{
        pix.load(".//../Shady-2.5D/icons/transparent.jpg");
        pix = pix.scaled(25,25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        lbl = new ClickableLabel;
        lbl->setPixmap(pix);
        lbl->setAlignment(Qt::AlignCenter);
        lbl->setStyleSheet(
                    "border:1px solid grey;"
                   );

        cellWidget = new QWidget;
        QHBoxLayout* pic_layout = new QHBoxLayout(cellWidget);
        pic_layout->setMargin(0);
        pic_layout->addWidget(lbl);
        pic_layout->setAlignment( Qt::AlignCenter );
        cellWidget->setLayout(pic_layout);
}

void PreviewIcon::setIcon(QString &iconPath)
{
    pix.load(iconPath);
    pix = pix.scaled(25,25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    lbl->setPixmap(pix);
}
