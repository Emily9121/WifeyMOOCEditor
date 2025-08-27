/*
* File: mainwindow.h
* Author: Emily
*
* Description:
* The COMPLETE and PERFECT main window header! ✨
* Now with ALL function declarations to match the implementation!
* Added new slots for adding and deleting questions, and a proper layout variable! 💖
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include <QJsonObject>
#include <QJsonArray>
#include <memory> // 💖 Added for std::unique_ptr! 💖
#include "basequestioneditor.h"

// Forward declarations to keep things super tidy!
class QAction;
class QMenu;
class QComboBox;
class QVBoxLayout;
class QListWidget;
class QListWidgetItem;
class QSplitter;

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
    // 💖 Our new slots to add and delete questions! So cute! 💖
    void onAddQuestion();
    void onDeleteQuestion();
    // Slots for when the user picks a new question type from the dropdown!
    void onQuestionTypeChanged(int index);

    // Slot for when the user clicks a question in our new list!
    void onQuestionSelected(QListWidgetItem *item);

private:
    // Functions to set up our beautiful new UI
    void createActions();
    void createMenus();
    void setupMainLayout();
    void showWelcomeMessage();
    void applyStylesheet();

    // Functions to manage the editors and our new question list
    void clearEditorPanel();
    void loadEditorForQuestion(const QJsonObject &questionJson);
    bool saveToFile(const QString &filePath);
    void refreshQuestionList();
    void saveCurrentQuestion();

    // --- All our lovely UI elements and variables ---

    // The main layout and widgets
    QPushButton *newButton;
    QPushButton *saveButton;
    QPushButton *deleteButton;

    // The dropdown for picking question types
    QComboBox *questionTypeSelector;

    // 💖 Switching to a smart pointer for safe memory management! 💖
    std::unique_ptr<QWidget> currentEditor;

    // Our brand new question list!
    QListWidget *questionListWidget;
    QList<QJsonObject> allQuestions;
    int currentQuestionIndex;

    // File path for saving
    QString currentFilePath;

    // Menus and Actions for the top menu bar
    QMenu *fileMenu;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;

    // 💖 This little variable was hiding! Now it's a proper member! 💖
    QVBoxLayout *mainEditorFrameLayout;
};

#endif // MAINWINDOW_H
