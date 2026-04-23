# QCustomPlot 源码与实现原理解析文档
## 版本说明
本项目使用的QCustomPlot版本为 **2.1.1**，是Qt生态中最成熟的开源科学数据可视化库。

---
## 一、整体架构设计
QCustomPlot采用三层解耦的架构设计，各层职责明确，扩展性强：

| 层级 | 核心模块 | 职责描述 |
|------|----------|----------|
| 逻辑控制层 | 布局系统、坐标轴管理器、交互控制器、图例管理器 | 处理业务逻辑、用户交互、元素布局和事件响应 |
| 数据管理层 | QCPDataContainer及其派生类 | 高效存储、检索和管理绘图数据，支持百万级数据点 |
| 渲染绘制层 | QCPPainter、OpenGL渲染后端 | 执行实际的绘图渲染工作，支持软件渲染和硬件加速 |

---
## 二、核心组件结构
### 2.1 顶层容器 QCustomPlot
- 继承自`QWidget`，是标准的Qt控件，作为整个绘图系统的入口
- 包含所有对外API接口，管理所有子组件的生命周期
- 核心属性：
  - `plotLayout`：根布局管理器，控制所有元素的排列
  - `interactions`：交互功能开关，支持缩放、平移、选择等操作
  - `selectionTolerance`：鼠标选择容差，单位像素
  - `openGl`：OpenGL硬件加速开关，开启后大幅提升渲染性能

### 2.2 图层系统 QCPLayer
- 所有可见元素都属于某个图层，图层按Z轴顺序绘制，上层覆盖下层
- 默认图层顺序（从下到上）：背景层 → 网格层 → 坐标轴层 → 数据层 → 装饰层 → 交互层
- 支持独立控制图层的可见性、抗锯齿属性，便于局部重绘优化

### 2.3 可绘制对象 QCPAbstractPlottable
- 所有数据图表的抽象基类，派生类包括：
  - `QCPGraph`：折线图、散点图、曲线图
  - `QCPBars`：柱状图、堆叠柱状图
  - `QCPColorMap`：热力图、频谱图
  - `QCPCurve`：参数曲线
  - `QCPFinancial`：K线图、金融图表
- 每个可绘制对象独立管理自己的数据、样式和绘制逻辑

### 2.4 坐标轴系统 QCPAxis
- 支持上下左右四个方向的坐标轴
- 自动计算刻度、标签范围，支持自定义刻度格式
- 支持对数坐标、时间坐标等多种坐标类型

### 2.5 布局系统
- 基于`QCPLayoutElement`实现灵活的布局系统
- 自动适配窗口大小变化，支持多坐标轴、多图例、色条等元素的自由排列
- 核心布局类`QCPLayoutGrid`实现网格状布局管理

---
## 三、核心实现原理
### 3.1 渲染流程（replot函数核心逻辑）
QCustomPlot的核心渲染入口是`replot()`函数，完整流程如下：
```cpp
void QCustomPlot::replot(RefreshPriority refreshPriority)
{
  // 1. 重绘优化：队列重绘机制，避免多次重复触发重绘
  if (refreshPriority == rpQueuedReplot) {
    if (!mReplotQueued) {
      mReplotQueued = true;
      QTimer::singleShot(0, this, SLOT(replot()));
    }
    return;
  }
  
  // 2. 防重入保护，避免重绘过程中再次触发重绘导致死循环
  if (mReplotting) return;
  mReplotting = true;
  
  // 3. 发送重绘前信号，允许用户插入自定义逻辑
  emit beforeReplot();
  
  // 4. 更新布局：重新计算所有元素的位置和大小
  updateLayout();
  
  // 5. 初始化绘制缓冲区
  setupPaintBuffers();
  
  // 6. 分层绘制：按顺序绘制所有图层到各自的缓冲区
  foreach (QCPLayer *layer, mLayers)
    layer->drawToPaintBuffer();
  
  // 7. 触发界面更新，合并所有缓冲区绘制到窗口
  if (refreshPriority == rpImmediateRefresh)
    repaint();
  else
    update();
  
  // 8. 统计重绘耗时，更新平均重绘时间
  mReplotTime = replotTimer.elapsed();
  mReplotTimeAverage = mReplotTimeAverage*0.9 + mReplotTime*0.1;
  
  // 9. 发送重绘完成信号
  emit afterReplot();
  mReplotting = false;
}
```

### 3.2 事件处理机制
- 重写`paintEvent`实现最终的界面绘制
- 重写`mousePressEvent`、`mouseMoveEvent`、`wheelEvent`等实现交互逻辑
- 支持自定义交互事件处理，允许用户扩展交互功能

---
## 四、核心性能优化方案
### 4.1 双缓冲机制
所有绘制操作先在离屏缓冲区完成，再一次性绘制到界面，完全避免绘制闪烁，提升视觉流畅度。

### 4.2 分层重绘
修改某层内容时只重绘该层，不需要重绘整个图表，大幅降低重绘开销。

### 4.3 数据存储优化
数据容器`QCPDataContainer`采用连续内存存储，支持O(1)随机访问和O(logN)范围查询，非常适合大数据量场景。

### 4.4 OpenGL硬件加速
开启OpenGL模式后，渲染性能提升5~10倍，适合实时数据流可视化场景（如本项目中的ECG心电图实时显示）。

### 4.5 降采样渲染
当数据点数量远大于像素宽度时，自动进行降采样处理，只绘制可见的有效数据点，大幅提升渲染效率。

---
## 五、本项目中的应用场景
结合本项目的医院管理系统，QCustomPlot主要应用于：
1. **ECG心电图实时显示**：通过`QCPGraph`实现多导联心电图波形的实时滚动展示
2. **患者生命体征趋势图**：展示体温、血压、心率等生命体征的历史变化趋势
3. **统计分析图表**：实现就诊量、病种分布等统计数据的可视化展示

---
## 六、使用建议
1. 对于实时数据展示场景，建议开启OpenGL加速，设置`setOpenGl(true)`
2. 大数据量场景下，建议使用`QCPDataContainer`的`add()`方法批量添加数据，避免频繁重绘
3. 实时滚动显示时，可以设置`setDataRange`只保留最新的N个数据点，避免内存持续增长
