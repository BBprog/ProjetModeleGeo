/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <meshviewerwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_chargement;
    QCheckBox *checkBox_showMesh;
    QPushButton *pushButton_addShape;
    QListWidget *listWidget_shapes;
    QFrame *tools;
    QGridLayout *gridLayout;
    QDoubleSpinBox *spinBox_scale;
    QLabel *label_2;
    QLabel *label;
    QDoubleSpinBox *spinBox_angle;
    QLabel *label_3;
    QSpinBox *spinBox_quality;
    QSpacerItem *verticalSpacer;
    MeshViewerWidget *displayWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(831, 577);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widget_2 = new QWidget(centralWidget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(150);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy);
        widget_2->setMinimumSize(QSize(150, 0));
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setSpacing(4);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(3, 3, 3, 3);
        pushButton_chargement = new QPushButton(widget_2);
        pushButton_chargement->setObjectName(QString::fromUtf8("pushButton_chargement"));
        pushButton_chargement->setMinimumSize(QSize(200, 0));

        verticalLayout->addWidget(pushButton_chargement);

        checkBox_showMesh = new QCheckBox(widget_2);
        checkBox_showMesh->setObjectName(QString::fromUtf8("checkBox_showMesh"));
        checkBox_showMesh->setChecked(true);

        verticalLayout->addWidget(checkBox_showMesh);

        pushButton_addShape = new QPushButton(widget_2);
        pushButton_addShape->setObjectName(QString::fromUtf8("pushButton_addShape"));

        verticalLayout->addWidget(pushButton_addShape);

        listWidget_shapes = new QListWidget(widget_2);
        listWidget_shapes->setObjectName(QString::fromUtf8("listWidget_shapes"));

        verticalLayout->addWidget(listWidget_shapes);

        tools = new QFrame(widget_2);
        tools->setObjectName(QString::fromUtf8("tools"));
        tools->setEnabled(false);
        tools->setFrameShape(QFrame::StyledPanel);
        tools->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(tools);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        spinBox_scale = new QDoubleSpinBox(tools);
        spinBox_scale->setObjectName(QString::fromUtf8("spinBox_scale"));
        spinBox_scale->setDecimals(2);
        spinBox_scale->setMaximum(10.000000000000000);
        spinBox_scale->setSingleStep(0.010000000000000);
        spinBox_scale->setValue(0.010000000000000);

        gridLayout->addWidget(spinBox_scale, 0, 1, 1, 1);

        label_2 = new QLabel(tools);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label = new QLabel(tools);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        spinBox_angle = new QDoubleSpinBox(tools);
        spinBox_angle->setObjectName(QString::fromUtf8("spinBox_angle"));
        spinBox_angle->setDecimals(2);
        spinBox_angle->setMinimum(-180.000000000000000);
        spinBox_angle->setMaximum(180.000000000000000);
        spinBox_angle->setSingleStep(1.000000000000000);

        gridLayout->addWidget(spinBox_angle, 1, 1, 1, 1);

        label_3 = new QLabel(tools);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        spinBox_quality = new QSpinBox(tools);
        spinBox_quality->setObjectName(QString::fromUtf8("spinBox_quality"));
        spinBox_quality->setMinimum(2);
        spinBox_quality->setMaximum(200);

        gridLayout->addWidget(spinBox_quality, 2, 1, 1, 1);


        verticalLayout->addWidget(tools);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addWidget(widget_2);

        displayWidget = new MeshViewerWidget(centralWidget);
        displayWidget->setObjectName(QString::fromUtf8("displayWidget"));

        horizontalLayout->addWidget(displayWidget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 831, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton_chargement->setText(QCoreApplication::translate("MainWindow", "Charger OBJ", nullptr));
        checkBox_showMesh->setText(QCoreApplication::translate("MainWindow", "Show mesh", nullptr));
        pushButton_addShape->setText(QCoreApplication::translate("MainWindow", "Add Shape", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "angle", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "scale", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "quality", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
