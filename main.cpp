#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application properties for QSettings
    app.setOrganizationName("WifeyMOOC");
    app.setOrganizationDomain("wifeymooc.com");
    app.setApplicationName("JSON Editor");
    app.setApplicationVersion("1.0.0");

    // Create and show main window
    MainWindow window;
    window.show();

    return app.exec();
}
