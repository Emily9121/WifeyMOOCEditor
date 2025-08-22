#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QScrollArea>
#include <QFrame>
#include <QJsonArray>
#include <QJsonObject>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSplitter>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QApplication>
#include <QProcess>

#include "questiontypes.h"
#include "questioneditor.h"
#include "configdialog.h"
#include "jsonhandler.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newJson();
    void loadJson();
    void saveJson();
    void saveJsonAs();
    void saveAndLaunch();
    void showAddQuestionDialog();
    void onQuestionSelected();
    void deleteQuestion();
    void moveQuestionUp();
    void moveQuestionDown();
    void showConfiguration();

private:
    void setupUI();
    void setupMenuBar();
    void setupColors();
    void refreshQuestionsList();
    void showWelcomeMessage();
    void editQuestion(int index);
    void addQuestion(QuestionType type);
    bool promptUnsavedChanges();

    // UI Components
    QWidget *m_centralWidget;
    QSplitter *m_splitter;
    QFrame *m_leftFrame;
    QFrame *m_rightFrame;
    QListWidget *m_questionsListWidget;
    QScrollArea *m_editorScrollArea;
    QWidget *m_editorContainer;

    // Buttons
    QPushButton *m_newButton;
    QPushButton *m_loadButton;
    QPushButton *m_saveButton;
    QPushButton *m_saveAsButton;
    QPushButton *m_launchButton;
    QPushButton *m_addQuestionButton;
    QPushButton *m_deleteButton;
    QPushButton *m_moveUpButton;
    QPushButton *m_moveDownButton;
    QPushButton *m_configButton;

    // Data
    QJsonArray m_questions;
    QString m_currentFile;
    int m_currentQuestionIndex;
    QuestionEditor *m_currentEditor;

    // Configuration
    QSettings *m_settings;
    QString m_wifeyMoocPath;

    // Colors
    struct Colors {
        QString bg = "#FFB6C1";           // Light pink
        QString secondary = "#FFC0CB";    // Pink
        QString accent = "#FF69B4";       // Hot pink
        QString text = "#8B008B";         // Dark magenta
        QString white = "#FFFFFF";        // White
        QString button = "#FF1493";       // Deep pink
        QString entry = "#FFEFD5";        // Papaya whip
    } m_colors;
};

#endif // MAINWINDOW_H
