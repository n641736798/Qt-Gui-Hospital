#ifndef QCUSTOMPLOTCHART_ADAPTER_H
#define QCUSTOMPLOTCHART_ADAPTER_H

#include "iecgchart.h"
#include "qcustomplot.h"

class QCustomPlotChartAdapter : public IECGChart
{
    Q_OBJECT
public:
    explicit QCustomPlotChartAdapter(QWidget *parent = nullptr);
    ~QCustomPlotChartAdapter() override;

    void init(const QString &title, const QString &yAxisLabel, double yMin, double yMax) override;
    void addDataPoint(double x, double y) override;
    void updateDisplay() override;
    void clearData() override;
    void setXRange(double min, double max) override;
    void setYRange(double min, double max) override;
    void setMaxDataPoints(int count) override;

private:
    QCustomPlot *m_plot;
    QList<QPointF> m_dataBuffer;
    int m_maxDataPoints = 200;
};

#endif // QCUSTOMPLOTCHART_ADAPTER_H
