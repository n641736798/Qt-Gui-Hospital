#include "qtchart_adapter.h"

QtChartAdapter::QtChartAdapter(QWidget *parent)
    : IECGChart(parent)
{
    m_chart = new QChart();
    m_series = new QLineSeries();
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();

    m_chart->addSeries(m_series);
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_series->attachAxis(m_axisX);
    m_series->attachAxis(m_axisY);

    m_chartView = new QChartView(m_chart, this);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_chartView);
    setLayout(layout);
}

QtChartAdapter::~QtChartAdapter()
{
}

void QtChartAdapter::init(const QString &title, const QString &yAxisLabel, double yMin, double yMax)
{
    // 设置背景色
    m_chart->setBackgroundBrush(QBrush(QColor("#F5E6D3")));
    m_chart->setPlotAreaBackgroundBrush(QBrush(QColor("#F5E6D3")));

    // 设置标题
    m_chart->setTitle(title);
    QFont titleFont = m_chart->titleFont();
    titleFont.setFamily("微软雅黑");
    titleFont.setBold(true);
    titleFont.setPointSize(10);
    m_chart->setTitleFont(titleFont);
    m_chart->setTitleBrush(QBrush(QColor("#333333")));

    // 设置系列样式
    QPen pen(QColor("#2C5F2D"), 2);
    m_series->setPen(pen);

    // 设置坐标轴
    m_axisX->setTitleText("时间(s)");
    m_axisY->setTitleText(yAxisLabel);
    m_axisY->setRange(yMin, yMax);

    // 美化坐标轴样式
    m_axisX->setLinePen(QPen(QColor("#666666")));
    m_axisY->setLinePen(QPen(QColor("#666666")));
    m_axisX->setLabelsColor(QColor("#333333"));
    m_axisY->setLabelsColor(QColor("#333333"));
    m_axisX->setTitleBrush(QBrush(QColor("#333333")));
    m_axisY->setTitleBrush(QBrush(QColor("#333333")));

    // 隐藏图例
    m_chart->legend()->hide();
}

void QtChartAdapter::addDataPoint(double x, double y)
{
    if (m_dataBuffer.size() >= m_maxDataPoints) {
        m_dataBuffer.removeFirst();
    }
    m_dataBuffer.append(QPointF(x, y));
}

void QtChartAdapter::updateDisplay()
{
    m_series->clear();
    for (const auto &point : m_dataBuffer) {
        m_series->append(point);
    }

    if (!m_dataBuffer.isEmpty()) {
        double minX = m_dataBuffer.first().x();
        double maxX = m_dataBuffer.last().x();
        m_axisX->setRange(minX, maxX);
    }
}

void QtChartAdapter::clearData()
{
    m_dataBuffer.clear();
    m_series->clear();
}

void QtChartAdapter::setXRange(double min, double max)
{
    m_axisX->setRange(min, max);
}

void QtChartAdapter::setYRange(double min, double max)
{
    m_axisY->setRange(min, max);
}

void QtChartAdapter::setMaxDataPoints(int count)
{
    m_maxDataPoints = count;
}
