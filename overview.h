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

namespace Ui {
    class Overview;
}

class Report;

enum YTickerType { YT_Number, YT_Time, YT_Weight };

/*! \class Overview
    \author Mark Watkins <jedimark_at_users.sourceforge.net>
    \brief Overview tab, showing overall summary data
    */
class Overview : public QWidget
{
    Q_OBJECT

public:
    explicit Overview(QWidget *parent, gGraphView *shared=NULL);
    ~Overview();

    //! \brief Returns Overview gGraphView object containing it's graphs
    gGraphView *graphView() { return GraphView; }

    //! \brief Recalculates Overview chart info
    void ReloadGraphs();

    //! \brief Reset graphs to uniform heights
    void ResetGraphLayout();

    //! \brief Calls updateGL to redraw the overview charts
    void RedrawGraphs();

    /*! \brief Create an overview graph, adding it to the overview gGraphView object
        \param QString name  The title of the graph
        \param QString units The units of measurements to show in the popup */
    gGraph * createGraph(QString name,QString units="",YTickerType yttype=YT_Number);

    gGraph *AHI, *AHIHR, *UC, *US, *PR,*LK,*NPB,*SET,*SES,*RR,*MV,*TV,*PTB,*PULSE,*SPO2,*WEIGHT,*ZOMBIE, *BMI;
    SummaryChart *bc,*uc, *us, *pr,*lk,*npb,*set,*ses,*rr,*mv,*tv,*ptb,*pulse,*spo2,*weight,*zombie, *bmi, *ahihr;

    //! \breif List of SummaryCharts shown on the overview page
    QVector<SummaryChart *> OverviewCharts;

public slots:
    //! \brief Print button down the bottom, does the same as File->Print
    void on_printButton_clicked();

private slots:
/*    void on_drStart_dateChanged(const QDate &date);
    void on_drEnd_dateChanged(const QDate &date);
    void on_rbDateRange_toggled(bool checked);
    void on_rbLastWeek_clicked();
    void on_rbLastMonth_clicked();
    void on_rbEverything_clicked();
    void on_rbDateRange_clicked(); */

    //! \brief Resets the graph view because the Start date has been changed
    void on_dateStart_dateChanged(const QDate &date);

    //! \brief Resets the graph view because the End date has been changed
    void on_dateEnd_dateChanged(const QDate &date);

    //! \brief Updates the calendar highlighting when changing to a new month
    void dateStart_currentPageChanged(int year, int month);

    //! \brief Updates the calendar highlighting when changing to a new month
    void dateEnd_currentPageChanged(int year, int month);

    //! \brief Resets view to currently shown start & end dates
    void on_toolButton_clicked();

    void on_printDailyButton_clicked();

    void on_rangeCombo_activated(int index);

private:
    Ui::Overview *ui;
    gGraphView *GraphView;
    MyScrollBar *scrollbar;
    QHBoxLayout *layout;
    gGraphView * m_shared;

    //! \brief Updates the calendar highlighting for the calendar object for this date.
    void UpdateCalendarDay(QDateEdit * calendar,QDate date);


    //SessionTimes *session_times;
    //,*PRESSURE,*LEAK,*SESSTIMES;

    //Layer *prmax,*prmin,*iap,*eap,*pr,*sesstime;

    Day * day;// dummy in this case

};

#endif // OVERVIEW_H
