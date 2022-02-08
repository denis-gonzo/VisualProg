#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    model = new PlantsModel(this);

    /* load default data */
    //loadFromFile("./save.json");

    ui->treeView->setModel(model);
    ui->treeView->setColumnHidden(1,true);
    ui->treeView->setColumnHidden(2,true);

    /* connect all activities */
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &Widget::updateSideView);
    connect(ui->edit_pushButton, &QPushButton::clicked,
            this, &Widget::updateInfo);
    connect(ui->img_pushButton, &QPushButton::clicked,
            this, &Widget::changeImage);
    connect(ui->load_pushButton, &QPushButton::clicked,
            this, &Widget::openFile);
    connect(ui->save_pushButton, &QPushButton::clicked,
            this, &Widget::saveFile);

    connect(ui->del_pushButton, &QPushButton::clicked,
            this, &Widget::removeRow);
    connect(ui->add_pushButton, &QPushButton::clicked,
            this, &Widget::insertRow);

    connect(ui->cut_pushButton, &QPushButton::clicked,
            this, &Widget::cutAndPaste);

    updateSideView();
}

Widget::~Widget()
{
    delete ui;
    delete model;
}

void Widget::updateSideView()
{
    bool hasSelection = !ui->treeView->selectionModel()->selection().isEmpty();
    ui->del_pushButton->setEnabled(hasSelection);
    ui->add_pushButton->setEnabled(hasSelection);
    ui->edit_pushButton->setEnabled(hasSelection);
    ui->img_pushButton->setEnabled(hasSelection);
    ui->cut_pushButton->setEnabled(hasSelection);
    ui->cut_pushButton->setText(model->hasCut()?"Вставить":"Вырезать");

    bool hasCurrent = ui->treeView->selectionModel()->currentIndex().isValid();
    if(hasCurrent){
        QModelIndex currentIndex = ui->treeView->selectionModel()->currentIndex();
        ui->treeView->closePersistentEditor(currentIndex);
        /* Set Name */
        ui->labelName->setText( currentIndex.data().toString() );
        /* Set Info */
        QModelIndex index = currentIndex.siblingAtColumn(1);
        ui->infoText->setText(index.data().toString() );
        /* Set Image */
        index = currentIndex.siblingAtColumn(2);
        QByteArray base64Data = QByteArray::fromBase64( index.data().toByteArray() );
        QImage img = QImage::fromData(base64Data);
        if(!img.isNull()){
            int w = ui->labelImage->width()<img.width()?ui->labelImage->width():img.width();
            ui->labelImage->setPixmap(QPixmap::fromImage(img.scaled(w,w,Qt::AspectRatioMode::KeepAspectRatio)));
        } else {
            ui->labelImage->setText("Изображение");
        }
        /* Set parent names */
        QList<QLabel*>labels;
        labels << ui->labelDomainData
               << ui->labelKingdomeData
               << ui->labelPartionData
               << ui->labelClassData
               << ui->labelOrderData
               << ui->labelFamilyData;
        QStringList pname = model->parentList(currentIndex);
        for( int i=0; i<labels.size(); i++ ) {
            if(pname.size()>i)
                labels.at(i)->setText(pname.at(i));
            else
                labels.at(i)->setText("...");
        }
        /* Manage buttons */
        int lvl = model->level(currentIndex);
        ui->add_pushButton->setEnabled(1 <= lvl && lvl < 6);
        ui->del_pushButton->setEnabled(lvl >= 2);
        if(model->hasCut())
            ui->cut_pushButton->setEnabled(1 <= lvl && lvl < 6);
        else
            ui->cut_pushButton->setEnabled(lvl >= 2);

    } else {
        /* clear data */
        ui->labelName->setText( "Название" );
        ui->infoText->setText("" );
        ui->labelImage->setText("Изображение");
        QList<QLabel*>labels;
        labels << ui->labelDomainData
               << ui->labelKingdomeData
               << ui->labelPartionData
               << ui->labelClassData
               << ui->labelOrderData
               << ui->labelFamilyData;
        for( int i=0; i<labels.size(); i++ )
            labels.at(i)->setText("...");
    }
}
void Widget::updateInfo()
{
    QModelIndex index = ui->treeView->currentIndex().siblingAtColumn(1);
    model->setData(index, ui->infoText->toPlainText());
}
void Widget::changeImage()
{
    QString filename = QFileDialog::getOpenFileName(this,
                         "Открыть изображение","./",
                         "Изображения (*.jpeg *.jpg *.png *.bmp)");
    if(!QFile::exists(filename)){
        return;
    }
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly) ){
        qDebug() << "Cant open file!!!";
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QModelIndex index = ui->treeView->currentIndex().siblingAtColumn(2);

    model->setData(index,data.toBase64());

    updateSideView();

}
void Widget::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this,
                         "Открыть справочник",
                         "./", "Json (*.json)");
    loadFromFile(filename);
    updateSideView();
}
void Widget::loadFromFile(const QString &filename)
{
    if(!QFile::exists(filename)){
        return;
    }
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly) ){
        qDebug() << "Cant open file!!!";
        return;
    }

    QByteArray data = file.readAll();
    file.close();
    model->loadFromJson(data);

}
void Widget::saveFile()
{
    QString filename = QFileDialog::getSaveFileName(this,
                         "Сохранить справочник как Json",
                         "./save.json", "Json (*.json)");
    QFile saveFile(filename);
    if(!saveFile.open(QIODevice::WriteOnly) ){
        qDebug() << "Cant open file!!!";
        return;
    }

    saveFile.write(model->saveToJson());
    saveFile.close();
}
void Widget::removeRow()
{
    QModelIndex index = ui->treeView->currentIndex();
    if ( model->removeRow(index.row(), index.parent() ) )
        updateSideView();

}
void Widget::insertRow()
{
    QModelIndex index = ui->treeView->currentIndex();
    if ( model->insertRow(0, index ) ) {
        ui->treeView->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                                QItemSelectionModel::ClearAndSelect);
        updateSideView();
    }
}
void Widget::cutAndPaste()
{
    QModelIndex index = ui->treeView->currentIndex();
    if(!model->hasCut())
        model->cutItem(index);
    else
    {
        model->pasteItem(index);
        ui->treeView->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                                QItemSelectionModel::ClearAndSelect);
    }
    updateSideView();
}
