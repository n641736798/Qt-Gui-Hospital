# ECG 曲线改用 QCustomPlot — 任务清单（Task List）

基于《ECG 曲线显示改用 QCustomPlot — 设计文档》拆分的具体实施任务，按阶段排序，便于跟踪与验收。

---

## 阶段一：环境与工程配置

| ID | 任务 | 说明 | 产出/验收 |
|----|------|------|-----------|
| T1.1 | 确认 QCustomPlot 源文件可用 | 检查 `D:\software\Qt\additional part\qcustomplot-source` 下存在 `qcustomplot.h`、`qcustomplot.cpp` | 路径可访问，文件完整 |
| T1.2 | 选择 QCustomPlot 集成方式 | 二选一：A) 工程内直接引用该路径；B) 将 `qcustomplot.h`、`qcustomplot.cpp` 复制到项目（如 `src/` 或 `include/`） | 选定方案并记录 |
| T1.3 | 修改 `Hospital_reorganized.pro` | 按选定方案：添加 `qcustomplot.cpp` 到 `SOURCES`、`qcustomplot.h` 到 `HEADERS`（若适用），以及 `INCLUDEPATH` | `.pro` 修改完成，qmake 通过 |
| T1.4 | 移除 Qt Charts 对 ECG 的依赖（可选） | 若项目其他处不用 Qt Charts：在 `.pro` 中注释或删除 `QT += charts` | `.pro` 中无 `QT += charts` 或已注释 |
| T1.5 | 清理旧构建产物 | 执行 `qmake` 后 `make clean`（或 Qt Creator 清理），再重新构建 | 无旧 Charts 相关目标残留 |

---

## 阶段二：头文件修改（mainwindow.h）

| ID | 任务 | 说明 | 产出/验收 |
|----|------|------|-----------|
| T2.1 | 移除 Qt Charts 相关 `#include` | 删除 `QChartView`、`QLineSeries`、`QChart`、`QValueAxis`、`QChartGlobal` 等 Charts 头文件 | 头文件中无 Qt Charts 引用 |
| T2.2 | 添加 QCustomPlot 头文件 | 增加 `#include "qcustomplot.h"`，确保包含路径已配置 | 编译通过，无找不到头文件错误 |
| T2.3 | 移除 `ecgSeries`、`ecgChartViews` 成员 | 删除 `#ifndef NO_QT_CHARTS` 块内的 `QList<QLineSeries*> ecgSeries`、`QList<QChartView*> ecgChartViews` | 对应成员已删除 |
| T2.4 | 添加 `ecgPlots` 成员 | 在 ECG 相关 private 区域添加 `QList<QCustomPlot*> ecgPlots;` | 成员声明存在且类型正确 |
| T2.5 | 确认保留的 ECG 成员未改动 | 核对 `ecgDataBuffers`、`MAX_DATA_POINTS`、`axisUpdateCounter`、`chartUpdateTimer`、`setupECGThread`、`updateECGDisplay`、`batchUpdateCharts` 等保留 | 与设计文档一致 |

---

## 阶段三：实现文件修改（mainwindow.cpp）

### 3.1 包含与全局

| ID | 任务 | 说明 | 产出/验收 |
|----|------|------|-----------|
| T3.1 | 移除 Qt Charts 的 `#include` | 删除 `QChartView`、`QLineSeries`、`QChart`、`QValueAxis` 等 | 无 Charts 引用 |
| T3.2 | 添加 `#include "qcustomplot.h"` | 在合适位置加入 QCustomPlot 头文件 | 编译通过 |

### 3.2 setupECGDashboard()

