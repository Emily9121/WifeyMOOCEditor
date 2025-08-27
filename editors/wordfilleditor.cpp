/*
* File: wordfilleditor.cpp
* Author: Emily
*
* Description:
* The COMPLETE implementation for our word-filling editor!
* Perfect for fill-in-the-blanks with sentence parts! 💖
*/

#include "wordfilleditor.h"
#include "../helpers.h"
#include <QFileDialog>
#include <QMessageBox>

WordFillEditor::WordFillEditor(QWidget *parent) : BaseQuestionEditor(parent) 
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    // Question text section
    auto questionGroup = new QGroupBox("❓ Question Prompt ❓");
    auto questionLayout = new QVBoxLayout(questionGroup);

    m_questionTextEdit = new QTextEdit();
    m_questionTextEdit->setPlaceholderText("Fill in the cute blanks! 💕");
    m_questionTextEdit->setMaximumHeight(100);
    questionLayout->addWidget(m_questionTextEdit);

    mainLayout->addWidget(questionGroup);

    // Media section  
    auto mediaGroup = new QGroupBox("🎬 Media (Optional) 🎬");
    auto mediaLayout = new QVBoxLayout(mediaGroup);

    auto mediaRowLayout = new QHBoxLayout();
    m_mediaTypeCombo = new QComboBox();
    m_mediaTypeCombo->addItems({"None", "Video", "Audio"});

    m_mediaEdit = new QLineEdit();
    m_mediaEdit->setPlaceholderText("Select media file path...");

    auto browseMediaBtn = new QPushButton("Browse 📁");
    connect(browseMediaBtn, &QPushButton::clicked, this, &WordFillEditor::browseMedia);

    mediaRowLayout->addWidget(new QLabel("Type:"));
    mediaRowLayout->addWidget(m_mediaTypeCombo);
    mediaRowLayout->addWidget(new QLabel("File:"));
    mediaRowLayout->addWidget(m_mediaEdit, 1);
    mediaRowLayout->addWidget(browseMediaBtn);

    mediaLayout->addLayout(mediaRowLayout);
    mainLayout->addWidget(mediaGroup);

    // Sentence parts section
    auto partsGroup = new QGroupBox("📝 Sentence Parts 📝");
    auto partsGroupLayout = new QVBoxLayout(partsGroup);

    auto partsLabel = new QLabel("💡 Create sentence fragments with blanks between them!");
    partsLabel->setWordWrap(true);
    partsLabel->setStyleSheet("font-style: italic; color: #8B008B;");
    partsGroupLayout->addWidget(partsLabel);

    QScrollArea *partsScrollArea = new QScrollArea();
    partsScrollArea->setWidgetResizable(true);
    QWidget *partsScrollWidget = new QWidget();
    m_partsLayout = new QVBoxLayout(partsScrollWidget);
    partsScrollArea->setWidget(partsScrollWidget);

    partsGroupLayout->addWidget(partsScrollArea);

    auto addPartButton = new QPushButton("Add Sentence Part 📝");
    connect(addPartButton, &QPushButton::clicked, this, &WordFillEditor::addSentencePart);
    partsGroupLayout->addWidget(addPartButton);

    mainLayout->addWidget(partsGroup);

    // Answers section
    auto answersGroup = new QGroupBox("✏️ Fill-in Answers ✏️");
    auto answersGroupLayout = new QVBoxLayout(answersGroup);

    auto answersLabel = new QLabel("💡 The correct words/phrases that fill the blanks!");
    answersLabel->setWordWrap(true);
    answersLabel->setStyleSheet("font-style: italic; color: #8B008B;");
    answersGroupLayout->addWidget(answersLabel);

    QScrollArea *answersScrollArea = new QScrollArea();
    answersScrollArea->setWidgetResizable(true);
    QWidget *answersScrollWidget = new QWidget();
    m_answersLayout = new QVBoxLayout(answersScrollWidget);
    answersScrollArea->setWidget(answersScrollWidget);

    answersGroupLayout->addWidget(answersScrollArea);

    auto addAnswerButton = new QPushButton("Add Answer ✏️");
    connect(addAnswerButton, &QPushButton::clicked, this, &WordFillEditor::addAnswer);
    answersGroupLayout->addWidget(addAnswerButton);

    mainLayout->addWidget(answersGroup);

    // Initialize with defaults
    m_currentQuestion["type"] = "word_fill";
    m_currentQuestion["question"] = "";
    m_currentQuestion["sentence_parts"] = QJsonArray{
        "Fill this ", " with the perfect word ", " please!"
    };
    m_currentQuestion["answers"] = QJsonArray{"blank", "darling"};
    m_currentQuestion["media"] = QJsonValue::Null;

    refreshPartsUI();
    refreshAnswersUI();
}

void WordFillEditor::loadJson(const QJsonObject& question) 
{
    m_currentQuestion = question;

    m_questionTextEdit->setText(question["question"].toString());

    // Load media
    QJsonValue mediaValue = question["media"];
    if (mediaValue.isNull()) {
        m_mediaTypeCombo->setCurrentText("None");
        m_mediaEdit->clear();
    } else {
        QJsonObject media = mediaValue.toObject();
        if (media.contains("video")) {
            m_mediaTypeCombo->setCurrentText("Video");
            m_mediaEdit->setText(media["video"].toString());
        } else if (media.contains("audio")) {
            m_mediaTypeCombo->setCurrentText("Audio");  
            m_mediaEdit->setText(media["audio"].toString());
        }
    }

    refreshPartsUI();
    refreshAnswersUI();
}

