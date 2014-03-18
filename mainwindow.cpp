/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "glwidget.h"
#include "mainwindow.h"
#include "sampleshape.h"
#include "canvas.h"
#include "meshshape/Patch.h"
#include "meshshape/spineshape.h"
#include "meshshape/meshshape.h"
#include "ellipseshape.h"
#include "shapecontrol.h"

#ifndef MODELING_MODE
#include "Renderer/renderoptionspenal.h"
#endif

double                  EllipseShape::Radius = 0.2;
ControlPoint_p          ControlPoint::_pTheActive = 0;


MainWindow::MainWindow()
{

    Session::init(this);

    centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    glWidget = Session::get()->glWidget();

    glWidgetArea = new QScrollArea;
    glWidgetArea->setWidget(glWidget);
    glWidgetArea->setWidgetResizable(true);
    glWidgetArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    glWidgetArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    glWidgetArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    glWidgetArea->setMinimumSize(50, 50);

    createActions();
    createMenus();

    initTools();

    QGridLayout *centralLayout = new QGridLayout;
    centralLayout->addWidget(glWidgetArea, 0, 0);

    centralWidget->setLayout(centralLayout);
    setWindowTitle(tr("Shady"));
    resize(1200, 900);

    // set the number of patch lines
    //Patch::setN(16);
}


void MainWindow::initScene(){
    //init scene
}

void MainWindow::initTools()
{

    QToolBar *toolbar = addToolBar("main toolbar");

    toolbar->addAction(dragAct);
    toolbar->addAction(previewAct);

    toolbar->addSeparator();

    toolbar->addAction(shapeInsertEllipseAct);
    toolbar->addAction(shapeInsertGridAct);
    toolbar->addAction(shapeInsert2NGonAct);
    toolbar->addAction(shapeInsertTorusAct);
    toolbar->addAction(shapeInsertSpineAct);
    toolbar->addAction(shapeInsertImageShapeAct);

    toolbar->addSeparator();

    toolbar->addAction(extrudeEdgeAct);
    toolbar->addAction(extrudeFaceAct);
    toolbar->addAction(insertSegmentAct);
    toolbar->addAction(deleteFaceAct);

    toolbar->addSeparator();

    toolbar->addSeparator();
    toolbar->addAction(assignPatternAct);
    toolbar->addAction(setFoldsAct);

    //init tool options dock
    optionsDockWidget = new QDockWidget(QString("Options"), this);
    attrDockWidget = new QDockWidget(QString("Attributes"), this);

    optionsStackedWidget = new QStackedWidget();
    attrStackedWidget = new QStackedWidget();

    optionsDockWidget->setWidget(optionsStackedWidget);
    attrDockWidget->setWidget(attrStackedWidget);


    this->addDockWidget(Qt::LeftDockWidgetArea, optionsDockWidget);
    this->addDockWidget(Qt::LeftDockWidgetArea, attrDockWidget);
    this->setDockOptions(!QMainWindow::AllowTabbedDocks);

#ifndef MODELING_MODE
    rendererDockWidget = new QDockWidget(QString("Render"), this);
    rendererDockWidget->setWidget(new RenderOptionsPenal(this, glWidget));
    this->addDockWidget(Qt::LeftDockWidgetArea, rendererDockWidget);
#endif


    addAttrWidget(new QWidget, 0);//default widget
    createAllOptionsWidgets();
}

int MainWindow::addOptionsWidget(QWidget* widget,int key){
    int id = optionsStackedWidget->addWidget(widget);
    _optionWidgetIDs[key] = id;
    return id;
}

void MainWindow::setOptionsWidget(int key){

    if (key==Options::NONE && (!optionsDockWidget->isVisible() || !optionsStackedWidget->isVisible()))
        return;

    int id = _optionWidgetIDs[key];
    optionsStackedWidget->setCurrentIndex(id);
    optionsDockWidget->setVisible(true); //modify later
}