| ID | 任务 | 说明 | 产出/验收 |
|----|------|------|-----------|
| T3.3 | 移除 `setupECGDashboard` 内 Qt Charts / `NO_QT_CHARTS` 分支 | 删除所有 `#ifndef NO_QT_CHARTS` 下创建 `QLineSeries`、`QChart`、`QChartView` 的代码，以及 `#else` 分支的占位 `QLabel` | 仅保留清空 statsGrid、创建 Frames/Layouts、导联名称等与 QCustomPlot 无关的公共逻辑 |
| T3.4 | 实现基于 QCustomPlot 的 4 图创建逻辑 | 在循环内：`QCustomPlot *customPlot = new QCustomPlot();`，`ecgPlots.append(customPlot)`；为每个创建 `addGraph()`，配置 `graph(0)` 的 `QPen`（`#FF4444`，线宽 2） | 4 个 `QCustomPlot` 被创建并加入 `ecgPlots` |
| T3.5 | 配置每个 QCustomPlot 的 X 轴 | 对 `xAxis`：`setRange(0,10)`；`grid()->setVisible(true)`；`grid()->setPen(...)#E0E0E0`；`setTicks(false)`；`setTickLabels(false)`；`setBasePen(Qt::NoPen)`；`setSubTicks(false)` | 与设计文档 6.2 节一致 |
| T3.6 | 配置每个 QCustomPlot 的 Y 轴 | 对 `yAxis`：`setRange(-2,2)`；网格、刻度、轴线配置同 X 轴 | 与设计文档 6.2 节一致 |
| T3.7 | 设置绘图区背景与最小高度 | `setBackground(QBrush(QColor("#FFFFFF")))`；`setMinimumHeight(150)`；按需设置 `setInteractions` | 白底、最小高度 150px |
| T3.8 | 将各 QCustomPlot 加入对应 Frame 布局 | 在每个导联的 `QVBoxLayout` 中先加标题 `QLabel`，再加 `customPlot`；保持「导联 I / II / III / aVR」标题及样式 | 布局与设计文档 4.1 节一致 |

### 3.3 batchUpdateCharts()

| ID | 任务 | 说明 | 产出/验收 |
|----|------|------|-----------|
| T3.9 | 移除 `batchUpdateCharts` 内 Qt Charts 实现 | 删除 `#ifndef NO_QT_CHARTS` 下所有 `QLineSeries::replace`、`QChartView`、`QValueAxis` 相关代码 | 无 Charts 调用 |
| T3.10 | 实现按缓冲区更新 QCustomPlot 数据 | 遍历 `i = 0..3`，对 `ecgPlots[i]`、`ecgDataBuffers[i]`：若缓冲区非空，将 `QList<QPointF>` 转为 `QVector<double> keys, values`（`reserve` 后逐点 `x/y` 填入），再 `graph(0)->setData(keys, values)` | 数据来源、转换与设计 4.2、5.3 一致 |
| T3.11 | 实现 X 轴滚动更新逻辑 | 维护 `axisUpdateCounter`，每 10 次调用：`axisUpdateCounter=0`；对各导联若缓冲区非空，取最后时间 `currentTime`，`xAxis->setRange(currentTime - 10, currentTime)` | 约 1 秒更新一次 X 轴，10 秒滚动窗口 |
| T3.12 | 调用 `replot` 刷新所有图表 | 在更新数据和 X 轴之后，对 `ecgPlots` 中每个 `QCustomPlot` 调用 `replot()` | 曲线与坐标轴视觉更新 |

### 3.4 setChildrenBackground()

| ID | 任务 | 说明 | 产出/验收 |
|----|------|------|-----------|
| T3.13 | 修改背景跳过逻辑 | 将原先跳过 `QChartView`/`QGraphicsView` 的判断改为跳过 `QCustomPlot`（如 `child->inherits("QCustomPlot")`） | 递归设置背景时不再影响 4 个曲线图 |

### 3.5 其他与 Charts 相关的清理

| ID | 任务 | 说明 | 产出/验收 |
|----|------|------|-----------|
| T3.14 | 移除其余 `NO_QT_CHARTS` 相关分支 | 全局搜索 `NO_QT_CHARTS`，删除或替换所有仅用于 Charts 的 `#ifdef`/`#else`/`#endif` 块 | 代码中不再保留与 Qt Charts ECG 相关的条件编译 |
| T3.15 | 检查并移除未使用的 QPainter 等包含 | 若 `QPainter` 等仅被 Charts 使用，可移除；若仍被别处使用则保留 | 无多余包含，编译无告警 |

---

## 阶段四：编译与静态检查

