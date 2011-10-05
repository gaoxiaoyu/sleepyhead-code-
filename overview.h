/*
 Overview GUI Headers
 Copyright (c)2011 Mark Watkins <jedimark@users.sourceforge.net>
 License: GPL
*/

#ifndef OVERVIEW_H
#define OVERVIEW_H

#include <QWidget>
#include <QGLContext>
#include <QHBoxLayout>
#include <QDateEdit>
#include "SleepLib/profiles.h"
#include "Graphs/gGraphView.h"
#include "Graphs/gSummaryChart.h"
#include "report.h"

namespace Ui {
    class Overview;
}

class Report;
class Overview : public QWidget
{
    Q_OBJECT

public:
    explicit Overview(QWidget *parent, gGraphView *shared=NULL);
    ~Overview();

    void ReloadGraphs();
    void ResetGraphLayout();
    void RedrawGraphs();


    gGraph *AHI,*UC,*PR,*LK,*NPB,*SET;
    SummaryChart *bc,*uc,*pr,*lk,*npb,*set;
public slots:
    void on_printButton_clicked();

private slots:
/*    void on_drStart_dateChanged(const QDate &date);
    void on_drEnd_dateChanged(const QDate &date);
    void on_rbDateRange_toggled(bool checked);
    void on_rbLastWeek_clicked();
    void on_rbLastMonth_clicked();
    void on_rbEverything_clicked();
    void on_rbDateRange_clicked(); */

    void on_dateStart_dateChanged(const QDate &date);
    void on_dateEnd_dateChanged(const QDate &date);

    void on_dateStart_currentPageChanged(int year, int month);
    void on_dateEnd_currentPageChanged(int year, int month);

    void on_toolButton_clicked();

    void on_htmlButton_clicked();

private:
    Ui::Overview *ui;
    gGraphView *GraphView;
    MyScrollBar *scrollbar;
    QHBoxLayout *layout;
    gGraphView * m_shared;
    Report * report;

    void UpdateHTML();
    void UpdateCalendarDay(QDateEdit * calendar,QDate date);


    QString GetHTML();

    //SessionTimes *session_times;
    //,*PRESSURE,*LEAK,*SESSTIMES;

    //Layer *prmax,*prmin,*iap,*eap,*pr,*sesstime;

    Day * day;// dummy in this case

};

#endif // OVERVIEW_H
