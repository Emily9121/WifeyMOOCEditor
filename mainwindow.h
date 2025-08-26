#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <map>

class BaseQuestionEditor;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newJson();
    void loadJson();
    bool saveJson();
    bool saveJsonAs();
    void saveAndLaunch();
    void addQuestion();
    void deleteQuestion();
    void moveQuestionUp();
    void moveQuestionDown();
    void onQuestionSelected(int index);

private:
    void setupUi();
    void refreshQuestionsList();
    void createEditorForQuestion(const QJsonObject& question, int index);
    QJsonObject getQuestionTemplate(const QString& type);
    void cleanupEditors();

    QListWidget* m_questionsListWidget;
    QStackedWidget* m_editorStackedWidget;
    QWidget* m_welcomeWidget;

    QJsonArray m_questions;
    QString m_currentFilePath;

    std::map<int, QWidget*> m_editorWidgets;
};

#endif // MAINWINDOW_H
