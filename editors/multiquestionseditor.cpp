/*
* File: multiquestionseditor.cpp
* Author: Emily
*
* Description: 
* The COMPLETE implementation for our multi-questions editor!
* This creates nested questions with individual editors - so complex and cute! 🌟💖
*/

#include "multiquestionseditor.h"
#include "mcqsingleeditor.h"
#include "mcqmultipleeditor.h" 
#include "wordfilleditor.h"
#include "listpickeditor.h"
#include "sequenceaudioeditor.h"
#include "matchsentenceeditor.h"
#include "orderphraseeditor.h"
#include "categorizationeditor.h"
#include "fillblanksdropdowneditor.h"
#include "matchphraseseditor.h"
#include "imagetaggingeditor.h"
#include <QMessageBox>

MultiQuestionsEditor::MultiQuestionsEditor(QWidget *parent) : BaseQuestionEditor(parent) 
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    // Question text section
    auto questionGroup = new QGroupBox("❓ Multi-Questions Container Prompt ❓");
    auto questionLayout = new QVBoxLayout(questionGroup);

    m_questionTextEdit = new QTextEdit();
    m_questionTextEdit->setPlaceholderText("This container holds multiple adorable questions! 💖");
    m_questionTextEdit->setMaximumHeight(80);
    questionLayout->addWidget(m_questionTextEdit);

    mainLayout->addWidget(questionGroup);

    // Nested questions section
    auto questionsGroup = new QGroupBox("🌟 Nested Questions 🌟");
    auto questionsGroupLayout = new QVBoxLayout(questionsGroup);

    auto questionsLabel = new QLabel("💡 Each nested question gets its own editor! Super advanced! ✨");
    questionsLabel->setWordWrap(true);
    questionsLabel->setStyleSheet("font-style: italic; color: #8B008B;");
    questionsGroupLayout->addWidget(questionsLabel);

    // Scroll area for nested questions
    m_questionsScrollArea = new QScrollArea();
    m_questionsScrollArea->setWidgetResizable(true);
    m_questionsScrollArea->setMinimumHeight(400);
    QWidget *questionsScrollWidget = new QWidget();
    m_questionsLayout = new QVBoxLayout(questionsScrollWidget);
    m_questionsScrollArea->setWidget(questionsScrollWidget);

    questionsGroupLayout->addWidget(m_questionsScrollArea);

    auto addQuestionButton = new QPushButton("Add Nested Question 🌟");
    connect(addQuestionButton, &QPushButton::clicked, this, &MultiQuestionsEditor::addNestedQuestion);
    questionsGroupLayout->addWidget(addQuestionButton);

    mainLayout->addWidget(questionsGroup);

    // Initialize with defaults
    m_currentQuestion["type"] = "multi_questions";
    m_currentQuestion["questions"] = QJsonArray{
        QJsonObject{
            {"type", "mcq_single"},
            {"question", "First cute question! 💖"},
            {"options", QJsonArray{
                QJsonObject{{"image", "image1.jpg"}, {"text", "Option 1"}},
                QJsonObject{{"image", "image2.jpg"}, {"text", "Option 2"}}
            }},
            {"answer", QJsonArray{0}}
        },
        QJsonObject{
            {"type", "mcq_multiple"},  
            {"question", "Second adorable question! 💕"},
            {"options", QJsonArray{
                QJsonObject{{"image", "image3.jpg"}, {"text", "Choice A"}},
                QJsonObject{{"image", "image4.jpg"}, {"text", "Choice B"}}
            }},
            {"answer", QJsonArray{0, 1}}
        }
    };

    refreshQuestionsUI();
}

void MultiQuestionsEditor::loadJson(const QJsonObject& question) 
{
    m_currentQuestion = question;

    // Load container question text (if any)
    m_questionTextEdit->setText(question["question"].toString());

    refreshQuestionsUI();
}

QJsonObject MultiQuestionsEditor::getJson() 
{
    m_currentQuestion["type"] = "multi_questions";

    // Save container question text
    QString containerQuestion = m_questionTextEdit->toPlainText().trimmed();
    if (!containerQuestion.isEmpty()) {
        m_currentQuestion["question"] = containerQuestion;
    }

    // Save all nested questions
    QJsonArray questionsArray;

    for (const NestedQuestionWidget& nestedWidget : m_nestedWidgets) {
        if (nestedWidget.editor) {
            QJsonObject nestedQuestion = nestedWidget.editor->getJson();
            questionsArray.append(nestedQuestion);
        }
    }

    m_currentQuestion["questions"] = questionsArray;

    return m_currentQuestion;
}

