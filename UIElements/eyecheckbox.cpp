#include "eyecheckbox.h"

EyeCheckBox::EyeCheckBox(QWidget *parent) :
    QWidget(parent)
{
    cellWidget = new QWidget();
    checkBox = new QCheckBox;
    QHBoxLayout *layout;
    layout = new QHBoxLayout(cellWidget);
    layout->addWidget(checkBox);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(1,1,1,1);
    cellWidget->setStyleSheet(
                "QWidget::indicator:unchecked {image: url(.//../Shady-2.5D/UIElements/icons/eye_grey.png);}"
                "QWidget::indicator:checked {image: url(.//../Shady-2.5D/UIElements/icons/eye.png);}"
                );
    cellWidget->setLayout(layout);
    checkBox->setChecked(true);

}
