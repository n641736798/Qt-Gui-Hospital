#include "qcustomplotchart_adapter.h"

QCustomPlotChartAdapter::QCustomPlotChartAdapter(QWidget *parent)
    : IECGChart(parent)
{
    m_plot = new QCustomPlot(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_plot);
    setLayout(layout);
}

QCustomPlotChartAdapter::~QCustomPlotChartAdapter()
{
}

void QCustomPlotChartAdapter::init(const QString &title, const QString &yAxisLabel, double yMin, double yMax)
{
    // 设置背景色
    m_plot->setBackground(QBrush(QColor("#FFFFFF")));
    m_plot->axisRect()->setBackground(QBrush(QColor("#FFFFFF")));

    // 添加图表
    m_plot->addGraph();
    m_plot->graph(0)->setPen(QPen(QColor("#FF4444"), 2));

    // 设置坐标轴
    m_plot->xAxis->setLabel("时间(s)");
    m_plot->yAxis->setLabel(yAxisLabel);
    m_plot->yAxis->setRange(yMin, yMax);

    // 设置X轴样式（隐藏刻度和标签）
    m_plot->xAxis->setRange(0, 10);
    m_plot->xAxis->grid()->setVisible(true);
    m_plot->xAxis->grid()->setPen(QPen(QColor("#E0E0E0"), 1, Qt::DotLine));
    m_plot->xAxis->setTicks(false);
    m_plot->xAxis->setTickLabels(false);
    m_plot->xAxis->setBasePen(Qt::NoPen);
    m_plot->xAxis->setSubTicks(false);

    // 设置Y轴样式
    m_plot->yAxis->grid()->setVisible(true);
    m_plot->yAxis->grid()->setPen(QPen(QColor("#E0E0E0"), 1, Qt::DotLine));
    m_plot->yAxis->setTicks(false);
    m_plot->yAxis->setTickLabels(false);
    m_plot->yAxis->setBasePen(Qt::NoPen);
    m_plot->yAxis->setSubTicks(false);

    // 设置标题
    m_plot->plotLayout()->insertRow(0);
    QCPTextElement *titleElement = new QCPTextElement(m_plot, title, QFont("微软雅黑", 10, QFont::Bold));
    titleElement->setTextColor(QColor("#8B4513"));
    m_plot->plotLayout()->addElement(0, 0, titleElement);
}

void QCustomPlotChartAdapter::addDataPoint(double x, double y)
{
    if (m_dataBuffer.size() >= m_maxDataPoints) {
        m_dataBuffer.removeFirst();
    }
    m_dataBuffer.append(QPointF(x, y));
}

void QCustomPlotChartAdapter::updateDisplay()
{
    QVector<double> x, y;
    for (const auto &point : m_dataBuffer) {
        x.append(point.x());
        y.append(point.y());
    }
    m_plot->graph(0)->setData(x, y);

    if (!m_dataBuffer.isEmpty()) {
        double minX = m_dataBuffer.first().x();
        double maxX = m_dataBuffer.last().x();
        m_plot->xAxis->setRange(minX, maxX);
    }

    m_plot->replot(QCustomPlot::rpQueuedReplot);
}

void QCustomPlotChartAdapter::clearData()
{
    m_dataBuffer.clear();
    m_plot->graph(0)->data()->clear();
    m_plot->replot();
}

void QCustomPlotChartAdapter::setXRange(double min, double max)
{
    m_plot->xAxis->setRange(min, max);
}

void QCustomPlotChartAdapter::setYRange(double min, double max)
{
    m_plot->yAxis->setRange(min, max);
}

void QCustomPlotChartAdapter::setMaxDataPoints(int count)
{
    m_maxDataPoints = count;
}
