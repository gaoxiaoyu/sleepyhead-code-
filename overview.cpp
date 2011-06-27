/********************************************************************
 Overview GUI Implementation
 Copyright (c)2011 Mark Watkins <jedimark@users.sourceforge.net>
 License: GPL
*********************************************************************/

#include <QCalendarWidget>
#include <QTextCharFormat>
#include "overview.h"
#include "ui_overview.h"
#include "Graphs/graphdata_custom.h"
#include "Graphs/gXAxis.h"
#include "Graphs/gBarChart.h"
#include "Graphs/gLineChart.h"
#include "Graphs/gYAxis.h"
#include "Graphs/gFooBar.h"

Overview::Overview(QWidget *parent,QGLContext *context) :
    QWidget(parent),
    ui(new Ui::Overview)
{
    ui->setupUi(this);
    shared_context=context;

    profile=Profiles::Get(pref["Profile"].toString());
    AddData(ahidata=new HistoryData(profile));
    AddData(pressure=new HistoryCodeData(profile,CPAP_PressureAverage));
    AddData(pressure_min=new HistoryCodeData(profile,CPAP_PressureMin));
    AddData(pressure_max=new HistoryCodeData(profile,CPAP_PressureMax));

    AddData(pressure_eap=new HistoryCodeData(profile,BIPAP_EAPAverage));
    AddData(pressure_iap=new HistoryCodeData(profile,BIPAP_IAPAverage));

   // pressure->ForceMinY(3);
   // pressure->ForceMaxY(12);
    AddData(leak=new HistoryCodeData(profile,CPAP_LeakMedian));
    AddData(usage=new UsageHistoryData(profile,UHD_Hours));
    AddData(waketime=new UsageHistoryData(profile,UHD_Waketime));
    AddData(bedtime=new UsageHistoryData(profile,UHD_Bedtime));

    gSplitter=new QSplitter(Qt::Vertical,ui->SummaryGraphWindow);
    gSplitter->setStyleSheet("QSplitter::handle { background-color: 'dark grey'; }");

    gSplitter->setChildrenCollapsible(true);
    gSplitter->setHandleWidth(3);
    ui->graphLayout->addWidget(gSplitter);

    AHI=new gGraphWindow(ui->SummaryGraphWindow,"AHI",(QGLWidget *)NULL); // Not sure here..
    AHI->SetTopMargin(10);
    AHI->SetBottomMargin(AHI->GetBottomMargin()+gXAxis::Margin+25);
    AHI->AddLayer(new gFooBar(7));
    AHI->AddLayer(new gYAxis());
    AHI->AddLayer(new gBarChart(ahidata,QColor("red")));
    AHI->setMinimumHeight(170);

    PRESSURE=new gGraphWindow(ui->SummaryGraphWindow,"Pressure",AHI);
    //PRESSURE->SetMargins(10,15,65,80);
    PRESSURE->AddLayer(new gYAxis());
    PRESSURE->AddLayer(new gXAxis());
    PRESSURE->AddLayer(new gFooBar(7));
    PRESSURE->AddLayer(prmax=new gLineChart(pressure_max,QColor("blue"),6192,false,true,true));
    PRESSURE->AddLayer(prmin=new gLineChart(pressure_min,QColor("red"),6192,false,true,true));
    PRESSURE->AddLayer(eap=new gLineChart(pressure_eap,QColor("blue"),6192,false,true,true));
    PRESSURE->AddLayer(iap=new gLineChart(pressure_iap,QColor("red"),6192,false,true,true));
    PRESSURE->AddLayer(pr=new gLineChart(pressure,QColor("dark green"),6192,false,true,true));
    PRESSURE->SetBottomMargin(PRESSURE->GetBottomMargin()+25);
    PRESSURE->setMinimumHeight(170);

    LEAK=new gGraphWindow(ui->SummaryGraphWindow,"Leak",AHI);
    //LEAK->SetMargins(10,15,65,80);
    //LEAK->AddLayer(new gBarChart(leak,wxYELLOW));
    LEAK->AddLayer(new gXAxis());
    LEAK->AddLayer(new gYAxis());
    LEAK->AddLayer(new gFooBar(7));
    LEAK->AddLayer(new gLineChart(leak,QColor("purple"),6192,false,false,true));
    LEAK->SetBottomMargin(LEAK->GetBottomMargin()+25);
    LEAK->setMinimumHeight(170);

    USAGE=new gGraphWindow(ui->SummaryGraphWindow,"Usage (Hours)",AHI);
    //USAGE->SetMargins(10,15,65,80);
    USAGE->AddLayer(new gFooBar(7));
    USAGE->AddLayer(new gYAxis());
    USAGE->AddLayer(new gBarChart(usage,QColor("green")));
    USAGE->SetBottomMargin(USAGE->GetBottomMargin()+gXAxis::Margin+15);
    //USAGE->AddLayer(new gXAxis());
    //USAGE->AddLayer(new gLineChart(usage,QColor("green")));
    USAGE->setMinimumHeight(170);

    AHI->LinkZoom(PRESSURE);
    AHI->LinkZoom(LEAK);
    AHI->LinkZoom(USAGE);
    PRESSURE->LinkZoom(AHI);
    PRESSURE->LinkZoom(LEAK);
    PRESSURE->LinkZoom(USAGE);
    LEAK->LinkZoom(AHI);
    LEAK->LinkZoom(PRESSURE);
    LEAK->LinkZoom(USAGE);
    USAGE->LinkZoom(AHI);
    USAGE->LinkZoom(PRESSURE);
    USAGE->LinkZoom(LEAK);


    gSplitter->addWidget(AHI);
    gSplitter->addWidget(PRESSURE);
    gSplitter->addWidget(LEAK);
    gSplitter->addWidget(USAGE);
    dummyday=new Day(NULL);

    ReloadGraphs();
}