QJsonObject WordFillEditor::getJson() 
{
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    m_currentQuestion["type"] = "word_fill";

    // Handle media
    QString mediaType = m_mediaTypeCombo->currentText();
    QString mediaPath = m_mediaEdit->text().trimmed();

    if (mediaType == "None" || mediaPath.isEmpty()) {
        m_currentQuestion["media"] = QJsonValue::Null;
    } else {
        QJsonObject media;
        media[mediaType.toLower()] = mediaPath;
        m_currentQuestion["media"] = media;
    }

    // Save sentence parts
    QJsonArray partsArray;
    for (QWidget* widget : m_partsWidgets) {
        QTextEdit* textEdit = widget->findChild<QTextEdit*>();
        if (textEdit) {
            partsArray.append(textEdit->toPlainText());
        }
    }
    m_currentQuestion["sentence_parts"] = partsArray;

    // Save answers
    QJsonArray answersArray;
    for (QWidget* widget : m_answersWidgets) {
        QLineEdit* lineEdit = widget->findChild<QLineEdit*>();
        if (lineEdit) {
            QString answer = lineEdit->text().trimmed();
            if (!answer.isEmpty()) {
                answersArray.append(answer);
            }
        }
    }
    m_currentQuestion["answers"] = answersArray;

    return m_currentQuestion;
}

void WordFillEditor::refreshPartsUI() 
{
    clearParts();

    QJsonArray parts = m_currentQuestion["sentence_parts"].toArray();
    for (int i = 0; i < parts.size(); ++i) {
        QString partText = parts[i].toString();

        QWidget* row = new QWidget();
        auto layout = new QVBoxLayout(row);

        // Header
        auto headerLayout = new QHBoxLayout();
        auto label = new QLabel(QString("📝 Part %1").arg(i + 1));
        label->setStyleSheet("font-weight: bold; color: #8B008B;");

        QPushButton* deleteButton = new QPushButton("Delete 🗑️");
        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray current = m_currentQuestion["sentence_parts"].toArray();
            if (current.size() > 1) {
                current.removeAt(i);
                m_currentQuestion["sentence_parts"] = current;
                refreshPartsUI();
            }
        });

        headerLayout->addWidget(label);
        headerLayout->addStretch();
        headerLayout->addWidget(deleteButton);
        layout->addLayout(headerLayout);

        // Text input
        QTextEdit* textEdit = new QTextEdit();
        textEdit->setPlainText(partText);
        textEdit->setPlaceholderText("Sentence part text...");
        textEdit->setMaximumHeight(60);
        layout->addWidget(textEdit);

        m_partsLayout->addWidget(row);
        m_partsWidgets.append(row);
    }
}

void WordFillEditor::refreshAnswersUI() 
{
    clearAnswers();

    QJsonArray answers = m_currentQuestion["answers"].toArray();
    for (int i = 0; i < answers.size(); ++i) {
        QString answerText = answers[i].toString();

        QWidget* row = new QWidget();
        auto layout = new QHBoxLayout(row);

        auto label = new QLabel(QString("✏️ Answer %1:").arg(i + 1));
        label->setStyleSheet("font-weight: bold; color: #8B008B;");
        label->setMinimumWidth(80);

        QLineEdit* lineEdit = new QLineEdit(answerText);
        lineEdit->setPlaceholderText("Correct answer...");

        QPushButton* deleteButton = new QPushButton("Delete 🗑️");
        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray current = m_currentQuestion["answers"].toArray();
            if (current.size() > 1) {
                current.removeAt(i);
                m_currentQuestion["answers"] = current;
                refreshAnswersUI();
            }
        });

        layout->addWidget(label);
        layout->addWidget(lineEdit, 1);
        layout->addWidget(deleteButton);

        m_answersLayout->addWidget(row);
        m_answersWidgets.append(row);
    }
}

void WordFillEditor::addSentencePart() 
{
    QJsonArray parts = m_currentQuestion["sentence_parts"].toArray();
    parts.append("new part...");
    m_currentQuestion["sentence_parts"] = parts;
    refreshPartsUI();
}

void WordFillEditor::addAnswer() 
{
    QJsonArray answers = m_currentQuestion["answers"].toArray();
    answers.append("new answer");
    m_currentQuestion["answers"] = answers;
    refreshAnswersUI();
}

void WordFillEditor::clearParts()
{
    for (QWidget* widget : m_partsWidgets) {
        m_partsLayout->removeWidget(widget);
        widget->deleteLater();
    }
    m_partsWidgets.clear();
}

void WordFillEditor::clearAnswers()
{
    for (QWidget* widget : m_answersWidgets) {
        m_answersLayout->removeWidget(widget);
        widget->deleteLater();
    }
    m_answersWidgets.clear();
}

void WordFillEditor::browseMedia()
{
    QString filter;
    QString mediaType = m_mediaTypeCombo->currentText().toLower();

    if (mediaType == "video") {
        filter = "Video Files (*.mp4 *.avi *.mov *.mkv);;All Files (*)";
    } else if (mediaType == "audio") {
        filter = "Audio Files (*.mp3 *.wav *.ogg *.m4a);;All Files (*)";
    } else {
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this,
        "💖 Select Cute Media File 💖", "", filter);

    if (!fileName.isEmpty()) {
        m_mediaEdit->setText(fileName);
    }
}