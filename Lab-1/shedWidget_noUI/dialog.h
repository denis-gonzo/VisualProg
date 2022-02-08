#pragma once

#include <QDialog>

#include <QLabel>
#include <QComboBox>
#include <QTimeEdit>
#include <QPushButton>
#include <QCalendarWidget>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    QLabel *actionLabel;
    QLabel *animalLabel;
    QLabel *atypeLabel;
    QLabel *atypeNameLabel;
    QLabel *timeLabel;
    QLabel *dateLabel;

    QPushButton *actionInfoButton;
    QPushButton *animInfoButton;
    QPushButton *animFindButton;
    QPushButton *applyButton;
    QPushButton *closeButton;

    QComboBox *actionCombo;
    QComboBox *animalCombo;

    QCalendarWidget *calendar;
    QTimeEdit *timeedit;

};
