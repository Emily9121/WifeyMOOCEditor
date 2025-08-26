/*
 * File: mainwindow.h
 * Author: Emily
 *
 * Description:
 * The header file for our main window! This is like the table of contents
 * that tells the compiler about all the amazing functions and variables
 * our main window has. It's super important! <3
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
#include "basequestioneditor.h"

// Forward declarations to keep things tidy!
class QAction;
class QMenu;
class QComboBox;
class QVBoxLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slots for our file menu actions!
    void newFile();
    void openFile();
    bool saveFile();
    bool saveFileAs();

    // Slot for when the user picks a new question type!
    void onQuestionTypeChanged(int index);

private:
    // Functions to set up our beautiful UI
    void createActions();
    void createMenus();
    void applyStylesheet();

    // Functions to manage the editors
    void clearCurrentEditor();
    void loadEditorForQuestion(const QJsonObject &questionJson);
    bool saveToFile(const QString &filePath);

    // --- All our lovely UI elements and variables ---

    // The main layout and widgets
    QWidget *centralAreaWidget;
    QVBoxLayout *mainLayout;
    QWidget *editorContainer;
    QVBoxLayout *editorLayout;

    // The dropdown for picking question types
    QComboBox *questionTypeSelector;

    // The currently active editor widget
    QWidget *currentEditor;

    // File path for saving
    QString currentFilePath;

    // --- Menus and Actions for the top menu bar ---
    QMenu *fileMenu;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;
};

#endif // MAINWINDOW_H