int MainWindow::addAttrWidget(QWidget* widget, void* key){
    int id = attrStackedWidget->addWidget(widget);
    _attrWidgetIDs[key] = id;
    return id;
}

void MainWindow::setAttrWidget(void* key){

    if (!attrDockWidget->isVisible() || !attrStackedWidget->isVisible())
        return;

    map<void*, int>::const_iterator it = _attrWidgetIDs.find(key);
    if (it == _attrWidgetIDs.end()){
        attrStackedWidget->setCurrentIndex(0);
        return;
    }
    int id = it->second;
    attrStackedWidget->setCurrentIndex(id);
//    attrDockWidget->setVisible(true); //modify later
}

void MainWindow::removeAttrWidget(void* key){

    if (key==0)
        return;

    map<void*, int>::const_iterator it = _attrWidgetIDs.find(key);
    if (it == _attrWidgetIDs.end())
        return;

    int id = it->second;
    QWidget* widget = attrStackedWidget->find(id);
    attrStackedWidget->removeWidget(widget);
    delete widget;
}


void MainWindow::createActions()
{
    //file Actions
    fileNewAct = new QAction(tr("New"), this);
    fileNewAct->setShortcuts(QKeySequence::New);
    connect(fileNewAct, SIGNAL(triggered()), this, SLOT(newFile()));

    fileSaveAct = new QAction(tr("Save"), this);
    fileSaveAct->setShortcuts(QKeySequence::Save);
    connect(fileSaveAct, SIGNAL(triggered()), this, SLOT(saveFile()));

    fileSaveAsAct = new QAction(tr("Save As"), this);
    fileSaveAsAct->setShortcuts(QKeySequence::SaveAs);
    connect(fileSaveAsAct, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    fileOpenAct = new QAction(tr("Open"), this);
    fileOpenAct->setShortcuts(QKeySequence::Open);
    connect(fileOpenAct, SIGNAL(triggered()), this, SLOT(openFile()));

    fileExportINPAct = new QAction(tr("Export INP"), this);
    connect(fileExportINPAct, SIGNAL(triggered()), this, SLOT(exportINP()));


    editUndoAct = new QAction(tr("Undo"), this);
    editUndoAct->setShortcuts(QKeySequence::Undo);
    connect(editUndoAct, SIGNAL(triggered()), this, SLOT(undo()));

    editRedoAct = new QAction(tr("Redo"), this);
    editRedoAct->setShortcuts(QKeySequence::Redo);
    connect(editRedoAct, SIGNAL(triggered()), this, SLOT(redo()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    //view
    viewOptionsAct =  new QAction(tr("Show Options"), this);
    connect(viewOptionsAct, SIGNAL(triggered()), this, SLOT(viewOptions()));

    viewAttrAct =  new QAction(tr("Show Attributes"), this);
    connect(viewAttrAct, SIGNAL(triggered()), this, SLOT(viewAttr()));

    dragAct = new QAction(tr("Drag"), this);
    dragAct->setShortcut(Qt::Key_Space);
    connect(dragAct, SIGNAL(triggered()), this, SLOT(flipDrag()));


    previewAct = new QAction(tr("Preview"), this);
    previewAct->setShortcut(Qt::Key_F5);
    previewAct->setCheckable(true);
    connect(previewAct, SIGNAL(triggered()), this, SLOT(togglePreview()));
    //view Act
    shadingOnAct = new QAction(tr("&Shading On"), this);
    shadingOnAct->setShortcut('S');
    shadingOnAct->setCheckable(true);
    connect(shadingOnAct, SIGNAL(triggered()), this, SLOT(toggleShading()));

    previewOnAct = new QAction(tr("Preview On"), this);
    previewOnAct->setCheckable(true);
   // connect(previewOnAct, SIGNAL(triggered()), this, SLOT(flipRender()));

    ambientOnAct = new QAction(tr("&Ambient On"), this);
    ambientOnAct->setShortcut('A');
    ambientOnAct->setCheckable(true);
    connect(ambientOnAct, SIGNAL(triggered()), this, SLOT(toggleAmbient()));

    shadowOnAct = new QAction(tr("&Shadow On"), this);
    shadowOnAct->setShortcut('W');
    shadowOnAct->setCheckable(true);
    connect(shadowOnAct, SIGNAL(triggered()), this, SLOT(toggleShadow()));

    normalsOnAct = new QAction(tr("Show &Normals"), this);
    normalsOnAct->setShortcut('N');
    normalsOnAct->setCheckable(true);
    connect(normalsOnAct, SIGNAL(triggered()), this, SLOT(toggleNormals()));

    patchesOnAct = new QAction(tr("Show &Patches"), this);
    patchesOnAct->setShortcut('P');
    patchesOnAct->setCheckable(true);
    connect(patchesOnAct, SIGNAL(triggered()), this, SLOT(togglePathces()));

    //Mesh Shape Actions
    extrudeEdgeAct = new QAction(tr("&Extrude Edge"), this);
    extrudeEdgeAct->setShortcut(tr("Ctrl+E"));
    connect(extrudeEdgeAct, SIGNAL(triggered()), this, SLOT(selectExtrudeEdge()));

    extrudeFaceAct = new QAction(tr("Extrude &Face"), this);
    extrudeFaceAct->setShortcut(tr("Ctrl+Q"));
    connect(extrudeFaceAct, SIGNAL(triggered()), this, SLOT(selectExtrudeFace()));

    insertSegmentAct = new QAction(tr("&Insert Segment"), this);
    insertSegmentAct->setShortcut(tr("Ctrl+I"));
    connect(insertSegmentAct, SIGNAL(triggered()), this, SLOT(selectInsertSegment()));

    deleteFaceAct = new QAction(tr("&Delete Face"), this);
    deleteFaceAct->setShortcut(tr("Ctrl+D"));
    connect(deleteFaceAct, SIGNAL(triggered()), this, SLOT(selectDeleteFace()));


    assignPatternAct = new QAction(tr("&Assign Pattern "), this);
    assignPatternAct->setShortcut(tr("Ctrl+P"));
    connect(assignPatternAct, SIGNAL(triggered()), this, SLOT(selectAssignPatternTool()));

    setFoldsAct = new QAction(tr("&Set Folds "), this);
    setFoldsAct->setShortcut(tr("Ctrl+P"));
    connect(setFoldsAct, SIGNAL(triggered()), this, SLOT(selectSetFoldsTool()));

    QActionGroup* toolset = new QActionGroup(this);

    dragAct->setCheckable(true);
    dragAct->setChecked(true);

    previewAct->setCheckable(true);
    previewAct->setChecked(false);

    extrudeEdgeAct->setCheckable(true);
    extrudeFaceAct->setCheckable(true);
    insertSegmentAct->setCheckable(true);
    deleteFaceAct->setCheckable(true);

    extrudeEdgeAct->setActionGroup(toolset);
    extrudeFaceAct->setActionGroup(toolset);
    insertSegmentAct->setActionGroup(toolset);
    deleteFaceAct->setActionGroup(toolset);


    //SHAPE ACTIONS
    shapeInsertTorusAct = new QAction(tr("Torus"), this);
    connect(shapeInsertTorusAct, SIGNAL(triggered()), this, SLOT(newTorus()));

    shapeInsertEllipseAct = new QAction(tr("Ellipse"), this);
    connect(shapeInsertEllipseAct, SIGNAL(triggered()), this, SLOT(insertEllipse()));

    shapeInsertGridAct = new QAction(tr("Grid"), this);
    connect(shapeInsertGridAct, SIGNAL(triggered()), this, SLOT(newGrid()));

    shapeInsert2NGonAct = new QAction(tr("2NGon"), this);
    connect(shapeInsert2NGonAct, SIGNAL(triggered()), this, SLOT(new2NGon()));

    shapeInsertSpineAct = new QAction(tr("Spine"), this);
    connect(shapeInsertSpineAct, SIGNAL(triggered()), this, SLOT(newSpine()));

    shapeInsertFacialAct = new QAction(tr("Facial Shape"), this);
    connect(shapeInsertFacialAct, SIGNAL(triggered()), this, SLOT(newFacial()));

    shapeInsertImageShapeAct = new QAction(tr("Image Shape"), this);
    connect(shapeInsertImageShapeAct, SIGNAL(triggered()), this, SLOT(newImageShape()));

    shapeLockAct = new QAction(tr("&Lock"), this);
    shapeLockAct->setShortcut(tr("Ctrl+L"));
    connect(shapeLockAct, SIGNAL(triggered()), this, SLOT(toggleLockShape()));

    shapeMoveFrontAct = new QAction(tr("Move Front"), this);
    shapeMoveFrontAct->setShortcut(Qt::Key_PageUp);
    connect(shapeMoveFrontAct, SIGNAL(triggered()), this, SLOT(moveShapeToFront()));

    shapeMoveBackAct = new QAction(tr("Move Back"), this);
    shapeMoveBackAct->setShortcut(Qt::Key_PageDown);
    connect(shapeMoveBackAct, SIGNAL(triggered()), this, SLOT(moveShapeToBack()));

    shapeSendFrontAct = new QAction(tr("Send &Front"), this);
    shapeSendFrontAct->setShortcut(tr("Ctrl+F"));
    connect(shapeSendFrontAct, SIGNAL(triggered()), this, SLOT(sendShapeFront()));

    shapeSendBackAct = new QAction(tr("Send &Back"), this);
    shapeSendBackAct->setShortcut(tr("Ctrl+B"));
    connect(shapeSendBackAct, SIGNAL(triggered()), this, SLOT(sendShapeBack()));

    shapeDeleteAct = new QAction(tr("Delete"), this);
    shapeDeleteAct->setShortcut(Qt::Key_Delete);
    connect(shapeDeleteAct, SIGNAL(triggered()), this, SLOT(deleteShape()));

    shapeTransformAct = new QAction(tr("Transform"), this);
    shapeTransformAct->setShortcut(tr("Ctrl+T"));
    connect(shapeTransformAct, SIGNAL(triggered()), this, SLOT(transformShape()));

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));   
    fileMenu->addAction(fileNewAct);
    fileMenu->addAction(fileOpenAct);
    fileMenu->addAction(fileSaveAct);
    fileMenu->addAction(fileSaveAsAct);

    fileMenu->addSeparator();
    fileMenu->addAction("Import");
    fileMenu->addAction(fileExportINPAct);

    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(editUndoAct);
    editMenu->addAction(editRedoAct);

    viewMenu  = menuBar()->addMenu(tr("View"));
    viewMenu->addAction(viewOptionsAct);
    viewMenu->addAction(viewAttrAct);

    displayMenu  = menuBar()->addMenu(tr("Display"));
    displayMenu->addAction(normalsOnAct);
    displayMenu->addAction(patchesOnAct);
    displayMenu->addAction(shadingOnAct);
    displayMenu->addAction(ambientOnAct);
    displayMenu->addAction(shadowOnAct);
    displayMenu->addAction(previewOnAct);

    insertMenu = menuBar()->addMenu("Create");
    insertMenu->addAction(shapeInsertEllipseAct);
    insertMenu->addAction(shapeInsertGridAct);
    insertMenu->addAction(shapeInsert2NGonAct);
    insertMenu->addAction(shapeInsertTorusAct);
    insertMenu->addAction(shapeInsertSpineAct);
    insertMenu->addAction(shapeInsertFacialAct);

    shapeMenu  = menuBar()->addMenu(tr("Shape"));
    shapeMenu->addAction(shapeLockAct);
    shapeMenu->addAction(shapeDeleteAct);

    QMenu* arrangeMenu = shapeMenu->addMenu("Arrange");
    arrangeMenu->addAction(shapeMoveFrontAct);
    arrangeMenu->addAction(shapeMoveBackAct);
    arrangeMenu->addAction(shapeSendFrontAct);
    arrangeMenu->addAction(shapeSendBackAct);

    shapeMenu->addAction(shapeTransformAct);
    shapeMenu->addAction(shapeDeleteAct);
    shapeMenu->addAction("Group");
    shapeMenu->addAction("Rasterize");

    toolsMenu  = menuBar()->addMenu(tr("Tools"));
    QMenu * GeoTool = toolsMenu->addMenu("Geometry Tools");
    GeoTool->addAction(extrudeEdgeAct);
    GeoTool->addAction(extrudeFaceAct);
    GeoTool->addAction(insertSegmentAct);
    GeoTool->addAction(deleteFaceAct);

    QMenu *RenderTool = toolsMenu->addMenu("Preview Tools");
    RenderTool->addAction("Refraction");
    RenderTool->addAction("Alpha");
    RenderTool->addAction("Shadow");
    RenderTool->addAction("Cartoon");
    RenderTool->addAction("Translucancy");
    RenderTool->addAction("SM Quality");


    selectMenu  = menuBar()->addMenu(tr("Select"));
    selectMenu->addAction("Select All");
    selectMenu->addAction("Select Inverse");
    selectMenu->addAction("Grow Selection");
    selectMenu->addAction("Clear Selection");


    previewMenu = menuBar()->addMenu(tr("Render"));
    previewMenu->addAction("Filter Size");
    previewMenu->addAction("Toggle Point Light");
    previewMenu->addAction("Enviroment Map");

    windowMenu  = menuBar()->addMenu(tr("Window"));

    helpMenu    = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(aboutAct);
}

QSize MainWindow::getSize()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Grabber"),
                                         tr("Enter pixmap size:"),
                                         QLineEdit::Normal,
                                         tr("%1 x %2").arg(glWidget->width())
                                                      .arg(glWidget->height()),
                                         &ok);
    if (!ok)
        return QSize();

    QRegExp regExp(tr("([0-9]+) *x *([0-9]+)"));
    if (regExp.exactMatch(text)) {
        int width = regExp.cap(1).toInt();
        int height = regExp.cap(2).toInt();
        if (width > 0 && width < 2048 && height > 0 && height < 2048)
            return QSize(width, height);
    }

    return glWidget->size();
}

