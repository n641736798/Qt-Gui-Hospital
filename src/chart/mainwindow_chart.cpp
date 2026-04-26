#include "mainwindow.h"
#include "qcustomplotchart_adapter.h"
#include "qtchart_adapter.h"

IECGChart* MainWindow::createChart(const QString &title, const QString &yAxisLabel, double yMin, double yMax)
{
    IECGChart *chart = nullptr;
    if (m_chartRenderer == ChartRenderer::QCustomPlot) {
        chart = new QCustomPlotChartAdapter(this);
    } else {
        chart = new QtChartAdapter(this);
    }
    chart->init(title, yAxisLabel, yMin, yMax);
    chart->setMaxDataPoints(MAX_DATA_POINTS);
    return chart;
}
