/*
 * File: mainwindow.cpp
 * Author: Emily
 *
 * Description:
 * The COMPLETE and SAFE main window implementation with the missing onQuestionTypeChanged() slot.
 * Added robust widget management to fix all crashes when switching questions. 💖
 */

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "editors/mcqsingleeditor.h"
#include "editors/mcqmultipleeditor.h"
#include "editors/wordfilleditor.h"
#include "editors/orderphraseeditor.h"
#include "editors/matchphraseseditor.h"
#include "editors/categorizationeditor.h"
#include "editors/listpickeditor.h"
#include "editors/imagetaggingeditor.h"
#include "editors/matchsentenceeditor.h"
#include "editors/sequenceaudioeditor.h"
#include "editors/fillblanksdropdowneditor.h"
#include "editors/multiquestionseditor.h"

#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "helpers.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentQuestionIndex(-1)
{
    setWindowTitle("💖 Wifey MOOC Editor C++ Edition 💖");
    setMinimumSize(1200, 800);
    setupMainLayout();
    createActions();
    createMenus();
    applyStylesheet();

    connect(questionTypeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onQuestionTypeChanged);
    connect(questionListWidget, &QListWidget::currentItemChanged,
            this, &MainWindow::onQuestionSelected);

    showWelcomeMessage();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupMainLayout()
{
    mainSplitter = new QSplitter(Qt::Horizontal, this);

    // Left Panel
    leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(5, 5, 5, 5);
    QLabel *listTitle = new QLabel("💕 Questions List 💕");
    listTitle->setAlignment(Qt::AlignCenter);
    questionListWidget = new QListWidget();
    leftLayout->addWidget(listTitle);
    leftLayout->addWidget(questionListWidget);

    // Right Panel
    rightPanel = new QWidget();
    rightPanelLayout = new QVBoxLayout(rightPanel);
    rightPanelLayout->setContentsMargins(5, 5, 5, 5);

    QWidget *topBar = new QWidget;
    QHBoxLayout *topBarLayout = new QHBoxLayout(topBar);
    QLabel *questionTypeLabel = new QLabel("Question Type:");
    questionTypeSelector = new QComboBox;
    questionTypeSelector->addItem("Multiple Choice (Single Answer)", "mcq_single");
    questionTypeSelector->addItem("Multiple Choice (Multiple Answers)", "mcq_multiple");
    questionTypeSelector->addItem("Word Fill", "word_fill");
    questionTypeSelector->addItem("Order Phrase", "order_phrase");
    questionTypeSelector->addItem("Match Phrases", "match_phrases");
    questionTypeSelector->addItem("Categorization", "categorization_multiple");
    questionTypeSelector->addItem("List Pick", "list_pick");
    questionTypeSelector->addItem("Image Tagging", "image_tagging");
    questionTypeSelector->addItem("Match Sentence", "match_sentence");
    questionTypeSelector->addItem("Sequence Audio", "sequence_audio");
    questionTypeSelector->addItem("Fill in the Blanks (Dropdown)", "fill_blanks_dropdown");
    questionTypeSelector->addItem("Multi-Questions", "multi_questions");

    topBarLayout->addWidget(questionTypeLabel);
    topBarLayout->addWidget(questionTypeSelector);
    rightPanelLayout->addWidget(topBar);

    mainSplitter->addWidget(leftPanel);
    mainSplitter->addWidget(rightPanel);
    mainSplitter->setSizes({250, 950});
    setCentralWidget(mainSplitter);
}

void MainWindow::showWelcomeMessage()
{
    clearEditorPanel();
    // 💖 We now create the QLabel directly on the heap and use the smart pointer to manage it. 💖
    QLabel *welcomeLabel = new QLabel("💖 Welcome to the Wifey MOOC Editor! 💖\n\nLoad a JSON file to start editing,\n or create a new file and add questions!");
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet("font-size: 18pt; color: #8B008B;");
    currentEditor.reset(welcomeLabel);
    rightPanelLayout->addWidget(currentEditor.get(), 1);
}

void MainWindow::openFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Question File"), "", tr("JSON Files (*.json);;All Files (*)"));
    if (filePath.isEmpty())
        return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open file for reading.");
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    if (doc.isNull() || !doc.isArray()) {
        QMessageBox::warning(this, "Error", "Invalid JSON file. File must contain an array of questions.");
        return;
    }

    allQuestions.clear();
    QJsonArray questionArray = doc.array();
    for (const QJsonValue &value : questionArray) {
        if (value.isObject()) {
            allQuestions.append(value.toObject());
        }
    }

    currentFilePath = filePath;
    setWindowTitle(QString("💖 %1 - Wifey MOOC Editor 💖").arg(QFileInfo(filePath).fileName()));

    refreshQuestionList();
    if (!allQuestions.isEmpty()) {
        questionListWidget->setCurrentRow(0);
    } else {
        showWelcomeMessage();
    }
}

