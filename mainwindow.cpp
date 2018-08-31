/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
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
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
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

#include "arrow.h"
#include "diagramitem.h"
#include "diagramscene.h"
#include "diagramtextitem.h"
#include "diagramellipseitem.h"
#include "mainwindow.h"
#include "diagramview.h"
#include "diagramdashitem.h"
#include <QtXlsx>
#include <QtWidgets>

const int InsertTextButton = 10;
int cnt = 0;
//! [0]
MainWindow::MainWindow()
{
    createActions();
    createToolBox();
    createMenus();
    createStatusBar();
    createInfoWidget();
    scene = new DiagramScene(itemMenu, this, this);
    scene->setSceneRect(QRectF(0, 0, 9000, 9000));
    connect(scene, SIGNAL(itemInserted(DiagramItem*)),
            this, SLOT(itemInserted(DiagramItem*)));
    connect(scene, SIGNAL(ellipseItemInserted(DiagramEllipseItem*)),
            this, SLOT(ellipseItemInserted(DiagramEllipseItem*)));
    connect(scene, SIGNAL(textInserted(QGraphicsTextItem*)),
            this, SLOT(textInserted(QGraphicsTextItem*)));
    connect(scene, SIGNAL(itemSelected(QGraphicsItem*)),
            this, SLOT(itemSelected(QGraphicsItem*)));
    connect(scene, SIGNAL(itemChange()),
            this, SLOT(beModifed()));
    connect(this, SIGNAL(itemChanged()),
            this, SLOT(beModifed()));
    connect(scene, SIGNAL(collidingOccured()),
            this, SLOT(collidingOccured()));
    connect(scene, SIGNAL(widgetClear()),
            this, SLOT(widgetClear()));
    connect(infoWidget->itemDelegate(), SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),
            this, SLOT(infoChange()));
    connect(infoWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this, SLOT(getCurItemChange(QTableWidgetItem*)));
    createToolbars();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(toolBox);
    view = new DiagramView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    layout->addWidget(view);
    layout->addWidget(infoWidget);


    QWidget *widget = new QWidget;
    widget->setLayout(layout);


    setCentralWidget(widget);
    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);
}
//! [0]

//! [1]
void MainWindow::backgroundButtonGroupClicked(QAbstractButton *button)
{
    QList<QAbstractButton *> buttons = backgroundButtonGroup->buttons();
    foreach (QAbstractButton *myButton, buttons) {
        if (myButton != button)
            button->setChecked(false);
    }
    QString text = button->text();
    if (text == tr("Blue Grid"))
        scene->setBackgroundBrush(QPixmap(":/images/background1.png"));
    else if (text == tr("White Grid"))
        scene->setBackgroundBrush(QPixmap(":/images/background2.png"));
    else if (text == tr("Gray Grid"))
        scene->setBackgroundBrush(QPixmap(":/images/background3.png"));
    else
        scene->setBackgroundBrush(QPixmap(":/images/background4.png"));

    scene->update();
    view->update();
}
//! [1]

//! [2]
void MainWindow::buttonGroupClicked(int id)
{
    QList<QAbstractButton *> buttons = buttonGroup->buttons();
    foreach (QAbstractButton *button, buttons) {
        if (buttonGroup->button(id) != button)
            button->setChecked(false);
    }
    if (id == InsertTextButton) {
        scene->setMode(DiagramScene::InsertText);
    } else {
        scene->setItemType(DiagramItem::DiagramType(id));
        scene->setMode(DiagramScene::InsertItem);
    }
}
//! [2]

//! [3]
void MainWindow::deleteItem()
{
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == Arrow::Type) {
            scene->removeItem(item);
            Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            delete item;
        }
    }

    foreach (QGraphicsItem *item, scene->selectedItems()) {
         if (item->type() == DiagramItem::Type)
             qgraphicsitem_cast<DiagramEllipseItem *>(item)->removeArrows();
         DiagramEllipseItem *_item = qgraphicsitem_cast<DiagramEllipseItem *>(item);




         scene->removeItem(item);
         DiagramScene::pointer2Type.remove(item);
         delete item;

     }
    emit itemChanged();
}
//! [3]

//! [4]
void MainWindow::pointerGroupClicked(int)
{
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [4]

//! [5]
void MainWindow::bringToFront()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() >= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
}
//! [5]

