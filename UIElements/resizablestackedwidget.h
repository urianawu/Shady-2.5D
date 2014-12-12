#ifndef RESIZABLESTACKEDWIDGET_H
#define RESIZABLESTACKEDWIDGET_H

#include <QStackedWidget>

class ResizableStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit ResizableStackedWidget(QWidget *parent = 0);
    int addWidget(QWidget* pWidget);
signals:
    
public slots:
    void onCurrentChanged(int index);
};

#endif // RESIZABLESTACKEDWIDGET_H