| ID | 任务 | 说明 | 产出/验收 |
|----|------|------|-----------|
| T4.1 | 全量编译 | `qmake` 后 `make`（或 Qt Creator 构建） | 编译通过，无错误 |
| T4.2 | 修复编译错误 | 根据报错修正缺失头文件、类型、成员、API 用法等 | 无编译错误 |
| T4.3 | 消除新增告警（可选） | 处理与本次修改相关的编译器/静态分析告警 | 无新增告警或已记录例外 |

---

## 阶段五：功能与回归测试

| ID | 任务 | 说明 | 产出/验收 |
|----|------|------|-----------|
| T5.1 | 启动程序并打开仪表盘 | 运行应用，切到仪表盘（Home），确认 4 个 ECG 图区域存在 | 4 个图框、标题可见 |
| T5.2 | 初始化与样式检查 | 检查 4 个图均为 QCustomPlot 绘制；曲线色 `#FF4444`、线宽 2；白底、网格 `#E0E0E0`；无刻度/轴线；最小高度 150px | 与设计文档 2.4、6.2、6.3 一致 |
| T5.3 | 开始监护 — 数据更新 | 点击「开始监护」，观察四条曲线随 `ECGDataThread` 数据变化 | 曲线持续更新 |
| T5.4 | X 轴滚动与 Y 轴范围 | 长时间运行，确认 X 轴约 10 秒窗口滚动；Y 轴保持 [-2, 2] | 与设计 2.3、4.2 一致 |
| T5.5 | 暂停监护 | 点击「暂停监护」，曲线停止更新，界面无崩溃、无报错 | 符合需求 |
| T5.6 | 恢复监护 | 再次「开始监护」，曲线继续更新 | 与需求一致 |
| T5.7 | 背景色与 setChildrenBackground | 触发 `setChildrenBackground`（如切换页面再回仪表盘等），确认 4 个 QCustomPlot 区域未被错误上色 | 曲线图背景保持白色 |

---

## 阶段六：性能与兼容性（可选）

| ID | 任务 | 说明 | 产出/验收 |
|----|------|------|-----------|
| T6.1 | 长时间运行观察 | 监护运行 5–10 分钟，观察内存、CPU 是否异常 | 无明显内存泄漏、CPU 占用可接受 |
| T6.2 | 不同 Qt 版本编译（若有条件） | 在现有支持的 Qt 版本下编译运行 | 编译与运行正常 |
| T6.3 | OpenGL 加速（可选） | 若遇卡顿，可尝试 `customPlot->setOpenGl(true)` 并验证 | 按需启用，无渲染异常 |

---

## 任务依赖关系简图

```
T1.1 → T1.2 → T1.3 → T1.5
T1.4 可选
T2.1,T2.2,T2.3,T2.4 可并行；T2.5 与前序同阶段
T3.1,T3.2 先做；T3.3–T3.8 属 setupECGDashboard；T3.9–T3.12 属 batchUpdateCharts；T3.13–T3.15 可随后
阶段二、三 均依赖于 阶段一 完成
T4.1–T4.3 依赖 阶段二、三 完成
T5.x 依赖 T4.1 通过
T6.x 依赖 T5 基本通过，属增强项
```

---

## 检查清单（交付前自检）

- [ ] **工程**：QCustomPlot 已接入 `.pro`，Qt Charts 已从 ECG 移除（若适用）
- [ ] **头文件**：`mainwindow.h` 仅使用 QCustomPlot，含 `ecgPlots`，无 Charts 成员
- [ ] **实现**：`setupECGDashboard`、`batchUpdateCharts`、`setChildrenBackground` 按设计文档实现并已移除 `NO_QT_CHARTS` 分支
- [ ] **编译**：本地默认配置下编译通过
- [ ] **功能**：4 图显示、实时更新、10 秒滚动、暂停/恢复、背景色均符合需求与设计

---

## 参考文档

- 设计文档：`ecg_改用_qcustomplot_设计文档_f036f4e7.plan.md`
- 需求文档：`ecg_改用_qcustomplot_需求文档_7ee7c94f.plan.md`
- QCustomPlot 源码：`D:\software\Qt\additional part\qcustomplot-source`
