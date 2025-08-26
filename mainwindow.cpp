#include "mainwindow.h"
#include "basequestioneditor.h"

// ✨ Include all our new, fabulous editor headers! ✨
#include "editors/mcqsingleeditor.h"
#include "editors/mcqmultipleeditor.h"
#include "editors/listpickeditor.h"
#include "editors/wordfilleditor.h"
#include "editors/matchsentenceeditor.h"
#include "editors/orderphraseeditor.h"
#include "editors/categorizationeditor.h"
#include "editors/fillblanksdropdowneditor.h"
#include "editors/sequenceaudioeditor.h"
#include "editors/matchphraseseditor.h"
#include "editors/multiquestionseditor.h"
#include "editors/imagetaggingeditor.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QProcess>
#include <QInputDialog>
#include <QScrollArea>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    setWindowTitle("💖 Wifey MOOC JSON Editor v4.0 (Qt6) 💖");
    setMinimumSize(1200, 800);
    showMaximized();
}

MainWindow::~MainWindow()
{
    cleanupEditors();
}

void MainWindow::setupUi()
{
    auto centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    auto mainLayout = new QVBoxLayout(centralWidget);

    auto headerWidget = new QWidget();
    auto headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setContentsMargins(0,0,0,0);

    auto titleLabel = new QLabel("💖 Wifey MOOC Question Editor 💖");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; padding: 5px;");
    headerLayout->addWidget(titleLabel);

    auto buttonLayout = new QHBoxLayout();
    QStringList buttonLabels = {"✨ New", "💾 Load", "💖 Save", "💖 Save As", "🚀 Save & Launch", "✨ Add Question"};
    for (const auto& label : buttonLabels) {
        buttonLayout->addWidget(new QPushButton(label));
    }
    headerLayout->addLayout(buttonLayout);
    mainLayout->addWidget(headerWidget);
    headerWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    connect(qobject_cast<QPushButton*>(buttonLayout->itemAt(0)->widget()), &QPushButton::clicked, this, &MainWindow::newJson);
    connect(qobject_cast<QPushButton*>(buttonLayout->itemAt(1)->widget()), &QPushButton::clicked, this, &MainWindow::loadJson);
    connect(qobject_cast<QPushButton*>(buttonLayout->itemAt(2)->widget()), &QPushButton::clicked, this, &MainWindow::saveJson);
    connect(qobject_cast<QPushButton*>(buttonLayout->itemAt(3)->widget()), &QPushButton::clicked, this, &MainWindow::saveJsonAs);
    connect(qobject_cast<QPushButton*>(buttonLayout->itemAt(4)->widget()), &QPushButton::clicked, this, &MainWindow::saveAndLaunch);
    connect(qobject_cast<QPushButton*>(buttonLayout->itemAt(5)->widget()), &QPushButton::clicked, this, &MainWindow::addQuestion);
    
    auto splitter = new QSplitter(Qt::Horizontal);
    mainLayout->addWidget(splitter);

    auto leftPanel = new QWidget();
    auto leftLayout = new QVBoxLayout(leftPanel);
    m_questionsListWidget = new QListWidget();
    leftLayout->addWidget(new QLabel("💕 Questions List 💕"));
    leftLayout->addWidget(m_questionsListWidget);
    
    auto listButtonLayout = new QHBoxLayout();
    auto deleteBtn = new QPushButton("🗑️ Delete");
    auto upBtn = new QPushButton("⬆️ Up");
    auto downBtn = new QPushButton("⬇️ Down");
    listButtonLayout->addWidget(deleteBtn);
    listButtonLayout->addWidget(upBtn);
    listButtonLayout->addWidget(downBtn);
    leftLayout->addLayout(listButtonLayout);
    
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::deleteQuestion);
    connect(upBtn, &QPushButton::clicked, this, &MainWindow::moveQuestionUp);
    connect(downBtn, &QPushButton::clicked, this, &MainWindow::moveQuestionDown);
    connect(m_questionsListWidget, &QListWidget::currentRowChanged, this, &MainWindow::onQuestionSelected);

    m_editorStackedWidget = new QStackedWidget();
    m_welcomeWidget = new QWidget();
    auto welcomeLayout = new QVBoxLayout(m_welcomeWidget);
    auto welcomeLabel = new QLabel("💖 Welcome to the Complete Wifey MOOC Editor! 💖\n\nSelect a question or load a file to begin!");
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setWordWrap(true);
    welcomeLabel->setStyleSheet("font-size: 18px;");
    welcomeLayout->addWidget(welcomeLabel);
    m_editorStackedWidget->addWidget(m_welcomeWidget);

    splitter->addWidget(leftPanel);
    splitter->addWidget(m_editorStackedWidget);
    splitter->setSizes({300, 900});
}