//! [6]
void MainWindow::sendToBack()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() <= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
}
//! [6]

//! [7]
void MainWindow::itemInserted(DiagramItem *item)
{
    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    buttonGroup->button(int(item->diagramType()))->setChecked(false);
}

void MainWindow::ellipseItemInserted(DiagramEllipseItem *item)
{
    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    buttonGroup->button(int(DiagramItem::Circle))->setChecked(false);
}
//! [7]

//! [8]
void MainWindow::textInserted(QGraphicsTextItem *)
{
    buttonGroup->button(InsertTextButton)->setChecked(false);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [8]

//! [9]
void MainWindow::currentFontChanged(const QFont &)
{
    handleFontChange();
}
//! [9]

//! [10]
void MainWindow::fontSizeChanged(const QString &)
{
    handleFontChange();
}
//! [10]

//! [11]
void MainWindow::sceneScaleChanged(const QString &scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = view->matrix();
    view->resetMatrix();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
}
//! [11]

//! [12]
void MainWindow::textColorChanged()
{
    textAction = qobject_cast<QAction *>(sender());
    fontColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/textpointer.png",
                                     qvariant_cast<QColor>(textAction->data())));
    textButtonTriggered();
}
//! [12]

//! [13]
void MainWindow::itemColorChanged()
{
    fillAction = qobject_cast<QAction *>(sender());
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/floodfill.png",
                                     qvariant_cast<QColor>(fillAction->data())));
    fillButtonTriggered();
}
//! [13]

//! [14]
void MainWindow::lineColorChanged()
{
    lineAction = qobject_cast<QAction *>(sender());
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/linecolor.png",
                                     qvariant_cast<QColor>(lineAction->data())));
    lineButtonTriggered();
}
//! [14]

//! [15]
void MainWindow::textButtonTriggered()
{
    scene->setTextColor(qvariant_cast<QColor>(textAction->data()));
}
//! [15]

//! [16]
void MainWindow::fillButtonTriggered()
{
    scene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
}
//! [16]

//! [17]
void MainWindow::lineButtonTriggered()
{
    scene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
}
//! [17]

//! [18]
void MainWindow::handleFontChange()
{
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());

    scene->setFont(font);
}
//! [18]