void MainWindow::keyPressEvent(QKeyEvent *event){

    bool ctrl = (event->modifiers() & Qt::ControlModifier);

    int key = event->key();

    if (key == Qt::Key_H)
    {
        Patch::flipH();
    }

    if (key == Qt::Key_Escape){
        Session::get()->controller()->cancel();
        Session::get()->deactivate();
    }


    if (key == Qt::Key_Return){
        Session::get()->controller()->apply();
    }

    glWidget->keyPressEvent(event);
    glWidget->updateGL();
}

//slots

void MainWindow::newFile(){
    Session::get()->reset();
}

void MainWindow::openFile(){
    QString fname = QFileDialog::getOpenFileName(this,"Open");
    Session::get()->open(fname.toUtf8().constData());
}

void MainWindow::saveFile(){

}

void MainWindow::saveFileAs(){
    QString fname = QFileDialog::getSaveFileName(this,"Save As");
    Session::get()->saveAs(fname.toUtf8().constData());
}

void MainWindow::exportINP(){
    QString fname = QFileDialog::getSaveFileName(this,"Export INP");
    Session::get()->exportShape(fname.toUtf8().constData(), 0);
}

void MainWindow::undo(){
    Session::get()->undo();
}

void MainWindow::redo(){
    Session::get()->redo();
}

