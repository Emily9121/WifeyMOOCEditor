/*
 * File: mainwindow.cpp
 * Author: Emily
 *
 * Description:
 * This file contains the implementation for the main window of the WifeyMOOCEditor.
 * It handles file operations (new, open, save), manages the different question
 * editor widgets, and applies a cute pink theme to the application.
 * It's the heart of our adorable editor! <3
 */

#include "mainwindow.h"
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


#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QStyleFactory>
#include <QDebug>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentEditor(nullptr), centralAreaWidget(new QWidget)
{
    // Set up the main window with a cute title!
    setWindowTitle("💖 Wifey MOOC Editor C++ Edition 💖");
    setMinimumSize(1000, 700);

    // Create the main layout
    mainLayout = new QVBoxLayout(centralAreaWidget);

    // --- Top Bar for selecting question type ---
    QWidget* topBar = new QWidget;
    QHBoxLayout* topBarLayout = new QHBoxLayout(topBar);

    QLabel* questionTypeLabel = new QLabel("Question Type:");
    questionTypeSelector = new QComboBox;
    questionTypeSelector->addItem("Multiple Choice (Single Answer)", "mcq-single");
    questionTypeSelector->addItem("Multiple Choice (Multiple Answers)", "mcq-multiple");
    questionTypeSelector->addItem("Word Fill", "word-fill");
    questionTypeSelector->addItem("Order Phrase", "order-phrase");
    questionTypeSelector->addItem("Match Phrases", "match-phrases");
    questionTypeSelector->addItem("Categorization", "categorization");
    questionTypeSelector->addItem("List Pick", "list-pick");
    questionTypeSelector->addItem("Image Tagging", "image-tagging");
    questionTypeSelector->addItem("Match Sentence", "match-sentence");
    questionTypeSelector->addItem("Sequence Audio", "sequence-audio");
    questionTypeSelector->addItem("Fill in the Blanks (Dropdown)", "fill-blanks-dropdown");
    questionTypeSelector->addItem("Multi-Questions", "multi-questions");

    topBarLayout->addWidget(questionTypeLabel);
    topBarLayout->addWidget(questionTypeSelector);
    mainLayout->addWidget(topBar);

    // --- Editor Area ---
    editorContainer = new QWidget();
    editorLayout = new QVBoxLayout(editorContainer);
    mainLayout->addWidget(editorContainer, 1); // Give it stretch factor

    setCentralWidget(centralAreaWidget);

    // --- Create Actions and Menus ---
    createActions();
    createMenus();
    
    // Connect the selector to a function that changes the editor
    connect(questionTypeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onQuestionTypeChanged);

    // Initialize with a default new editor
    onQuestionTypeChanged(0);
    
    // Apply our super cute stylesheet!
    applyStylesheet();
}

MainWindow::~MainWindow()
{
    // No need to delete currentEditor separately if it has a parent
}

void MainWindow::applyStylesheet()
{
    // A cute pink stylesheet inspired by our Python app! <3
    QString style = R"(
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
    )";
    this->setStyleSheet(style);
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

void MainWindow::newFile()
{
    currentFilePath.clear();
    // Just switch to the default editor type
    questionTypeSelector->setCurrentIndex(0);
    onQuestionTypeChanged(0); 
    setWindowTitle("💖 New Question - Wifey MOOC Editor 💖");
}

void MainWindow::openFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Question File"), "", tr("JSON Files (*.json);;All Files (*)"));
    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open file for reading.");
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    if (doc.isNull() || !doc.isObject()) {
        QMessageBox::warning(this, "Error", "Invalid JSON file.");
        return;
    }

    QJsonObject questionJson = doc.object();
    loadEditorForQuestion(questionJson);
    currentFilePath = filePath;
    setWindowTitle(QString("💖 %1 - Wifey MOOC Editor 💖").arg(QFileInfo(filePath).fileName()));
}

