#include "clickablelabel.h"

ClickableLabel::ClickableLabel( const QString& text, QWidget * parent ) :
    QLabel(parent)

  {
      this->setText(text);
    this->setFixedSize(25,25);
  }

  ClickableLabel::~ClickableLabel()
  {
  }

  void ClickableLabel::mousePressEvent ( QMouseEvent * event )

  {
      emit clicked();
  }