//! [19]
void MainWindow::itemSelected(QGraphicsItem *item)
{
    DiagramTextItem *textItem =
    qgraphicsitem_cast<DiagramTextItem *>(item);

    QFont font = textItem->font();
    fontCombo->setCurrentFont(font);
    fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    boldAction->setChecked(font.weight() == QFont::Bold);
    italicAction->setChecked(font.italic());
    underlineAction->setChecked(font.underline());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(maybeSave()){
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile()
{
    if(maybeSave()) {
        scene->clear();
        DiagramScene::pointer2Type.clear();
        DiagramScene::id2ItemPointer.clear();
        setCurrentFile(QString());
    }
}

void MainWindow::open()
{
    if(maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open a new map"), "C:/", tr("Excel files(*xlsx)"));
        if(!fileName.isEmpty())
            DiagramScene::pointer2Type.clear();
            DiagramScene::id2ItemPointer.clear();
            loadFile(fileName);
            DiagramEllipseItem::base_id = 0;
            DiagramDashItem::base_id = 0;

    }
}

bool MainWindow::save()
{
    if(curFile.isEmpty()) {
        return saveAs();
    } else {

        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if(dialog.exec() != QDialog::Accepted){
        return false;
    }
    return saveFile(dialog.selectedFiles().first());
}

bool MainWindow::maybeSave()
{
    if(getIsModified() == false)
        return true;
    const QMessageBox::StandardButton ret
            = QMessageBox::warning(this, tr("Application"),
                                   tr("The map has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setModifed(false);
    setWindowModified(false);
    QString shownFile = curFile;
    if(curFile.isEmpty())
        shownFile = "untitled.xlsx";
    setWindowFilePath(shownFile);
}

void MainWindow::loadFile(const QString &fileName)
{

    QXlsx::Document xlsx(fileName);
    int row = xlsx.dimension().rowCount();
    int col = xlsx.dimension().columnCount();
    int x, y, id, belong_wid, wid_type, radius, facade;
    QString name;
    for(int i = 2; i <= row; ++i){
        id = xlsx.cellAt(i,1)->value().toInt();
        x = xlsx.cellAt(i,2)->value().toInt();
        y = xlsx.cellAt(i,3)->value().toInt();
        belong_wid = xlsx.cellAt(i,4)->value().toInt();
        name = xlsx.cellAt(i,5)->value().toString();
        wid_type = xlsx.cellAt(i,6)->value().toInt();
        radius = xlsx.cellAt(i,7)->value().toInt();
        facade = xlsx.cellAt(i,8)->value().toInt();

        DiagramEllipseItem *item = new DiagramEllipseItem(scene->getMyItemMenu(), this, id, QRectF(-radius, -radius, radius*2,radius*2), belong_wid, name, wid_type, radius, facade);

        item->setBrush(scene->getItemColor());
        scene->addItem(item);
        item->setPos(QPointF(x, y));
        DiagramScene::pointer2Type[item] = 1;
        DiagramScene::id2ItemPointer[id] = item;
        emit ellipseItemInserted(item);
    }



    QList<QGraphicsItem*> list = scene->items();

    foreach(QGraphicsItem *_item, list){
        DiagramEllipseItem *item = qgraphicsitem_cast<DiagramEllipseItem*>(_item);

        int myBelong_wid = item->getMyBelong_wid();
        if(myBelong_wid == 0) continue;

        int myId = item->getMyId();

        DiagramEllipseItem *primaryItem = DiagramScene::id2ItemPointer[myBelong_wid];
        primaryItem->setMySecondId(myId);

        qreal radius = qSqrt(qPow((item->pos().x() - primaryItem->pos().x()), 2) + qPow((item->pos().y() - primaryItem->pos().y()),2));


        DiagramDashItem *dashItem = new DiagramDashItem(QRectF(-radius,-radius,radius*2,radius*2), radius, myBelong_wid, myId);
        DiagramScene::pointer2Type[dashItem] = 2;
        scene->addItem(dashItem);
        dashItem->setPos(primaryItem->pos());
    }
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QXlsx::Document xlsx;
    QList<QGraphicsItem*> list = scene->items();

    QPointF pos;
    int row = 1, col = 1;

    xlsx.write(row, col, QString("ID"));
    xlsx.write(row, col+1, QString("坐标X"));
    xlsx.write(row, col+2, QString("坐标Y"));
    xlsx.write(row, col+3, QString("归属"));
    xlsx.write(row, col+4, QString("名称"));
    xlsx.write(row, col+5, QString("类型"));
    xlsx.write(row, col+6, QString("范围"));
    xlsx.write(row, col+7, QString("外观"));

    row = row + 1;

    foreach(QGraphicsItem* _item, list){
        if(DiagramScene::pointer2Type[_item] == 2) continue;

        DiagramEllipseItem* item = qgraphicsitem_cast<DiagramEllipseItem*>(_item);
        pos = item->scenePos();

        xlsx.write(row, col, item->getMyId());

        xlsx.write(row, col+1, pos.x());
        xlsx.write(row, col+2, pos.y());
        xlsx.write(row, col+3, item->getMyBelong_wid());

        xlsx.write(row, col+4, item->getMyName());
        xlsx.write(row, col+5, item->getMyWid_tpye());
        xlsx.write(row, col+6, item->getRadius());
        xlsx.write(row, col+7, item->getMyFacade());
        row = row + 1;

    }

    xlsx.saveAs(fileName);
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("Map saved"), 2000);

    return true;
}
//! [19]

//! [20]
void MainWindow::about()
{
    QMessageBox::about(this, tr("About this Application"),
                       tr("A game map editor based on <b> the offical examples of Qt </b>."
                          " Appreciate it very much!"));
}
//! [20]

//! [21]
//!

void MainWindow::createInfoWidget()
{
    infoWidget = new QTableWidget(8,2);
    infoWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Name"));
    infoWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Value"));

    int row = infoWidget->rowCount();

    QList<QStringList> rows = QList<QStringList>()
        << (QStringList() << "Id" << "")
        << (QStringList() << "坐标X" << "")
        << (QStringList() << "坐标Y" << "")
        << (QStringList() << "归属" << "")
        << (QStringList() << "名称" << "")
        << (QStringList() << "类型" << "")
        << (QStringList() << "范围" << "")
        << (QStringList() << "外观" << "");

    for(int i = 0; i < row; ++i){

            QTableWidgetItem *item_0 = new QTableWidgetItem(rows.at(i).at(0));
            item_0->setFlags(item_0->flags() & ~Qt::ItemIsEditable);
            infoWidget->setItem(i,0, item_0);

            QTableWidgetItem *item_1 = new QTableWidgetItem(rows.at(i).at(1));
            item_1->setFlags(item_1->flags() & ~Qt::ItemIsEditable);
            infoWidget->setItem(i,1,new QTableWidgetItem(rows.at(i).at(1)));
    }

    infoWidget->verticalHeader()->hide();
    infoWidget->horizontalHeader()->setStretchLastSection(true);
    infoWidget->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
    infoWidget->resizeColumnsToContents();
    infoWidget->setMaximumWidth(infoWidget->contentsRect().width());
}

void MainWindow::createToolBox()
{
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);
    connect(buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(buttonGroupClicked(int)));
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(createCellWidget(tr("Conditional"), DiagramItem::Conditional), 0, 0);
    layout->addWidget(createCellWidget(tr("Process"), DiagramItem::Step),0, 1);
    layout->addWidget(createCellWidget(tr("Input/Output"), DiagramItem::Io), 1, 0);
    layout->addWidget(createCellWidget(tr("Circle"), DiagramItem::Circle), 2, 0);
//! [21]

    QToolButton *textButton = new QToolButton;
    textButton->setCheckable(true);
    buttonGroup->addButton(textButton, InsertTextButton);
    textButton->setIcon(QIcon(QPixmap(":/images/textpointer.png")));
    textButton->setIconSize(QSize(50, 50));
    QGridLayout *textLayout = new QGridLayout;
    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
    QWidget *textWidget = new QWidget;
    textWidget->setLayout(textLayout);
    layout->addWidget(textWidget, 1, 1);

    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);

    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    backgroundButtonGroup = new QButtonGroup(this);
    connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(backgroundButtonGroupClicked(QAbstractButton*)));

    QGridLayout *backgroundLayout = new QGridLayout;
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Blue Grid"),
                                                           ":/images/background1.png"), 0, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("White Grid"),
                                                           ":/images/background2.png"), 0, 1);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"),
                                                           ":/images/background3.png"), 1, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"),
                                                           ":/images/background4.png"), 1, 1);

    backgroundLayout->setRowStretch(2, 10);
    backgroundLayout->setColumnStretch(2, 10);

    QWidget *backgroundWidget = new QWidget;
    backgroundWidget->setLayout(backgroundLayout);