void MainWindow::cleanupEditors()
{
    // First, delete the editor widgets themselves from the map
    for (auto const& [key, val] : m_editorWidgets) {
        delete val;
    }
    m_editorWidgets.clear();
    
    // Then, clear the stacked widget (which holds the scroll areas)
    while(m_editorStackedWidget->count() > 1) { // Keep welcome widget
        QWidget* widget = m_editorStackedWidget->widget(1);
        m_editorStackedWidget->removeWidget(widget);
        widget->deleteLater(); // Use deleteLater for safety
    }
}

void MainWindow::refreshQuestionsList()
{
    m_questionsListWidget->blockSignals(true);
    m_questionsListWidget->clear();
    for (int i = 0; i < m_questions.size(); ++i) {
        QJsonObject q = m_questions[i].toObject();
        QString type = q["type"].toString("unknown");
        QString text;
        if (type == "multi_questions") {
            int count = q["questions"].toArray().size();
            text = QString("Multi-Block (%1 questions)").arg(count);
        } else {
            text = q["question"].toString("No question text");
        }

        if (text.length() > 35) {
            text = text.left(35) + "... 💖";
        }
        m_questionsListWidget->addItem(QString("%1. [%2] %3").arg(i + 1).arg(type).arg(text));
    }
    m_questionsListWidget->blockSignals(false);
}

void MainWindow::newJson() {
    if (!m_questions.isEmpty()) {
        auto reply = QMessageBox::question(this, "Create New File?", "All unsaved changes will be lost. Continue?",
                                           QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) return;
    }
    m_questions = QJsonArray();
    m_currentFilePath = "";
    cleanupEditors();
    refreshQuestionsList();
    m_editorStackedWidget->setCurrentWidget(m_welcomeWidget);
    QMessageBox::information(this, "New File", "New JSON file created! Start adding questions, babe!");
}

void MainWindow::loadJson()
{
    QString filePath = QFileDialog::getOpenFileName(this, "💖 Select Your Cute JSON File 💖", "", "JSON Files (*.json)");
    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Oopsie! 😢", "Could not open file!");
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull() || !doc.isArray()) {
        QMessageBox::critical(this, "Oopsie! 😢", "Invalid JSON file, honey. It must be an array of questions.");
        return;
    }

    m_questions = doc.array();
    m_currentFilePath = filePath;
    
    cleanupEditors();
    refreshQuestionsList();
    QMessageBox::information(this, "Success! 💖", QString("Loaded %1 adorable questions!").arg(m_questions.size()));
}

bool MainWindow::saveJson()
{
    if (m_currentFilePath.isEmpty()) {
        return saveJsonAs();
    }

    for(auto const& [index, widget] : m_editorWidgets) {
        auto editor = qobject_cast<BaseQuestionEditor*>(widget);
        if (editor) {
            m_questions[index] = editor->getSavedQuestion();
        }
    }

    QJsonDocument doc(m_questions);
    QFile file(m_currentFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Oopsie! 😢", "Could not save file!");
        return false;
    }
    file.write(doc.toJson(QJsonDocument::Indented));
    QMessageBox::information(this, "Success! 💖", "Questions saved successfully, darling!");
    return true;
}

bool MainWindow::saveJsonAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, "💖 Save Your Beautiful JSON File 💖", "", "JSON Files (*.json)");
    if (filePath.isEmpty()) return false;
    m_currentFilePath = filePath;
    return saveJson();
}

void MainWindow::saveAndLaunch()
{
    if (!saveJson()) return;
    
    QString appPath = "/Applications/WifeyMOOC.app/Contents/MacOS/WifeyMOOC";
    QStringList args;
    args << "-q" << m_currentFilePath;

    if (!QProcess::startDetached(appPath, args)) {
        QMessageBox::critical(this, "Oopsie! 😢", "Failed to launch WifeyMOOC app, babe!");
    } else {
        QMessageBox::information(this, "Launched! 🚀", "WifeyMOOC launched with your file!");
    }
}

void MainWindow::addQuestion()
{
    QStringList questionTypes = {
        "mcq_single", "mcq_multiple", "list_pick", "word_fill", 
        "match_sentence", "order_phrase", "categorization_multiple", 
        "fill_blanks_dropdown", "match_phrases", "multi_questions", 
        "image_tagging", "sequence_audio"
    };
    bool ok;
    QString type = QInputDialog::getItem(this, "✨ Add New Adorable Question ✨", 
                                        "💖 Choose Your Question Type, Princess! 💖", questionTypes, 0, false, &ok);
    if (ok && !type.isEmpty()) {
        QJsonObject newQuestion = getQuestionTemplate(type);
        m_questions.append(newQuestion);
        refreshQuestionsList();
        m_questionsListWidget->setCurrentRow(m_questions.size() - 1);
    }
}

