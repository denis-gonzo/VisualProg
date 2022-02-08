#include "batterywidget.h"
#include "ui_form.h"

BatteryWidget::BatteryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form),
    m_percent{0}, m_status{Unknown}, m_view{All}

{
    ui->setupUi(this);
    updateView();
}

BatteryWidget::~BatteryWidget()
{
    delete ui;
}
void BatteryWidget::setView(View view)
{
    if(m_view != view){
        m_view = view;
        updateView();
    }
}
void BatteryWidget::setPercent(int percent)
{
    if(m_percent != percent){
        m_percent = percent;
        updateView();
    }
}
void BatteryWidget::setStatus(int status)
{
    if(m_status != status){
        m_status = static_cast<Status>(status);
        updateView();
    }
}
void BatteryWidget::updateView()
{
    ui->labelPercent->setText(QString("%1%").arg(m_percent));
    switch (m_status) {
    case Unknown:
        ui->labelImage->setPixmap(QPixmap(":/battery/battery-unknown.svg"));
        ui->labelStatus->setText("- Неизвестно");
        break;
    case Discharging:
        ui->labelStatus->setText("- Разряжается");
        if(m_percent<=10)
            ui->labelImage->setPixmap(QPixmap(":/battery/battery-alert.svg"));
        else if(m_percent<=20)
            ui->labelImage->setPixmap(QPixmap(":/battery/battery-20.svg"));
        else if(m_percent<=30)
            ui->labelImage->setPixmap(QPixmap(":/battery/battery-30.svg"));
        else if(m_percent<=50)
            ui->labelImage->setPixmap(QPixmap(":/battery/battery-50.svg"));
        else if(m_percent<=80)
            ui->labelImage->setPixmap(QPixmap(":/battery/battery-80.svg"));
        else
            ui->labelImage->setPixmap(QPixmap(":/battery/battery-full.svg"));
        break;
    case Charging:
        ui->labelStatus->setText("- Заряжается");
        if(m_percent<=20)
            ui->labelImage->setPixmap(QPixmap(":/battery/battery-charging-20.svg"));
        else if(m_percent<=30)
            ui->labelImage->setPixmap(QPixmap(":/battery/battery-charging-30.svg"));
        else if(m_percent<=50)
            ui->labelImage->setPixmap(QPixmap(":/battery/battery-charging-50.svg"));
        else if(m_percent<=80)
            ui->labelImage->setPixmap(QPixmap(":/battery/battery-charging-80.svg"));
        else
            ui->labelImage->setPixmap(QPixmap(":/battery/battery-charging-full.svg"));
        break;
    case Full:
        ui->labelStatus->setText("- Заряжена");
        ui->labelImage->setPixmap(QPixmap(":/battery/battery-full.svg"));
        break;
    }
    switch (m_view) {
    case All:
        ui->labelImage->setVisible(true);
        ui->labelPercent->setVisible(true);
        ui->labelStatus->setVisible(true);
        break;
    case IconAndPercent:
        ui->labelImage->setVisible(true);
        ui->labelPercent->setVisible(true);
        ui->labelStatus->setVisible(false);
        break;
    case IconOnly:
        ui->labelImage->setVisible(true);
        ui->labelPercent->setVisible(false);
        ui->labelStatus->setVisible(false);
        break;
    }
}