void MainWindow::refreshQuestionList()
{
    questionListWidget->blockSignals(true);
    questionListWidget->clear();

    for (int i = 0; i < allQuestions.size(); ++i) {
        QJsonObject q = allQuestions[i];
        QString type = q["type"].toString("unknown");
        QString text = q["question"].toString("No question text.");
        if (text.length() > 30)
            text = text.left(30) + "...";
        questionListWidget->addItem(QString("%1. [%2] %3").arg(i + 1).arg(type).arg(text));
    }
    questionListWidget->blockSignals(false);
}

void MainWindow::onQuestionSelected(QListWidgetItem *item)
{
    if (!item) {
        currentQuestionIndex = -1;
        showWelcomeMessage();
        return;
    }

    saveCurrentQuestion();

    int index = questionListWidget->row(item);
    if (index < 0 || index >= allQuestions.size()) {
        currentQuestionIndex = -1;
        showWelcomeMessage();
        return;
    }

    currentQuestionIndex = index;
    loadEditorForQuestion(allQuestions[index]);
}

void MainWindow::saveCurrentQuestion()
{
    if (currentQuestionIndex < 0 || currentQuestionIndex >= allQuestions.size() || !currentEditor)
        return;

    BaseQuestionEditor *editor = qobject_cast<BaseQuestionEditor *>(currentEditor.get());
    if (!editor)
        return;

    allQuestions[currentQuestionIndex] = editor->getJson();

    QString type = allQuestions[currentQuestionIndex]["type"].toString("unknown");
    QString text = allQuestions[currentQuestionIndex]["question"].toString("No question text.");
    if (text.length() > 30)
        text = text.left(30) + "...";
    
    questionListWidget->item(currentQuestionIndex)->setText(QString("%1. [%2] %3").arg(currentQuestionIndex + 1).arg(type).arg(text));
}

void MainWindow::loadEditorForQuestion(const QJsonObject &questionJson)
{
    QString type = questionJson["type"].toString();
    int index = questionTypeSelector->findData(type);

    if (index == -1) {
        QMessageBox::warning(this, "Unsupported Type",
                             QString("Question type '%1' is not supported.").arg(type));
        showWelcomeMessage();
        return;
    }

    questionTypeSelector->blockSignals(true);
    questionTypeSelector->setCurrentIndex(index);
    questionTypeSelector->blockSignals(false);

    clearEditorPanel();

    // 💖 Now we're creating a new object on the heap and using reset() to assign it to the unique_ptr! 💖
    if (type == "mcq_single")
        currentEditor.reset(new MCQSingleEditor(this));
    else if (type == "mcq_multiple")
        currentEditor.reset(new McqMultipleEditor(this));
    else if (type == "word_fill")
        currentEditor.reset(new WordFillEditor(this));
    else if (type == "order_phrase")
        currentEditor.reset(new OrderPhraseEditor(this));
    else if (type == "match_phrases")
        currentEditor.reset(new MatchPhrasesEditor(this));
    else if (type == "categorization_multiple")
        currentEditor.reset(new CategorizationEditor(this));
    else if (type == "list_pick")
        currentEditor.reset(new ListPickEditor(this));
    else if (type == "image_tagging")
        currentEditor.reset(new ImageTaggingEditor(this));
    else if (type == "match_sentence")
        currentEditor.reset(new MatchSentenceEditor(this));
    else if (type == "sequence_audio")
        currentEditor.reset(new SequenceAudioEditor(this));
    else if (type == "fill_blanks_dropdown")
        currentEditor.reset(new FillBlanksDropdownEditor(this));
    else if (type == "multi_questions")
        currentEditor.reset(new MultiQuestionsEditor(this));
    else {
        QLabel *placeholder = new QLabel(QString("Editor for '%1' coming soon! ✨").arg(type), this);
        placeholder->setAlignment(Qt::AlignCenter);
        currentEditor.reset(placeholder);
    }

    rightPanelLayout->addWidget(currentEditor.get(), 1);

    if (auto editor = dynamic_cast<BaseQuestionEditor *>(currentEditor.get())) {
        editor->loadJson(questionJson);
    } else {
        showWelcomeMessage();
    }
}

void MainWindow::clearEditorPanel()
{
    // The smart pointer manages memory, we just need to remove the widget from the layout.
    rightPanelLayout->removeWidget(currentEditor.get());
    currentEditor.reset();
}

