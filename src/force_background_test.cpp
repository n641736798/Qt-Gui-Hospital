#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QDebug>
#include <QTimer>

// 测试程序：强制设置所有组件背景色
class BackgroundTester : public QMainWindow
{
public:
    BackgroundTester() {
        // 设置全局样式
        setStyleSheet(R"(
            * {
                background-color: #F5E6D3 !important;
            }
            QFrame {
                background-color: #FFFFFF !important;
                border: 1px solid #D2B48C !important;
            }
        )");
        
        // 设置调色板
        QPalette palette;
        palette.setColor(QPalette::Window, QColor("#F5E6D3"));
        palette.setColor(QPalette::Base, QColor("#F5E6D3"));
        palette.setColor(QPalette::Button, QColor("#F5E6D3"));
        palette.setColor(QPalette::WindowText, QColor("#8B4513"));
        setPalette(palette);
        
        resize(800, 600);
        setWindowTitle("背景色测试");
        
        qDebug() << "背景色测试程序启动";
        qDebug() << "如果看到暖色背景，说明设置成功";
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 全局样式
    app.setStyleSheet(R"(
        * {
            background-color: #F5E6D3;
        }
    )");
    
    BackgroundTester window;
    window.show();
    
    return app.exec();
}