void MainWindow::deleteQuestion()
{
    int index = m_questionsListWidget->currentRow();
    if (index < 0) return;

    auto reply = QMessageBox::question(this, "Confirm Delete 🗑️", "Are you sure you want to delete this question, honey?",
                                       QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        m_questions.removeAt(index);
        
        if (m_editorWidgets.count(index)) {
            QWidget* widget = m_editorWidgets.at(index);
            for(int i = 0; i < m_editorStackedWidget->count(); ++i) {
                auto scrollArea = qobject_cast<QScrollArea*>(m_editorStackedWidget->widget(i));
                if(scrollArea && scrollArea->widget() == widget) {
                    m_editorStackedWidget->removeWidget(scrollArea);
                    delete scrollArea; // This also deletes the editor widget it holds
                    break;
                }
            }
            m_editorWidgets.erase(index);
        }

        refreshQuestionsList();
        m_editorStackedWidget->setCurrentWidget(m_welcomeWidget);
    }
}

void MainWindow::moveQuestionUp()
{
    int index = m_questionsListWidget->currentRow();
    if (index <= 0) return;
    
    const QJsonValue temp = m_questions.at(index);
    m_questions.removeAt(index);
    m_questions.insert(index - 1, temp);

    cleanupEditors();
    refreshQuestionsList();
    m_questionsListWidget->setCurrentRow(index - 1);
}

void MainWindow::moveQuestionDown()
{
    int index = m_questionsListWidget->currentRow();
    if (index < 0 || index >= m_questions.size() - 1) return;

    const QJsonValue temp = m_questions.at(index);
    m_questions.removeAt(index);
    m_questions.insert(index + 1, temp);
    
    cleanupEditors();
    refreshQuestionsList();
    m_questionsListWidget->setCurrentRow(index + 1);
}

void MainWindow::onQuestionSelected(int index)
{
    if (index < 0 || index >= m_questions.size()) {
        m_editorStackedWidget->setCurrentWidget(m_welcomeWidget);
        return;
    }

    QWidget* editorWidgetInStack = nullptr;
    if (m_editorWidgets.count(index)) {
        QWidget* editor = m_editorWidgets.at(index);
        for(int i = 0; i < m_editorStackedWidget->count(); ++i) {
            auto scrollArea = qobject_cast<QScrollArea*>(m_editorStackedWidget->widget(i));
            if(scrollArea && scrollArea->widget() == editor) {
                editorWidgetInStack = m_editorStackedWidget->widget(i);
                break;
            }
        }
    }
    
    if (editorWidgetInStack) {
        m_editorStackedWidget->setCurrentWidget(editorWidgetInStack);
    } else {
        QJsonObject question = m_questions[index].toObject();
        createEditorForQuestion(question, index);
    }
}

void MainWindow::createEditorForQuestion(const QJsonObject& question, int index)
{
    QString type = question["type"].toString();
    BaseQuestionEditor* editor = nullptr;

    if (type == "mcq_single") {
        editor = new McqSingleEditor();
    } else if (type == "mcq_multiple") {
        editor = new McqMultipleEditor();
    } else if (type == "list_pick") {
        editor = new ListPickEditor();
    } else if (type == "word_fill") {
        editor = new WordFillEditor();
    } else if (type == "match_sentence") {
        editor = new MatchSentenceEditor();
    } else if (type == "order_phrase") {
        editor = new OrderPhraseEditor();
    } else if (type == "categorization_multiple") {
        editor = new CategorizationEditor();
    } else if (type == "fill_blanks_dropdown") {
        editor = new FillBlanksDropdownEditor();
    } else if (type == "sequence_audio") {
        editor = new SequenceAudioEditor();
    } else if (type == "match_phrases") {
        editor = new MatchPhrasesEditor();
    } else if (type == "multi_questions") {
        editor = new MultiQuestionsEditor();
    } else if (type == "image_tagging") {
        editor = new ImageTaggingEditor();
    }
    else {
        auto unsupportedWidget = new QWidget();
        auto layout = new QVBoxLayout(unsupportedWidget);
        layout->addWidget(new QLabel(QString("❌ Oopsie! Unknown question type '%1', sweetie!").arg(type)));
        m_editorStackedWidget->addWidget(unsupportedWidget);
        m_editorStackedWidget->setCurrentWidget(unsupportedWidget);
        m_editorWidgets[index] = unsupportedWidget; // Store it so we don't recreate
        return;
    }

    if (editor) {
        auto scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(editor);
        
        editor->loadQuestion(question);
        m_editorStackedWidget->addWidget(scrollArea);
        m_editorStackedWidget->setCurrentWidget(scrollArea);
        m_editorWidgets[index] = editor;
    }
}