//! [22]
    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, tr("Basic Flowchart Shapes"));
    toolBox->addItem(backgroundWidget, tr("Backgrounds"));
}
//! [22]

//! [23]
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}
void MainWindow::createActions()
{
    toFrontAction = new QAction(QIcon(":/images/bringtofront.png"),
                                tr("Bring to &Front"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Bring item to front"));
    connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));
//! [23]

    sendBackAction = new QAction(QIcon(":/images/sendtoback.png"), tr("Send to &Back"), this);
    sendBackAction->setShortcut(tr("Ctrl+T"));
    sendBackAction->setStatusTip(tr("Send item to back"));
    connect(sendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));

    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    const QIcon newFileIcon = QIcon::fromTheme("document-new", QIcon("C:/Qt/Qt5.11.1/Examples/Qt-5.11.1/widgets/mainwindows/application/images/new.png"));
    newFileAction = new QAction(newFileIcon, tr("&New..."), this);
    newFileAction->setShortcut(QKeySequence::New);
    newFileAction->setStatusTip(tr("Create a new file"));
    connect(newFileAction, SIGNAL(triggered()), this, SLOT(newFile()));

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon("C:/Qt/Qt5.11.1/Examples/Qt-5.11.1/widgets/mainwindows/application/images/open.png"));
    openAction = new QAction(openIcon, tr("&Open..."), this);
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon("C:/Qt/Qt5.11.1/Examples/Qt-5.11.1/widgets/mainwindows/application/images/save.png"));
    saveAction = new QAction(saveIcon, tr("&Save"), this);
    saveAction->setShortcuts(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the document to disk"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
    saveAsAction = new QAction(saveAsIcon, tr("Save &As.."), this);
    saveAsAction->setShortcuts(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit Scenediagram example"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    boldAction = new QAction(tr("Bold"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap(":/images/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    italicAction = new QAction(QIcon(":/images/italic.png"), tr("Italic"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    underlineAction = new QAction(QIcon(":/images/underline.png"), tr("Underline"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("F1"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

//! [24]
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newFileAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);
}
//! [24]

//! [25]
void MainWindow::createToolbars()
{
//! [25]
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(toFrontAction);
    editToolBar->addAction(sendBackAction);

    fontCombo = new QFontComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(currentFontChanged(QFont)));

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    QIntValidator *validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(fontSizeChanged(QString)));

    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()), Qt::black));
    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon(":/images/textpointer.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, SIGNAL(clicked()),
            this, SLOT(textButtonTriggered()));

//! [26]
    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::white));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/floodfill.png", Qt::white));
    connect(fillColorToolButton, SIGNAL(clicked()),
            this, SLOT(fillButtonTriggered()));
