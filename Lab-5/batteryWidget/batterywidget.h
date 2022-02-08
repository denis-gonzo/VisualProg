#pragma once

#include "batteryWidget_global.h"

#include <QWidget>

namespace Ui {
class Form;
}

class BATTERYWIDGET_EXPORT BatteryWidget  : public QWidget
{
    Q_OBJECT

public:
    explicit BatteryWidget(QWidget *parent = nullptr);
    ~BatteryWidget();

    enum View {
        All = 0,
        IconAndPercent,
        IconOnly
    };

    enum Status {
        Unknown = 0,
        Discharging,
        Charging,
        Full
    };

    void setView(View view);

public slots:
    void setPercent(int);
    void setStatus(int);


private:

    Ui::Form *ui;

    int m_percent;
    Status m_status;
    View m_view;

    void updateView();
};
