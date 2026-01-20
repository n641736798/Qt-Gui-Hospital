#include "mainwindow.h"

#include <QApplication>
#include <QCursor>
#include <QPixmap>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 设置全局应用程序样式 - 确保暖色调背景
    a.setStyleSheet(R"(
        QMainWindow, QWidget {
            background-color: #F5E6D3;
        }
        QDialog {
            background-color: #F5E6D3;
        }
        QStackedWidget {
            background-color: #F5E6D3;
        }
        QFrame {
            background-color: #F5E6D3;
        }
        QVBoxLayout, QHBoxLayout, QGridLayout {
            background-color: #F5E6D3;
        }
    )");

    QPixmap originalCursorPixmap(":/img/cursor.png");

    if (originalCursorPixmap.isNull()) {
        qDebug() << "Error: Could not load cursor image.";
    }

    int newCursorWidth = 16;
    int newCursorHeight = 16;

    QPixmap scaledCursorPixmap = originalCursorPixmap.scaled(newCursorWidth, newCursorHeight,
                                                             Qt::KeepAspectRatio,
                                                             Qt::SmoothTransformation);

    QCursor customCursor(scaledCursorPixmap);

    a.setOverrideCursor(customCursor);

    MainWindow w;
    w.show();

    int result = a.exec();

    a.restoreOverrideCursor();

    return result;
}