QJsonObject MainWindow::getQuestionTemplate(const QString& type)
{
    if (type == "mcq_single") {
        return QJsonObject{
            {"type", "mcq_single"}, {"question", "Choose the best answer, babe! 💕"},
            {"options", QJsonArray{"Option A", "Option B", "Option C"}}, {"answer", QJsonArray{0}},
        };
    }
    if (type == "mcq_multiple") {
        return QJsonObject{
            {"type", "mcq_multiple"}, {"question", "Pick all the right answers, sweetie! 💖"},
            {"options", QJsonArray{"Choice A", "Choice B", "Choice C"}}, {"answer", QJsonArray{0, 1}},
        };
    }
    if (type == "list_pick") {
        return QJsonObject{
            {"type", "list_pick"}, {"question", "Pick all the cute options you want! 💖"},
            {"options", QJsonArray{"Option 1", "Option 2", "Option 3"}}, {"answer", QJsonArray{0}},
        };
    }
    if (type == "word_fill") {
        return QJsonObject{
            {"type", "word_fill"}, {"question", "Fill in the cute blanks! 💕"},
            {"sentence_parts", QJsonArray{"Fill this ", " with the perfect word."}}, {"answers", QJsonArray{"blank"}},
        };
    }
    if (type == "match_sentence") {
        return QJsonObject{
            {"type", "match_sentence"}, {"question", "Match the adorable sentences with images! 💖"},
            {"pairs", QJsonArray{ QJsonObject{{"sentence", "Cute sentence 1"}, {"image_path", "image1.jpg"}},
                QJsonObject{{"sentence", "Sweet sentence 2"}, {"image_path", "image2.jpg"}} }},
            {"answer", QJsonObject{{"image1.jpg", "Cute sentence 1"}, {"image2.jpg", "Sweet sentence 2"}}},
        };
    }
    if (type == "order_phrase") {
        return QJsonObject{
            {"type", "order_phrase"}, {"question", "Put these phrases in the right order, honey! 💕"},
            {"phrase_shuffled", QJsonArray{"Second phrase", "First phrase"}}, {"answer", QJsonArray{"First phrase", "Second phrase"}},
        };
    }
    if (type == "categorization_multiple") {
        return QJsonObject{
            {"type", "categorization_multiple"}, {"question", "Categorize these cute items! 📂"},
            {"stimuli", QJsonArray{ QJsonObject{{"text", "Adorable Item 1"}, {"image", ""}} }},
            {"categories", QJsonArray{" ", "Category A", "Category B"}},
            {"answer", QJsonObject{{"Adorable Item 1", "Category A"}}},
        };
    }
    if (type == "fill_blanks_dropdown") {
        return QJsonObject{
            {"type", "fill_blanks_dropdown"}, {"question", "Choose from the dropdowns, sweetie! ⬇️"},
            {"sentence_parts", QJsonArray{"Choose ", " from the dropdown."}},
            {"options_for_blanks", QJsonArray{ QJsonArray{" ", "option1", "option2"} }},
            {"answers", QJsonArray{"option1"}},
        };
    }
    if (type == "match_phrases") {
        return QJsonObject{
             {"type", "match_phrases"}, {"question", "Match the phrase beginnings with their perfect endings! 💖"},
             {"pairs", QJsonArray{ QJsonObject{{"source", "Beginning..."}, {"targets", QJsonArray{" ", "ending A", "ending B"}}}} },
             {"answer", QJsonObject{{"Beginning...", "ending A"}}},
        };
    }
    if (type == "sequence_audio") {
        return QJsonObject{
            {"type", "sequence_audio"}, {"question", "Put these sweet sounds in order! 🎵"},
            {"audio_options", QJsonArray{ QJsonObject{{"option", "First sound"}}, QJsonObject{{"option", "Second sound"}} }},
            {"answer", QJsonArray{0, 1}},
        };
    }
     if (type == "multi_questions") {
        return QJsonObject{
            {"type", "multi_questions"}, {"questions", QJsonArray{}},
        };
    }
    if (type == "image_tagging") {
        return QJsonObject{
            {"type", "image_tagging"}, {"question", "Tag the cute image! 💖"},
            {"media", QJsonObject{{"image", "body.jpg"}}},
            {"tags", QJsonArray{ QJsonObject{{"label", "Tag 1"}, {"id", "tag1"}} }},
            {"answer", QJsonObject{{"tag1", QJsonArray{100, 150}}}},
        };
    }
    return QJsonObject{{"type", type}, {"question", "New Question"}};
}
