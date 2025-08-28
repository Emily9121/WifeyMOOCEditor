#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include <QJsonObject>
#include <QJsonArray>
#include <memory>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include "basequestioneditor.h"

// Forward declarations to keep things super tidy!
class QAction;
class QMenu;
class QComboBox;
class QVBoxLayout;
class QListWidget;
class QListWidgetItem;
class QSplitter;
class QLineEdit;
class QTextEdit;
class QLabel;
class QCheckBox; // For our new offline mode toggle! ✨
class QFrame;    // For showing/hiding UI sections!

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Original slots - untouched and perfect!
    void newFile();
    void openFile();
    bool saveFile();
    bool saveFileAs();
    void onAddQuestion();
    void onDeleteQuestion();
    void onQuestionTypeChanged(int index);
    void onQuestionSelected(QListWidgetItem *item);

    // --- New slots for our super cute AI Assistant! ---
    void showAiAssistantDialog();
    void onAIGenerateClicked();
    void onAIRequestFinished(QNetworkReply *reply);
    void onOfflineModeToggled(bool checked); // For our new offline mode!
    void onProcessPastedJson();            // For processing the pasted text!

private:
    // Original functions - untouched and perfect!
    void createActions();
    void createMenus();
    void setupMainLayout();
    void showWelcomeMessage();
    void applyStylesheet();
    void clearEditorPanel();
    void loadEditorForQuestion(const QJsonObject &questionJson);
    bool saveToFile(const QString &filePath);
    void refreshQuestionList();
    void saveCurrentQuestion();

    // --- New AI helper functions! ---
    void loadPrompts();
    void processAIGeneratedQuestions(const QJsonArray &items);
    QJsonObject transformAiQuestion(const QJsonObject &aiItem);

    // Original UI elements and variables
    QPushButton *newButton;
    QPushButton *saveButton;
    QPushButton *deleteButton;
    QComboBox *questionTypeSelector;
    std::unique_ptr<QWidget> currentEditor;
    QListWidget *questionListWidget;
    QList<QJsonObject> allQuestions;
    int currentQuestionIndex;
    QString currentFilePath;
    QMenu *fileMenu;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;
    QVBoxLayout *mainEditorFrameLayout;

    // --- New AI Assistant members! ---
    QPushButton *aiButton;
    QNetworkAccessManager *aiManager;
    QJsonObject promptTemplates;

    // Pointers to widgets inside the AI dialog
    QDialog* aiDialog = nullptr;
    QLineEdit *aiApiKeyInput;
    QComboBox *aiQuestionTypeCombo;
    QTextEdit *aiTopicTextEdit;
    QLabel *aiStatusLabel;
    QCheckBox *aiOfflineCheckbox;
    QFrame *aiOnlineFrame;
    QFrame *aiOfflineFrame;
    QTextEdit *aiPromptOutputText;
    QTextEdit *aiResponseInputText;
};

#endif // MAINWINDOW_H