bool MainWindow::saveFile()
{
    if (currentFilePath.isEmpty()) {
        return saveFileAs();
    }
    return saveToFile(currentFilePath);
}

bool MainWindow::saveFileAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Question File"), "", tr("JSON Files (*.json);;All Files (*)"));
    if (filePath.isEmpty()) {
        return false;
    }
    return saveToFile(filePath);
}

bool MainWindow::saveToFile(const QString &filePath)
{
    // Safely cast the current editor to our base class
    BaseQuestionEditor* editor = qobject_cast<BaseQuestionEditor*>(currentEditor);

    if (!editor) {
        QMessageBox::warning(this, "Error", "Cannot save this question type yet, sweetie!");
        return false;
    }

    QJsonObject questionJson = editor->getJson();
    QJsonDocument doc(questionJson);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open file for writing.");
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    currentFilePath = filePath;
    setWindowTitle(QString("💖 %1 - Wifey MOOC Editor 💖").arg(QFileInfo(filePath).fileName()));
    QMessageBox::information(this, "Success!", "File saved successfully, babe! 💕");
    return true;
}


void MainWindow::clearCurrentEditor()
{
    if (currentEditor) {
        editorLayout->removeWidget(currentEditor);
        currentEditor->deleteLater();
        currentEditor = nullptr;
    }
}

void MainWindow::onQuestionTypeChanged(int index)
{
    QString type = questionTypeSelector->itemData(index).toString();
    
    clearCurrentEditor();

    // This big block creates the right editor for the question type!
    if (type == "mcq-single") {
        currentEditor = new MCQSingleEditor(this);
    } else if (type == "mcq-multiple") {
        currentEditor = new MCQMultipleEditor(this);
    } else if (type == "word-fill") {
        currentEditor = new WordFillEditor(this);
    } else if (type == "order-phrase") {
        currentEditor = new OrderPhraseEditor(this);
    } else if (type == "match-phrases") {
        currentEditor = new MatchPhrasesEditor(this);
    } else if (type == "categorization") {
        currentEditor = new CategorizationEditor(this);
    } else if (type == "list-pick") {
        currentEditor = new ListPickEditor(this);
    } else if (type == "image-tagging") {
        currentEditor = new ImageTaggingEditor(this);
    } else if (type == "match-sentence") {
        currentEditor = new MatchSentenceEditor(this);
    } else if (type == "sequence-audio") {
        currentEditor = new SequenceAudioEditor(this);
    } else if (type == "fill-blanks-dropdown") {
        currentEditor = new FillBlanksDropdownEditor(this);
    } else if (type == "multi-questions") {
        currentEditor = new MultiQuestionsEditor(this);
    }
    else {
         // Fallback for any unexpected types, just in case!
        QLabel* placeholder = new QLabel(QString("Editor for '%1' is coming soon, princess! ✨").arg(type));
        placeholder->setAlignment(Qt::AlignCenter);
        currentEditor = placeholder;
    }

    if (currentEditor) {
        editorLayout->addWidget(currentEditor);
    }
}

void MainWindow::loadEditorForQuestion(const QJsonObject &questionJson)
{
    QString type = questionJson["type"].toString();
    if (type.isEmpty()) {
        QMessageBox::warning(this, "Error", "JSON file does not contain a 'type' field.");
        return;
    }

    int index = questionTypeSelector->findData(type);
    if (index != -1) {
        // This will trigger onQuestionTypeChanged to create the right editor
        questionTypeSelector->setCurrentIndex(index);
        
        // Now we cast and load the data into the newly created editor
        if (BaseQuestionEditor* editor = qobject_cast<BaseQuestionEditor*>(currentEditor)) {
            editor->loadJson(questionJson);
        } else {
             qDebug() << "Could not cast current editor to BaseQuestionEditor to load data.";
        }
    } else {
        QMessageBox::warning(this, "Unsupported Type", QString("Question type '%1' is not supported.").arg(type));
    }
}