//! [26]

    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()), Qt::black));
    lineAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/linecolor.png", Qt::black));
    connect(lineColorToolButton, SIGNAL(clicked()),
            this, SLOT(lineButtonTriggered()));

    textToolBar = addToolBar(tr("Font"));
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);

    colorToolBar = addToolBar(tr("Color"));
    colorToolBar->addWidget(fontColorToolButton);
    colorToolBar->addWidget(fillColorToolButton);
    colorToolBar->addWidget(lineColorToolButton);

    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));
    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/linepointer.png"));

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(pointerGroupClicked(int)));

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);
    connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(sceneScaleChanged(QString)));

    pointerToolbar = addToolBar(tr("Pointer type"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);
    pointerToolbar->addWidget(sceneScaleCombo);
//! [27]
}
//! [27]

//! [28]
QWidget *MainWindow::createBackgroundCellWidget(const QString &text, const QString &image)
{
    QToolButton *button = new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    backgroundButtonGroup->addButton(button);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
//! [28]

//! [29]
QWidget *MainWindow::createCellWidget(const QString &text, DiagramItem::DiagramType type)
{

    if(type != DiagramItem::Circle)
    {
        DiagramItem item(type, itemMenu);
        QIcon icon(item.image());

        QToolButton *button = new QToolButton;
        button->setIcon(icon);
        button->setIconSize(QSize(50, 50));
        button->setCheckable(true);
        buttonGroup->addButton(button, int(type));

        QGridLayout *layout = new QGridLayout;
        layout->addWidget(button, 0, 0, Qt::AlignHCenter);
        layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

        QWidget *widget = new QWidget;
        widget->setLayout(layout);

        return widget;
    }
    else
    {
        DiagramEllipseItem item(itemMenu);
        QIcon icon(item.image());

        QToolButton *button = new QToolButton;
        button->setIcon(icon);
        button->setIconSize(QSize(50, 50));
        button->setCheckable(true);
        buttonGroup->addButton(button, int(type));

        QGridLayout *layout = new QGridLayout;
        layout->addWidget(button, 0, 0, Qt::AlignHCenter);
        layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

        QWidget *widget = new QWidget;
        widget->setLayout(layout);

        return widget;
    }
}
//! [29]

//! [30]
QMenu *MainWindow::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue")
          << tr("yellow");

    QMenu *colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()), this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor)
            colorMenu->setDefaultAction(action);
    }
    return colorMenu;
}
//! [30]

//! [31]
QIcon MainWindow::createColorToolButtonIcon(const QString &imageFile, QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    // Draw icon centred horizontally on button.
    QRect target(4, 0, 42, 43);
    QRect source(0, 0, 42, 43);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}
//! [31]

//! [32]
QIcon MainWindow::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}
//! [32]
void MainWindow::setModifed(bool modified)
{
    isModified = modified;
}
bool MainWindow::getIsModified()
{
    return isModified;
}
void MainWindow::beModifed()
{
     isModified = true;
     setWindowModified(isModified);
}