Overview::~Overview()
{
    delete dummyday;
    delete ui;
}

void Overview::ReloadGraphs()
{
    for (list<HistoryData *>::iterator h=Data.begin();h!=Data.end();h++) {
        (*h)->SetProfile(profile);
        (*h)->ResetDateRange();
        (*h)->Reload(NULL);
    }
    on_rbLastWeek_clicked(true);
}
void Overview::UpdateGraphs()
{
    QDate first=ui->drStart->date();
    QDate last=ui->drEnd->date();
    for (list<HistoryData *>::iterator h=Data.begin();h!=Data.end();h++) {
          //(*h)->Update(dummyday);
          (*h)->SetDateRange(first,last);
      }

}


void Overview::on_rbLastWeek_clicked(bool checked)
{
    ui->drStart->setDateRange(profile->FirstDay(),profile->LastDay());
    ui->drEnd->setDateRange(profile->FirstDay(),profile->LastDay());

    QDate d=profile->LastDay();
    ui->drEnd->setDate(d);
    d=d.addDays(-7);
    if (d<profile->FirstDay()) d=profile->FirstDay();
    ui->drStart->setDate(d);
    UpdateGraphs();
}

void Overview::on_rbLastMonth_clicked(bool checked)
{
    ui->drStart->setDateRange(profile->FirstDay(),profile->LastDay());
    ui->drEnd->setDateRange(profile->FirstDay(),profile->LastDay());

    QDate d=profile->LastDay();
    ui->drEnd->setDate(d);
    d=d.addDays(-30);
    if (d<profile->FirstDay()) d=profile->FirstDay();
    ui->drStart->setDate(d);
    UpdateGraphs();
}

void Overview::on_rbEverything_clicked(bool checked)
{
    ui->drStart->setDateRange(profile->FirstDay(),profile->LastDay());
    ui->drEnd->setDateRange(profile->FirstDay(),profile->LastDay());

    ui->drEnd->setDate(profile->LastDay());
    ui->drStart->setDate(profile->FirstDay());
    UpdateGraphs();
}

void Overview::on_rbDateRange_clicked(bool checked)
{
    UpdateGraphs();
}
void UpdateCal(QCalendarWidget *cal)
{
    QDate d1=cal->minimumDate();
    d1.setYMD(d1.year(),d1.month(),1);
    QTextCharFormat fmt=cal->weekdayTextFormat(Qt::Monday);
    fmt.setForeground(QBrush(Qt::gray));
    for (QDate d=d1;d < cal->minimumDate();d=d.addDays(1)) {
        cal->setDateTextFormat(d,fmt);
    }
    d1=cal->maximumDate();
    d1.setYMD(d1.year(),d1.month(),d1.daysInMonth());
    for (QDate d=cal->maximumDate().addDays(1);d <= d1;d=d.addDays(1)) {
        cal->setDateTextFormat(d,fmt);
    }

}
void Overview::on_drStart_dateChanged(const QDate &date)
{
    ui->drEnd->setMinimumDate(date);
    QCalendarWidget *cal=ui->drEnd->calendarWidget();
    cal->setDateRange(date,profile->LastDay());
    UpdateCal(cal);
    UpdateGraphs();
}

void Overview::on_drEnd_dateChanged(const QDate &date)
{
    ui->drStart->setMaximumDate(date);
    QCalendarWidget *cal=ui->drStart->calendarWidget();
    cal->setDateRange(profile->FirstDay(),date);
    UpdateCal(cal);
    UpdateGraphs();
}

void Overview::on_rbDateRange_toggled(bool checked)
{
    ui->drStart->setEnabled(checked);
    ui->drEnd->setEnabled(checked);
    ui->drStartLabel->setEnabled(checked);
    ui->drEndLabel->setEnabled(checked);
}
