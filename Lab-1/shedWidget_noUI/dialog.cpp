#include "dialog.h"

#include <QLayout>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    actionLabel = new QLabel("Действие");
    animalLabel = new QLabel("Животное");
    atypeLabel = new QLabel("Вид");
    atypeNameLabel = new QLabel("Капибара");
    timeLabel = new QLabel("Время");
    dateLabel = new QLabel("Дата");

    actionInfoButton = new QPushButton("Информация");
    animInfoButton = new QPushButton("Информация");
    animFindButton = new QPushButton("Поиск");
    applyButton = new QPushButton("Назначить");
    closeButton = new QPushButton("Закрыть");

    actionCombo = new QComboBox();
    actionCombo->addItem("Праздник");
    actionCombo->addItem("Выходной");
    actionCombo->addItem("Встреча");
    animalCombo = new QComboBox();
    animalCombo->addItem("ДжоДжо");
    animalCombo->addItem("Ниф Ниф");
    animalCombo->addItem("Наф Наф");
    animalCombo->addItem("Нуф Нуф");

    calendar = new QCalendarWidget();
    timeedit = new QTimeEdit();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *topRow = new QHBoxLayout;
    QVBoxLayout *leftCol = new QVBoxLayout;
    QHBoxLayout *actRow = new QHBoxLayout;
    actRow->addWidget(actionLabel);
    actRow->addWidget(actionCombo,1);

    leftCol->addLayout(actRow);
    leftCol->addStretch();
    leftCol->addWidget(actionInfoButton);

    QFrame *sep = new QFrame();
    sep->setFrameShape(QFrame::VLine);
    sep->setFrameShadow(QFrame::Sunken);

    QVBoxLayout *rightCol = new QVBoxLayout;
    QGridLayout *animGrid = new QGridLayout;
    animGrid->addWidget(animalLabel,0,0);
    animGrid->addWidget(animalCombo,0,1);
    animGrid->addWidget(atypeLabel,1,0);
    animGrid->addWidget(atypeNameLabel,1,1);
    animGrid->setRowStretch(0,0);
    animGrid->setRowStretch(1,1);

    rightCol->addLayout(animGrid);
    rightCol->addWidget(animInfoButton);
    rightCol->addWidget(animFindButton);

    topRow->addLayout(leftCol,1);
    topRow->addWidget(sep,0);
    topRow->addLayout(rightCol,1);

    QGridLayout *timedateGrid = new QGridLayout;
    timedateGrid->addWidget(timeLabel,0,0);
    timedateGrid->addWidget(timeedit,0,1);
    timedateGrid->addWidget(dateLabel,1,0);
    timedateGrid->addWidget(calendar,1,1);
    timedateGrid->setRowStretch(0,0);
    timedateGrid->setRowStretch(1,1);

    QHBoxLayout *buttonsRow = new QHBoxLayout;
    buttonsRow->addWidget(applyButton);
    buttonsRow->addWidget(closeButton);

    mainLayout->addLayout(topRow,0);
    mainLayout->addLayout(timedateGrid,1);
    mainLayout->addLayout(buttonsRow,0);

    this->setLayout(mainLayout);

    this->setWindowTitle("Расписание (без ui Designer)");
    this->resize(650,500);

    connect(closeButton, &QPushButton::clicked, this, &Dialog::close);
}

Dialog::~Dialog()
{
}