void MainWindow::getCurItemChange(QTableWidgetItem* item)
{
    curItemChanged = item;
}
void MainWindow::infoChange()
{
    QList<QGraphicsItem*> list = scene->selectedItems();
    if(list.empty()){
            const QMessageBox::StandardButton ret
                    = QMessageBox::warning(this, tr("Application"),
                                           tr("You have not selected any item\n"));

        curItemChanged->setData(Qt::DisplayRole,"");
    } else {
        DiagramEllipseItem *ellipseItem = qgraphicsitem_cast<DiagramEllipseItem*>(list.at(0));
        int row = curItemChanged->row();
        switch(row){
            case 0:
                    ellipseItem->setMyId(curItemChanged->data(Qt::EditRole).toInt());
            break;
            case 1:
                    ellipseItem->setPos(curItemChanged->data(Qt::EditRole).toInt(), int(ellipseItem->pos().y()));
            break;
            case 2:
                    ellipseItem->setPos(int(ellipseItem->pos().x()), curItemChanged->data(Qt::EditRole).toInt());
            break;
            case 3:
                    ellipseItem->setMyBelong_wid(curItemChanged->data(Qt::EditRole).toInt());
            break;
            case 4:
                    ellipseItem->setMyName(curItemChanged->data(Qt::EditRole).toString());
            break;
            case 5:
                    ellipseItem->setWid_type(curItemChanged->data(Qt::EditRole).toInt());
            break;
            case 6:
                    ellipseItem->setRadius(curItemChanged->data(Qt::EditRole).toReal());
            break;
            case 7:
                    ellipseItem->setMyFacade(curItemChanged->data(Qt::EditRole).toInt());
            break;
            default:
            ;
        }
    }
}

void MainWindow::widgetClear()
{
    int row = infoWidget->rowCount();

    for(int i = 0; i < row; ++i){
        QTableWidgetItem *widgetItem = infoWidget->item(i,1);
        switch(i){
            case 0:
                    widgetItem->setData(Qt::DisplayRole,"");
            break;
            case 1:
                    widgetItem->setData(Qt::DisplayRole,"");
            break;
            case 2:
                    widgetItem->setData(Qt::DisplayRole,"");
            break;
            case 3:
                    widgetItem->setData(Qt::DisplayRole,"");
            break;
            case 4:
                    widgetItem->setData(Qt::DisplayRole,"");
            break;
            case 5:
                    widgetItem->setData(Qt::DisplayRole,"");
            break;
            case 6:
                    widgetItem->setData(Qt::DisplayRole,"");
            break;
            case 7:
                    widgetItem->setData(Qt::DisplayRole,"");
            break;
            default:
            ;
        }
    }

}
void MainWindow::collidingOccured()
{

    const QMessageBox::StandardButton ret
            = QMessageBox::warning(this, tr("Application"),
                                   tr("The item you just put has collided with the other item(s)\n"));
}

void MainWindow::infoUpdate(DiagramEllipseItem* item)
{
    QPointF pos = item->pos();

    int id = item->getMyId();
    int x = pos.x();
    int y = pos.y();
    int belong_wid = item->getMyBelong_wid();
    QString name = item->getMyName();
    int wid_type = item->getMyWid_tpye();
    int radius = item->getMyRectF().width()/2;
    int facade = item->getMyFacade();

    int row = infoWidget->rowCount();

    for(int i = 0; i < row; ++i){
        QTableWidgetItem *widgetItem = infoWidget->item(i,1);
        switch(i){
            case 0:
                    widgetItem->setData(Qt::DisplayRole,id);
            break;
            case 1:
                    widgetItem->setData(Qt::DisplayRole,x);
            break;
            case 2:
                    widgetItem->setData(Qt::DisplayRole,y);
            break;
            case 3:
                    widgetItem->setData(Qt::DisplayRole,belong_wid);
            break;
            case 4:
                    widgetItem->setData(Qt::DisplayRole,name);
            break;
            case 5:
                    widgetItem->setData(Qt::DisplayRole,wid_type);
            break;
            case 6:
                    widgetItem->setData(Qt::DisplayRole,radius);
            break;
            case 7:
                    widgetItem->setData(Qt::DisplayRole,facade);
            break;
            default:
            ;
        }
    }

}
void MainWindow::changeRadius(DiagramEllipseItem *item)
{
    item->setRect(item->getMyRectF());
}