void MainWindow::viewAttr(){
    bool ison = true; //= viewAttrAct->isChecked();
    attrDockWidget->setVisible(ison);
    attrStackedWidget->setVisible(ison);
    if (ison)
        setAttrWidget((void*)Session::get()->theShape());
}

void MainWindow::viewOptions(){
    bool ison = true; //viewOptionsAct->isChecked();
    optionsDockWidget->setVisible(ison);
    optionsStackedWidget->setVisible(ison);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Shady"), tr("<b>Shady</b> is an application in progress that implements theoretical framework developed atTexas A&M University for rendering 2D shapes as if they are part of 3D scene."));
}

void MainWindow::flipDrag()
{
     glWidget->setRender(DRAG_ON, dragAct->isChecked());
     if (dragAct->isChecked())
         setOptionsWidget(Options::DRAG);
}


void MainWindow::flipRender()
{
//    shadingOnAct->setChecked(previewAct->isChecked());
//    ambientOnAct->setChecked(previewAct->isChecked());
    shadowOnAct->setChecked(previewAct->isChecked());
    previewOnAct->setChecked(previewAct->isChecked());
//    emit(shadingOnAct->triggered());
//    emit(ambientOnAct->triggered());
    emit(shadowOnAct->triggered());
    emit(previewOnAct->triggered());
    glWidget->setRender(PREVIEW_ON, previewAct->isChecked());
}



