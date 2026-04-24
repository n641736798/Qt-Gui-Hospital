#ifndef IECGCHART_H
#define IECGCHART_H

#include <QWidget>
#include <QPointF>
#include <QList>

class IECGChart : public QWidget
{
    Q_OBJECT
public:
    explicit IECGChart(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~IECGChart() override = default;

    // 初始化图表，设置标题、坐标轴标签等
    virtual void init(const QString &title, const QString &yAxisLabel, double yMin, double yMax) = 0;

    // 添加数据点到缓冲区
    virtual void addDataPoint(double x, double y) = 0;

    // 批量更新图表显示
    virtual void updateDisplay() = 0;

    // 清除所有数据
    virtual void clearData() = 0;

    // 设置X轴范围
    virtual void setXRange(double min, double max) = 0;

    // 设置Y轴范围
    virtual void setYRange(double min, double max) = 0;

    // 设置最大数据点数
    virtual void setMaxDataPoints(int count) = 0;
};

#endif // IECGCHART_H