void MultiQuestionsEditor::refreshQuestionsUI() 
{
    clearQuestions();

    QJsonArray questions = m_currentQuestion["questions"].toArray();

    for (int i = 0; i < questions.size(); ++i) {
        QJsonObject questionObj = questions[i].toObject();
        QString questionType = questionObj["type"].toString();

        // Create container for this nested question
        QWidget* container = new QWidget();
        container->setStyleSheet("QWidget { border: 2px solid #FF1493; border-radius: 10px; margin: 10px; padding: 15px; background-color: #FFEFD5; }");
        auto containerLayout = new QVBoxLayout(container);

        // Header with type selector and remove button
        auto headerLayout = new QHBoxLayout();

        auto questionLabel = new QLabel(QString("🌟 Question %1 🌟").arg(i + 1));
        questionLabel->setStyleSheet("font-weight: bold; font-size: 16px; color: #8B008B;");

        QComboBox* typeSelector = new QComboBox();
        typeSelector->addItems({
            "mcq_single", "mcq_multiple", "word_fill", "list_pick", 
            "sequence_audio", "match_sentence", "order_phrase", 
            "categorization_multiple", "fill_blanks_dropdown", 
            "match_phrases", "image_tagging"
        });
        typeSelector->setCurrentText(questionType);

        QPushButton* removeButton = new QPushButton("Remove Question 🗑️");
        connect(removeButton, &QPushButton::clicked, [this, i](){
            removeNestedQuestion(i);
        });

        headerLayout->addWidget(questionLabel);
        headerLayout->addWidget(new QLabel("Type:"));
        headerLayout->addWidget(typeSelector);
        headerLayout->addStretch();
        headerLayout->addWidget(removeButton);

        containerLayout->addLayout(headerLayout);

        // Create editor for this question type
        BaseQuestionEditor* editor = createEditorForType(questionType, container);
        if (editor) {
            editor->loadJson(questionObj);
            containerLayout->addWidget(editor);
        } else {
            auto errorLabel = new QLabel(QString("❌ Unsupported question type: %1").arg(questionType));
            errorLabel->setStyleSheet("color: red; font-weight: bold;");
            containerLayout->addWidget(errorLabel);
        }

        // Connect type change
        connect(typeSelector, &QComboBox::currentTextChanged, [this, i](const QString& newType){
            changeQuestionType(i, newType);
        });

        // Store nested widget info
        NestedQuestionWidget nestedWidget;
        nestedWidget.container = container;
        nestedWidget.typeSelector = typeSelector;
        nestedWidget.editor = editor;
        nestedWidget.removeButton = removeButton;
        nestedWidget.index = i;

        m_nestedWidgets.append(nestedWidget);
        m_questionsLayout->addWidget(container);
    }
}

void MultiQuestionsEditor::addNestedQuestion() 
{
    QJsonArray questions = m_currentQuestion["questions"].toArray();

    // Add a default MCQ single question
    QJsonObject newQuestion;
    newQuestion["type"] = "mcq_single";
    newQuestion["question"] = QString("New question %1! 💖").arg(questions.size() + 1);
    newQuestion["options"] = QJsonArray{
        QJsonObject{{"text", "Option A"}},
        QJsonObject{{"text", "Option B"}}
    };
    newQuestion["answer"] = QJsonArray{0};

    questions.append(newQuestion);
    m_currentQuestion["questions"] = questions;

    refreshQuestionsUI();
}

void MultiQuestionsEditor::removeNestedQuestion(int index) 
{
    if (m_nestedWidgets.size() <= 1) {
        QMessageBox::information(this, "Can't Remove", "Multi-questions must have at least one nested question! 💖");
        return;
    }

    QJsonArray questions = m_currentQuestion["questions"].toArray();
    if (index >= 0 && index < questions.size()) {
        questions.removeAt(index);
        m_currentQuestion["questions"] = questions;
        refreshQuestionsUI();
    }
}

void MultiQuestionsEditor::changeQuestionType(int index, const QString& newType) 
{
    QJsonArray questions = m_currentQuestion["questions"].toArray();

    if (index >= 0 && index < questions.size()) {
        QJsonObject oldQuestion = questions[index].toObject();
        QString oldType = oldQuestion["type"].toString();

        if (oldType != newType) {
            // Create a new question with the new type but keep the question text
            QJsonObject newQuestion;
            newQuestion["type"] = newType;
            newQuestion["question"] = oldQuestion["question"].toString();

            // Add default fields based on type
            if (newType == "mcq_single") {
                newQuestion["options"] = QJsonArray{
                    QJsonObject{{"text", "Option A"}},
                    QJsonObject{{"text", "Option B"}}
                };
                newQuestion["answer"] = QJsonArray{0};
            } else if (newType == "mcq_multiple") {
                newQuestion["options"] = QJsonArray{
                    QJsonObject{{"text", "Choice A"}},
                    QJsonObject{{"text", "Choice B"}}
                };
                newQuestion["answer"] = QJsonArray{0, 1};
            } else if (newType == "word_fill") {
                newQuestion["sentence_parts"] = QJsonArray{"Fill this ", " with words!"};
                newQuestion["answers"] = QJsonArray{"blank"};
            } else if (newType == "list_pick") {
                newQuestion["options"] = QJsonArray{"Option 1", "Option 2"};
                newQuestion["answer"] = QJsonArray{0};
            }
            // Add more defaults as needed...

            questions[index] = newQuestion;
            m_currentQuestion["questions"] = questions;
            refreshQuestionsUI();
        }
    }
}

void MultiQuestionsEditor::clearQuestions()
{
    for (const NestedQuestionWidget& nested : m_nestedWidgets) {
        m_questionsLayout->removeWidget(nested.container);
        nested.container->deleteLater();
    }
    m_nestedWidgets.clear();
}

BaseQuestionEditor* MultiQuestionsEditor::createEditorForType(const QString& type, QWidget* parent) 
{
    if (type == "mcq_single") {
        return new MCQSingleEditor(parent);
    } else if (type == "mcq_multiple") {
        return new McqMultipleEditor(parent);
    } else if (type == "word_fill") {
        return new WordFillEditor(parent);
    } else if (type == "list_pick") {
        return new ListPickEditor(parent);
    } else if (type == "sequence_audio") {
        return new SequenceAudioEditor(parent);
    } else if (type == "match_sentence") {
        return new MatchSentenceEditor(parent);
    } else if (type == "order_phrase") {
        return new OrderPhraseEditor(parent);
    } else if (type == "categorization_multiple") {
        return new CategorizationEditor(parent);
    } else if (type == "fill_blanks_dropdown") {
        return new FillBlanksDropdownEditor(parent);
    } else if (type == "match_phrases") {
        return new MatchPhrasesEditor(parent);
    } else if (type == "image_tagging") {
        return new ImageTaggingEditor(parent);
    }

    return nullptr;
}