void MainWindow::onQuestionTypeChanged(int index)
{
    clearEditorPanel();

    QString type = questionTypeSelector->itemData(index).toString();

    // 💖 Now we're creating a new object on the heap and using reset() to assign it to the unique_ptr! 💖
    if (type == "mcq_single")
        currentEditor.reset(new MCQSingleEditor(this));
    else if (type == "mcq_multiple")
        currentEditor.reset(new McqMultipleEditor(this));
    else if (type == "word_fill")
        currentEditor.reset(new WordFillEditor(this));
    else if (type == "order_phrase")
        currentEditor.reset(new OrderPhraseEditor(this));
    else if (type == "match_phrases")
        currentEditor.reset(new MatchPhrasesEditor(this));
    else if (type == "categorization_multiple")
        currentEditor.reset(new CategorizationEditor(this));
    else if (type == "list_pick")
        currentEditor.reset(new ListPickEditor(this));
    else if (type == "image_tagging")
        currentEditor.reset(new ImageTaggingEditor(this));
    else if (type == "match_sentence")
        currentEditor.reset(new MatchSentenceEditor(this));
    else if (type == "sequence_audio")
        currentEditor.reset(new SequenceAudioEditor(this));
    else if (type == "fill_blanks_dropdown")
        currentEditor.reset(new FillBlanksDropdownEditor(this));
    else if (type == "multi_questions")
        currentEditor.reset(new MultiQuestionsEditor(this));
    else {
        QLabel *placeholder = new QLabel(QString("Editor for '%1' coming soon! ✨").arg(type), this);
        placeholder->setAlignment(Qt::AlignCenter);
        currentEditor.reset(placeholder);
    }

    rightPanelLayout->addWidget(currentEditor.get(), 1);
}

bool MainWindow::saveFile()
{
    if (currentFilePath.isEmpty())
        return saveFileAs();

    return saveToFile(currentFilePath);
}

bool MainWindow::saveFileAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Question File"), "", tr("JSON Files (*.json);;All Files (*)"));
    if (filePath.isEmpty())
        return false;

    return saveToFile(filePath);
}

bool MainWindow::saveToFile(const QString &filePath)
{
    saveCurrentQuestion();

    QJsonArray questionArray;
    for (const QJsonObject &q : allQuestions) {
        questionArray.append(q);
    }

    QJsonDocument doc(questionArray);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open file for writing.");
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    currentFilePath = filePath;
    setWindowTitle(QString("💖 %1 - Wifey MOOC Editor 💖").arg(QFileInfo(filePath).fileName()));
    QMessageBox::information(this, "Success!", "File saved successfully! 💕");
    return true;
}

void MainWindow::newFile()
{
    if (!allQuestions.isEmpty()) {
        if (QMessageBox::question(this, "New File", "Are you sure you want to create a new file? All unsaved changes will be lost!", QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
            return;
    }

    allQuestions.clear();
    currentFilePath.clear();
    currentQuestionIndex = -1;
    refreshQuestionList();
    showWelcomeMessage();
    setWindowTitle("💖 New Question File - Wifey MOOC Editor 💖");
}

void MainWindow::createActions()
{
    newAction = new QAction(tr("&New"), this);
    newAction->setShortcuts(QKeySequence::New);
    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);

    openAction = new QAction(tr("&Open..."), this);
    openAction->setShortcuts(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcuts(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);

    saveAsAction = new QAction(tr("Save &As..."), this);
    saveAsAction->setShortcuts(QKeySequence::SaveAs);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveFileAs);

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
}

void MainWindow::applyStylesheet()
{
    QString style =R"(
        QMainWindow {
            background-color: #FFB6C1; /* Light Pink */
        }
        QWidget {
            font-family: 'Arial';
            font-size: 12pt;
            color: #8B008B; /* Dark Magenta */
        }
        QMenuBar {
            background-color: #FFC0CB; /* Pink */
        }
        QMenu {
            background-color: #FFC0CB;
        }
        QPushButton {
            background-color: #FF1493; /* Deep Pink */
            color: #FFFFFF;
            border-radius: 5px;
            padding: 8px 12px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #FF69B4; /* Hot Pink */
        }
        QLabel {
            color: #8B008B;
        }
        QLineEdit, QTextEdit, QPlainTextEdit {
            background-color: #FFEFD5; /* Papaya Whip */
            border: 1px solid #FFC0CB;
            border-radius: 5px;
            padding: 5px;
        }
        QComboBox {
            background-color: #FFFFFF;
            border: 1px solid #FFC0CB;
            border-radius: 5px;
            padding: 5px;
        }
        QGroupBox {
            border: 1px solid #FF69B4;
            border-radius: 5px;
            margin-top: 10px;
            font-weight: bold;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top center;
            padding: 0 3px;
            background-color: #FFB6C1;
        }
        QListWidget {
            background-color: #FFEFD5;
            border: 1px solid #FFC0CB;
        }
        QSplitter::handle {
            background-color: #FF69B4;
        }
    )";
    this->setStyleSheet(style);
}