void MainWindow::unselectDrag()
{
    dragAct->setChecked(false);
    glWidget->setRender(DRAG_ON, false);
}

void MainWindow::toggleNormals(){
    glWidget->setRender(NORMALS_ON, normalsOnAct->isChecked());
}

void MainWindow::togglePathces(){
    glWidget->setRender(WIREFRAME_ON, patchesOnAct->isChecked());
}

void MainWindow::toggleShading(){
    glWidget->setRender(SHADING_ON, shadingOnAct->isChecked());
}

void MainWindow::toggleAmbient(){
    glWidget->setRender(AMBIENT_ON, ambientOnAct->isChecked());
}

void MainWindow::toggleShadow(){
    glWidget->setRender(SHADOWS_ON, shadowOnAct->isChecked());
}

void MainWindow::togglePreview(){
    glWidget->setRender(PREVIEW_ON, true);//previewAct->isChecked());//previewOnAct->isChecked());
    //glWidget->updateGL();
}

void MainWindow::toggleLockShape(){
    Shape_p shape = Session::get()->theShape();
    if (!shape)
        return;
    shape->flipLock();
    glWidget->updateGL();
}

void MainWindow::parentShape(){
    Shape_p shape = Session::get()->theShape();
     if (!shape)
         return;


     glWidget->updateGL();
}

void MainWindow::groupShape(){
    Shape_p shape = Session::get()->theShape();
     if (!shape)
         return;
}

void MainWindow::moveShapeToFront(){
    Session::get()->moveActiveUp();
}

void MainWindow::moveShapeToBack(){
    Session::get()->moveActiveDown();
}

void MainWindow::sendShapeBack(){
    Session::get()->sendActiveBack();
}

void MainWindow::sendShapeFront(){
    Session::get()->sendActiveFront();
}

void MainWindow::transformShape(){
    Shape_p shape = Session::get()->theShape();
    if (!shape)
        return;
    Session::get()->controller()->startTransform(shape);
    glWidget->updateGL();
}

void MainWindow::deleteShape(){
   Session::get()->removeShape(Session::get()->theShape());
}

void MainWindow::insertEllipse(){
    Session::get()->insertShape(new EllipseShape());
}

#ifndef MODELING_MODE
void updateGLSLLight(double x, double y, double z){
    MainWindow::glWidget->updateGLSLLight(x,y,z);
}
#endif
