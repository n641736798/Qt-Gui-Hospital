/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QWidget *sidebar;
    QVBoxLayout *sidebarLayout;
    QPushButton *toggleButton;
    QPushButton *homeButton;
    QPushButton *patientsButton;
    QPushButton *medicalRecordsButton;
    QPushButton *appointmentsButton;
    QSpacerItem *verticalSpacer;
    QPushButton *settingsButton;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QFrame *topbar;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_3;
    QPushButton *pushButton_2;
    QPushButton *close;
    QFrame *content;
    QVBoxLayout *verticalLayout_4;
    QStackedWidget *contentStackWidget;
    QWidget *homePage;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *statsGrid;
    QFrame *mainChartFrame;
    QVBoxLayout *mainChartLayout;
    QHBoxLayout *hboxLayout;
    QLabel *mainChartTitle;
    QSpacerItem *spacerItem;
    QPushButton *op1Button;
    QPushButton *op2Button;
    QGraphicsView *mainChartPlaceholder;
    QFrame *barChartFrame;
    QVBoxLayout *barChartLayout;
    QLabel *barChartTitle;
    QGraphicsView *barChartPlaceholder;
    QFrame *donutChartFrame;
    QVBoxLayout *verticalLayout_5;
    QLabel *label;
    QGraphicsView *donutChartPlaceholder;
    QFrame *verticalBarFrame;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_2;
    QGraphicsView *verticalBarPlaceholder;
    QWidget *patientsPage;
    QVBoxLayout *verticalLayout_3;
    QLabel *pageTitle;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *addPatientButton;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *searchLineEdit;
    QTableView *patientsTableView;
    QWidget *appointmentsPage;
    QVBoxLayout *verticalLayout_8;
    QLabel *appointmentsPageTitle;
    QHBoxLayout *horizontalLayout_4;
    QCalendarWidget *calendarWidget;
    QVBoxLayout *verticalLayout_9;
    QLabel *label_6;
    QTableView *appointmentsTableView;
    QPushButton *addAppointmentButton;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        MainWindow->setStyleSheet(QString::fromUtf8("/* \344\270\273\347\252\227\344\275\223\350\203\214\346\231\257\350\211\262 */\n"
"QMainWindow {\n"
"    background-color: #F5E6D3;\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setStyleSheet(QString::fromUtf8("/* --- \346\230\216\344\272\256\346\232\226\350\211\262\350\260\203\345\214\273\351\231\242\347\256\241\347\220\206\347\263\273\347\273\237\344\270\273\351\242\230 --- */\n"
"\n"
"/* Main Window & Central Widget */\n"
"QMainWindow, QWidget#centralwidget {\n"
"    background-color: #F5E6D3;\n"
"}\n"
"\n"
"/* --- Frame --- */\n"
"QWidget#frame\n"
"{\n"
"	background-color: #F5E6D3;\n"
"}\n"
"/* --- Sidebar --- */\n"
"QWidget#sidebar {\n"
"    background-color: #E8D5C4;\n"
"	border-radius: 10px;\n"
"	border: 1px solid #D2B48C;\n"
"}\n"
"QWidget#sidebar QPushButton {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"    color: #8B4513;\n"
"    padding: 10px;\n"
"    padding-left: 15px;\n"
"    border-radius: 5px;\n"
"    text-align: left;\n"
"    font-weight: bold;\n"
"}\n"
"QWidget#sidebar QPushButton:hover {\n"
"    background-color: #DEB887;\n"
"}\n"
"QWidget#sidebar QPushButton:pressed,\n"
"QWidget#sidebar QPushButton:checked {\n"
"    background-color: #FF8C42;\n"
"    color: #FFFFFF;\n"
"}\n"
""
                        "\n"
"/* --- Topbar --- */\n"
"QFrame#topbar {\n"
"    background-color: #E8D5C4;\n"
"    border-bottom: 1px solid #D2B48C;\n"
"}\n"
"}\n"
"QFrame#topbar QPushButton {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"    border-radius: 4px;\n"
"    padding: 5px;\n"
"    text-align: center;\n"
"    color: #8B4513;\n"
"}\n"
"QFrame#topbar QPushButton:hover {\n"
"    background-color: #DEB887;\n"
"}\n"
"QFrame#topbar QPushButton#close:hover {\n"
"    background-color: #CD853F;\n"
"}\n"
"\n"
"/* --- Main Content Area --- */\n"
"QFrame#content, QStackedWidget > QWidget {\n"
"    background-color: #F5E6D3;\n"
"    border: none;\n"
"}\n"
"\n"
"/* --- Stacked Widget Background --- */\n"
"QStackedWidget#contentStackWidget {\n"
"    background-color: #F5E6D3;\n"
"}\n"
"\n"
"/* --- Home Page Background --- */\n"
"QWidget#homePage {\n"
"    background-color: #F5E6D3;\n"
"}\n"
"\n"
"/* --- Layout Backgrounds --- */\n"
"QGridLayout#statsGrid {\n"
"    background-color: #F5E6D3;\n"
"}\n"
"\n"
"QVBoxLayout#vert"
                        "icalLayout_2 {\n"
"    background-color: #F5E6D3;\n"
"}\n"
"\n"
"/* General Page Titles */\n"
"QLabel#pageTitle {\n"
"    color: #8B4513;\n"
"    font-size: 18pt;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"/* --- Dashboard Specific Styles --- */\n"
"#homePage .QFrame {\n"
"    background-color: #FFFFFF;\n"
"    border-radius: 12px;\n"
"    border: 1px solid #D2B48C;\n"
"    box-shadow: 0 2px 4px rgba(139, 69, 19, 0.1);\n"
"}\n"
"#homePage QLabel {\n"
"    color: #8B4513;\n"
"    font-size: 11pt;\n"
"}\n"
"#homePage #mainChartTitle, #homePage #barChartTitle {\n"
"    font-size: 14pt;\n"
"    font-weight: bold;\n"
"    padding: 5px;\n"
"}\n"
"#homePage QProgressBar {\n"
"    border: 1px solid #D2B48C;\n"
"    border-radius: 5px;\n"
"    text-align: center;\n"
"    color: #8B4513;\n"
"    background-color: #F5E6D3;\n"
"}\n"
"#homePage QProgressBar::chunk {\n"
"    background-color: #FF8C42;\n"
"    border-radius: 4px;\n"
"}\n"
"#homePage #op1Button, #homePage #op2Button {\n"
"    background-color: #FF8C42;\n"
"   "
                        " color: #FFFFFF;\n"
"    border-radius: 5px;\n"
"    padding: 5px 10px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"/* --- Patients & Appointments Page Styles --- */\n"
"#patientsPage QPushButton, #appointmentsPage QPushButton {\n"
"    background-color: #FF8C42;\n"
"    color: #FFFFFF;\n"
"    border-radius: 5px;\n"
"    padding: 8px 12px;\n"
"    font-weight: bold;\n"
"}\n"
"#patientsPage QPushButton:hover, #appointmentsPage QPushButton:hover {\n"
"    background-color: #FF9F5A;\n"
"}\n"
"\n"
"/* --- Table View --- */\n"
"QTableView {\n"
"    background-color: #FFFFFF;\n"
"    border: 1px solid #D2B48C;\n"
"    border-radius: 8px;\n"
"    gridline-color: #D2B48C;\n"
"    color: #8B4513;\n"
"}\n"
"QTableView::item {\n"
"    padding: 5px;\n"
"    border-bottom: 1px solid #D2B48C;\n"
"}\n"
"QTableView::item:selected {\n"
"    background-color: #FF8C42;\n"
"    color: #FFFFFF;\n"
"}\n"
"QHeaderView::section {\n"
"    background-color: #E8D5C4;\n"
"    color: #8B4513;\n"
"    padding: 5px;\n"
"    border: 1px solid"
                        " #D2B48C;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"/* --- Line Edit (Search Bar) --- */\n"
"QLineEdit {\n"
"    background-color: #FFFFFF;\n"
"    border: 1px solid #D2B48C;\n"
"    border-radius: 5px;\n"
"    padding: 8px 10px;\n"
"    color: #8B4513;\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: #FF8C42;\n"
"}\n"
"\n"
"/* --- Calendar Widget --- */\n"
"QCalendarWidget QToolButton {\n"
"    color: #8B4513;\n"
"    background-color: transparent;\n"
"    border: none;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"QCalendarWidget QToolButton:hover {\n"
"    background-color: #DEB887;\n"
"}\n"
"QCalendarWidget QMenu {\n"
"    background-color: #FFFFFF;\n"
"    border: 1px solid #D2B48C;\n"
"}\n"
"QCalendarWidget QSpinBox {\n"
"    color: #8B4513;\n"
"    background-color: #FFFFFF;\n"
"    border: 1px solid #D2B48C;\n"
"}\n"
"QCalendarWidget QWidget#qt_calendar_navigationbar {\n"
"    background-color: #E8D5C4;\n"
"    border: 1px solid #D2B48C;\n"
"    border-bottom: none;\n"
"    border-radi"
                        "us: 8px 8px 0 0;\n"
"}\n"
"QCalendarWidget QAbstractItemView {\n"
"    background-color: #FFFFFF;\n"
"    color: #8B4513;\n"
"    selection-background-color: #FF8C42;\n"
"    selection-color: #FFFFFF;\n"
"    border: 1px solid #D2B48C;\n"
"    border-top: none;\n"
"    border-radius: 0 0 8px 8px;\n"
"}\n"
"QCalendarWidget QAbstractItemView:disabled {\n"
"    color: #CD853F;\n"
"}\n"
""));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(8);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(8, 8, 8, 8);
        sidebar = new QWidget(centralwidget);
        sidebar->setObjectName(QString::fromUtf8("sidebar"));
        sidebar->setMinimumSize(QSize(200, 0));
        sidebar->setMaximumSize(QSize(200, 16777215));
        sidebarLayout = new QVBoxLayout(sidebar);
        sidebarLayout->setSpacing(8);
        sidebarLayout->setObjectName(QString::fromUtf8("sidebarLayout"));
        sidebarLayout->setContentsMargins(8, 8, 8, 8);
        toggleButton = new QPushButton(sidebar);
        toggleButton->setObjectName(QString::fromUtf8("toggleButton"));
        toggleButton->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img/health.png"), QSize(), QIcon::Normal, QIcon::Off);
        toggleButton->setIcon(icon);
        toggleButton->setIconSize(QSize(24, 24));

        sidebarLayout->addWidget(toggleButton);

        homeButton = new QPushButton(sidebar);
        homeButton->setObjectName(QString::fromUtf8("homeButton"));
        homeButton->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/img/Dashboard.png"), QSize(), QIcon::Normal, QIcon::Off);
        homeButton->setIcon(icon1);
        homeButton->setIconSize(QSize(24, 24));

        sidebarLayout->addWidget(homeButton);

        patientsButton = new QPushButton(sidebar);
        patientsButton->setObjectName(QString::fromUtf8("patientsButton"));
        patientsButton->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/img/Patients.png"), QSize(), QIcon::Normal, QIcon::Off);
        patientsButton->setIcon(icon2);
        patientsButton->setIconSize(QSize(24, 24));

        sidebarLayout->addWidget(patientsButton);

        medicalRecordsButton = new QPushButton(sidebar);
        medicalRecordsButton->setObjectName(QString::fromUtf8("medicalRecordsButton"));
        medicalRecordsButton->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/img/Report.png"), QSize(), QIcon::Normal, QIcon::Off);
        medicalRecordsButton->setIcon(icon3);
        medicalRecordsButton->setIconSize(QSize(24, 24));

        sidebarLayout->addWidget(medicalRecordsButton);

        appointmentsButton = new QPushButton(sidebar);
        appointmentsButton->setObjectName(QString::fromUtf8("appointmentsButton"));
        appointmentsButton->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/img/Appointments.png"), QSize(), QIcon::Normal, QIcon::Off);
        appointmentsButton->setIcon(icon4);
        appointmentsButton->setIconSize(QSize(24, 24));

        sidebarLayout->addWidget(appointmentsButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        sidebarLayout->addItem(verticalSpacer);

        settingsButton = new QPushButton(sidebar);
        settingsButton->setObjectName(QString::fromUtf8("settingsButton"));
        settingsButton->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/img/Settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        settingsButton->setIcon(icon5);
        settingsButton->setIconSize(QSize(24, 24));

        sidebarLayout->addWidget(settingsButton);


        horizontalLayout->addWidget(sidebar);

        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        topbar = new QFrame(frame);
        topbar->setObjectName(QString::fromUtf8("topbar"));
        topbar->setMaximumSize(QSize(16777215, 40));
        topbar->setFrameShape(QFrame::Shape::StyledPanel);
        topbar->setFrameShadow(QFrame::Shadow::Raised);
        horizontalLayout_2 = new QHBoxLayout(topbar);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pushButton_3 = new QPushButton(topbar);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/img/minus.svg"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_3->setIcon(icon6);

        horizontalLayout_2->addWidget(pushButton_3);

        pushButton_2 = new QPushButton(topbar);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/img/maximize.svg"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_2->setIcon(icon7);

        horizontalLayout_2->addWidget(pushButton_2);

        close = new QPushButton(topbar);
        close->setObjectName(QString::fromUtf8("close"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/img/x.svg"), QSize(), QIcon::Normal, QIcon::Off);
        close->setIcon(icon8);

        horizontalLayout_2->addWidget(close);


        verticalLayout->addWidget(topbar);

        content = new QFrame(frame);
        content->setObjectName(QString::fromUtf8("content"));
        content->setFrameShape(QFrame::Shape::StyledPanel);
        content->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_4 = new QVBoxLayout(content);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        contentStackWidget = new QStackedWidget(content);
        contentStackWidget->setObjectName(QString::fromUtf8("contentStackWidget"));
        homePage = new QWidget();
        homePage->setObjectName(QString::fromUtf8("homePage"));
        verticalLayout_2 = new QVBoxLayout(homePage);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        statsGrid = new QGridLayout();
        statsGrid->setObjectName(QString::fromUtf8("statsGrid"));
        mainChartFrame = new QFrame(homePage);
        mainChartFrame->setObjectName(QString::fromUtf8("mainChartFrame"));
        mainChartLayout = new QVBoxLayout(mainChartFrame);
        mainChartLayout->setObjectName(QString::fromUtf8("mainChartLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        mainChartTitle = new QLabel(mainChartFrame);
        mainChartTitle->setObjectName(QString::fromUtf8("mainChartTitle"));

        hboxLayout->addWidget(mainChartTitle);

        spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        op1Button = new QPushButton(mainChartFrame);
        op1Button->setObjectName(QString::fromUtf8("op1Button"));

        hboxLayout->addWidget(op1Button);

        op2Button = new QPushButton(mainChartFrame);
        op2Button->setObjectName(QString::fromUtf8("op2Button"));

        hboxLayout->addWidget(op2Button);


        mainChartLayout->addLayout(hboxLayout);

        mainChartPlaceholder = new QGraphicsView(mainChartFrame);
        mainChartPlaceholder->setObjectName(QString::fromUtf8("mainChartPlaceholder"));
        mainChartPlaceholder->setStyleSheet(QString::fromUtf8(""));

        mainChartLayout->addWidget(mainChartPlaceholder);


        statsGrid->addWidget(mainChartFrame, 0, 0, 1, 2);

        barChartFrame = new QFrame(homePage);
        barChartFrame->setObjectName(QString::fromUtf8("barChartFrame"));
        barChartLayout = new QVBoxLayout(barChartFrame);
        barChartLayout->setObjectName(QString::fromUtf8("barChartLayout"));
        barChartTitle = new QLabel(barChartFrame);
        barChartTitle->setObjectName(QString::fromUtf8("barChartTitle"));

        barChartLayout->addWidget(barChartTitle);

        barChartPlaceholder = new QGraphicsView(barChartFrame);
        barChartPlaceholder->setObjectName(QString::fromUtf8("barChartPlaceholder"));
        barChartPlaceholder->setStyleSheet(QString::fromUtf8(""));

        barChartLayout->addWidget(barChartPlaceholder);


        statsGrid->addWidget(barChartFrame, 0, 2, 1, 2);

        donutChartFrame = new QFrame(homePage);
        donutChartFrame->setObjectName(QString::fromUtf8("donutChartFrame"));
        verticalLayout_5 = new QVBoxLayout(donutChartFrame);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label = new QLabel(donutChartFrame);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_5->addWidget(label);

        donutChartPlaceholder = new QGraphicsView(donutChartFrame);
        donutChartPlaceholder->setObjectName(QString::fromUtf8("donutChartPlaceholder"));
        donutChartPlaceholder->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_5->addWidget(donutChartPlaceholder);


        statsGrid->addWidget(donutChartFrame, 1, 0, 1, 1);

        verticalBarFrame = new QFrame(homePage);
        verticalBarFrame->setObjectName(QString::fromUtf8("verticalBarFrame"));
        verticalLayout_6 = new QVBoxLayout(verticalBarFrame);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label_2 = new QLabel(verticalBarFrame);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_6->addWidget(label_2);

        verticalBarPlaceholder = new QGraphicsView(verticalBarFrame);
        verticalBarPlaceholder->setObjectName(QString::fromUtf8("verticalBarPlaceholder"));
        verticalBarPlaceholder->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_6->addWidget(verticalBarPlaceholder);


        statsGrid->addWidget(verticalBarFrame, 1, 1, 1, 1);


        verticalLayout_2->addLayout(statsGrid);

        contentStackWidget->addWidget(homePage);
        patientsPage = new QWidget();
        patientsPage->setObjectName(QString::fromUtf8("patientsPage"));
        verticalLayout_3 = new QVBoxLayout(patientsPage);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        pageTitle = new QLabel(patientsPage);
        pageTitle->setObjectName(QString::fromUtf8("pageTitle"));
        pageTitle->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);

        verticalLayout_3->addWidget(pageTitle);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        addPatientButton = new QPushButton(patientsPage);
        addPatientButton->setObjectName(QString::fromUtf8("addPatientButton"));

        horizontalLayout_3->addWidget(addPatientButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        searchLineEdit = new QLineEdit(patientsPage);
        searchLineEdit->setObjectName(QString::fromUtf8("searchLineEdit"));

        horizontalLayout_3->addWidget(searchLineEdit);


        verticalLayout_3->addLayout(horizontalLayout_3);

        patientsTableView = new QTableView(patientsPage);
        patientsTableView->setObjectName(QString::fromUtf8("patientsTableView"));

        verticalLayout_3->addWidget(patientsTableView);

        contentStackWidget->addWidget(patientsPage);
        appointmentsPage = new QWidget();
        appointmentsPage->setObjectName(QString::fromUtf8("appointmentsPage"));
        verticalLayout_8 = new QVBoxLayout(appointmentsPage);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        appointmentsPageTitle = new QLabel(appointmentsPage);
        appointmentsPageTitle->setObjectName(QString::fromUtf8("appointmentsPageTitle"));

        verticalLayout_8->addWidget(appointmentsPageTitle);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(12);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        calendarWidget = new QCalendarWidget(appointmentsPage);
        calendarWidget->setObjectName(QString::fromUtf8("calendarWidget"));
        calendarWidget->setLocale(QLocale(QLocale::English, QLocale::FalklandIslands));

        horizontalLayout_4->addWidget(calendarWidget);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        label_6 = new QLabel(appointmentsPage);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setStyleSheet(QString::fromUtf8("font-size: 14pt; font-weight: bold;"));

        verticalLayout_9->addWidget(label_6);

        appointmentsTableView = new QTableView(appointmentsPage);
        appointmentsTableView->setObjectName(QString::fromUtf8("appointmentsTableView"));

        verticalLayout_9->addWidget(appointmentsTableView);

        addAppointmentButton = new QPushButton(appointmentsPage);
        addAppointmentButton->setObjectName(QString::fromUtf8("addAppointmentButton"));

        verticalLayout_9->addWidget(addAppointmentButton);


        horizontalLayout_4->addLayout(verticalLayout_9);

        horizontalSpacer_3 = new QSpacerItem(0, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);


        verticalLayout_8->addLayout(horizontalLayout_4);

        contentStackWidget->addWidget(appointmentsPage);

        verticalLayout_4->addWidget(contentStackWidget);


        verticalLayout->addWidget(content);


        horizontalLayout->addWidget(frame);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        contentStackWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\345\214\273\351\231\242\347\256\241\347\220\206\347\263\273\347\273\237", nullptr));
        toggleButton->setText(QCoreApplication::translate("MainWindow", "\345\214\273\351\231\242\347\263\273\347\273\237", nullptr));
        homeButton->setText(QCoreApplication::translate("MainWindow", "\344\273\252\350\241\250\347\233\230", nullptr));
        patientsButton->setText(QCoreApplication::translate("MainWindow", "\346\202\243\350\200\205\347\256\241\347\220\206", nullptr));
        medicalRecordsButton->setText(QCoreApplication::translate("MainWindow", "\347\227\205\345\216\206\347\256\241\347\220\206", nullptr));
        appointmentsButton->setText(QCoreApplication::translate("MainWindow", "\351\242\204\347\272\246\347\256\241\347\220\206", nullptr));
        settingsButton->setText(QCoreApplication::translate("MainWindow", "\347\263\273\347\273\237\350\256\276\347\275\256", nullptr));
        pushButton_3->setText(QString());
        pushButton_2->setText(QString());
        close->setText(QString());
        mainChartTitle->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#FF8C42;\">\345\277\203\347\224\265\347\233\221\346\212\244</span> \345\256\236\346\227\266\347\233\221\346\265\213</p></body></html>", nullptr));
        op1Button->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\347\233\221\346\212\244", nullptr));
        op2Button->setText(QCoreApplication::translate("MainWindow", "\346\232\202\345\201\234\347\233\221\346\212\244", nullptr));
        barChartTitle->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#FF8C42;\">\345\277\203\347\216\207</span> 72 BPM</p></body></html>", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:700; color:#FF8C42;\">\350\241\200\345\216\213</span><span style=\" font-size:14pt; font-weight:700;\"> 120/80 mmHg</span></p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:700; color:#FF8C42;\">\350\241\200\346\260\247</span><span style=\" font-size:14pt; font-weight:700;\"> 98% SpO2</span></p></body></html>", nullptr));
        pageTitle->setText(QCoreApplication::translate("MainWindow", "\346\202\243\350\200\205\344\277\241\346\201\257\347\256\241\347\220\206", nullptr));
        addPatientButton->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\346\226\260\346\202\243\350\200\205", nullptr));
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\346\214\211\345\247\223\345\220\215\346\210\226\347\274\226\345\217\267\346\220\234\347\264\242\346\202\243\350\200\205...", nullptr));
        appointmentsPageTitle->setText(QCoreApplication::translate("MainWindow", "\351\242\204\347\272\246\346\227\266\351\227\264\345\256\211\346\216\222", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\351\242\204\347\272\246\345\210\227\350\241\250", nullptr));
        addAppointmentButton->setText(QCoreApplication::translate("MainWindow", "\346\226\260\345\273\272\351\242\204\347\272\246", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
