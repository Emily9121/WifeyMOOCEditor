#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set up our ultra-cute pink stylesheet! 💕
    QString styleSheet = R"(
        * {
            font-family: 'Comic Sans MS', 'Chalkboard SE', 'Comic Neue', sans-serif;
            color: #8B008B; /* Dark Magenta */
        }
        QMainWindow, QDialog {
            background-color: #FFB6C1; /* Light Pink */
        }
        QFrame, QGroupBox {
             background-color: #FFC0CB; /* Pink */
        }
        QLabel, QRadioButton, QCheckBox {
            background-color: transparent;
        }
        QPushButton {
            background-color: #FF1493; /* Deep Pink */
            color: #000000; /* Black text for readability */
            border: 1px solid #FF69B4;
            border-radius: 8px;
            padding: 8px 12px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #FF69B4; /* Hot Pink */
        }
        QPushButton:pressed {
            background-color: #C71585; /* Medium Violet Red */
        }
        QLineEdit, QTextEdit, QListWidget, QComboBox {
            background-color: #FFEFD5; /* Papaya Whip */
            border: 1px solid #FFC0CB;
            border-radius: 5px;
            padding: 4px;
        }
        QListWidget::item:selected {
            background-color: #FF69B4; /* Hot Pink */
            color: #FFFFFF;
        }
        QGroupBox {
            font-weight: bold;
            font-size: 14px;
            border: 1px solid #FF69B4;
            border-radius: 8px;
            margin-top: 1ex;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top center;
            padding: 0 3px;
            background-color: #FFB6C1;
        }
        QScrollArea {
            border: none;
            background-color: white;
        }
    )";
    a.setStyleSheet(styleSheet);

    MainWindow w;
    w.show();

    return a.exec();
}
