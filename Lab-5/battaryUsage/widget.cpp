#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->viewComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Widget::onSetView);

    connect(ui->percSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            ui->batteryWidget, &BatteryWidget::setPercent);
    connect(ui->statusComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            ui->batteryWidget, &BatteryWidget::setStatus);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::onSetView(int view)
{
    ui->batteryWidget->setView(static_cast<BatteryWidget::View> (view));
}
