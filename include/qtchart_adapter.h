#ifndef QTCHART_ADAPTER_H
#define QTCHART_ADAPTER_H

#include "iecgchart.h"
#include <QVBoxLayout>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>

class QtChartAdapter : public IECGChart
{
    Q_OBJECT
public:
    explicit QtChartAdapter(QWidget *parent = nullptr);
    ~QtChartAdapter() override;

    void init(const QString &title, const QString &yAxisLabel, double yMin, double yMax) override;
    void addDataPoint(double x, double y) override;
    void updateDisplay() override;
    void clearData() override;
    void setXRange(double min, double max) override;
    void setYRange(double min, double max) override;
    void setMaxDataPoints(int count) override;

private:
    QChart *m_chart;
    QChartView *m_chartView;
    QLineSeries *m_series;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    QList<QPointF> m_dataBuffer;
    int m_maxDataPoints = 200;
};

#endif // QTCHART_ADAPTER_H
