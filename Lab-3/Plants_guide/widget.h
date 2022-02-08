#pragma once

#include <QWidget>

#include "plantsmodel.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void updateSideView();
    void updateInfo();
    void changeImage();
    void openFile();
    void saveFile();
    void removeRow();
    void insertRow();
    void cutAndPaste();

private:
    Ui::Widget *ui;
    PlantsModel *model;

    void loadFromFile(const QString &filename);

};
