#include "mainwindow.h"

#include <QApplication>
#include <QCursor>
#include <QPixmap>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

static QFile *g_logFile = nullptr;

static void debugMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(type)
    Q_UNUSED(context)
    if (g_logFile && g_logFile->isOpen()) {
        QTextStream stream(g_logFile);
        stream << QDateTime::currentDateTime().toString("hh:mm:ss.zzz")
               << " " << msg << "\n";
        g_logFile->flush();
    }
}

int main(int argc, char *argv[])
{
    g_logFile = new QFile("debug_log.txt");
    g_logFile->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    qInstallMessageHandler(debugMessageHandler);

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
