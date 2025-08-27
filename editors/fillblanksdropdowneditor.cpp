/*
* File: fillblanksdropdowneditor.cpp  
* Author: Emily
*
* Description:
* The COMPLETE implementation for our fill-blanks dropdown editor!
* Perfect for dropdown-based fill-in-the-blanks! ⬇️💖
*/

#include "fillblanksdropdowneditor.h"
#include "../helpers.h"
#include <QFileDialog>
#include <QMessageBox>

FillBlanksDropdownEditor::FillBlanksDropdownEditor(QWidget *parent) : BaseQuestionEditor(parent) 
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    // Question text section
    auto questionGroup = new QGroupBox("❓ Question Prompt ❓");
    auto questionLayout = new QVBoxLayout(questionGroup);

    m_questionTextEdit = new QTextEdit();
    m_questionTextEdit->setPlaceholderText("Choose from the dropdowns, sweetie! ⬇️");
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
    connect(browseMediaBtn, &QPushButton::clicked, this, &FillBlanksDropdownEditor::browseMedia);

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

    auto partsLabel = new QLabel("💡 Create sentence parts with dropdowns between them (like: 'Choose ', ' and then ', ' from dropdowns.')");
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
    connect(addPartButton, &QPushButton::clicked, this, &FillBlanksDropdownEditor::addSentencePart);
    partsGroupLayout->addWidget(addPartButton);

    mainLayout->addWidget(partsGroup);

    // Dropdown blanks section
    auto blanksGroup = new QGroupBox("⬇️ Dropdown Blanks ⬇️");
    auto blanksGroupLayout = new QVBoxLayout(blanksGroup);

    auto blanksLabel = new QLabel("💡 Configure the dropdown options and correct answers for each blank!");
    blanksLabel->setWordWrap(true);
    blanksLabel->setStyleSheet("font-style: italic; color: #8B008B;");
    blanksGroupLayout->addWidget(blanksLabel);

    QScrollArea *blanksScrollArea = new QScrollArea();
    blanksScrollArea->setWidgetResizable(true);
    QWidget *blanksScrollWidget = new QWidget();
    m_blanksLayout = new QVBoxLayout(blanksScrollWidget);
    blanksScrollArea->setWidget(blanksScrollWidget);

    blanksGroupLayout->addWidget(blanksScrollArea);

    auto addBlankButton = new QPushButton("Add Dropdown Blank ⬇️");
    connect(addBlankButton, &QPushButton::clicked, this, &FillBlanksDropdownEditor::addBlank);
    blanksGroupLayout->addWidget(addBlankButton);

    mainLayout->addWidget(blanksGroup);

    // Initialize with defaults  
    m_currentQuestion["type"] = "fill_blanks_dropdown";
    m_currentQuestion["question"] = "";
    m_currentQuestion["sentence_parts"] = QJsonArray{
        "Choose ", " and then ", " from these cute dropdowns."
    };
    m_currentQuestion["options_for_blanks"] = QJsonArray{
        QJsonArray{" ", "option1", "option2"},
        QJsonArray{" ", "choice1", "choice2"}
    };
    m_currentQuestion["answers"] = QJsonArray{"option1", "choice1"};
    m_currentQuestion["media"] = QJsonValue::Null;

    refreshPartsUI();
    refreshBlanksUI();
}

void FillBlanksDropdownEditor::loadJson(const QJsonObject& question) 
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
    refreshBlanksUI();
}

QJsonObject FillBlanksDropdownEditor::getJson() 
{
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    m_currentQuestion["type"] = "fill_blanks_dropdown";

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

    // Save dropdown options and answers
    QJsonArray optionsForBlanksArray;
    QJsonArray answersArray;

    for (QWidget* widget : m_blanksWidgets) {
        QTextEdit* optionsTextEdit = widget->findChildren<QTextEdit*>()[0]; // First is options
        QComboBox* answerCombo = widget->findChild<QComboBox*>();

        if (optionsTextEdit && answerCombo) {
            // Parse options from text (one per line)
            QString optionsText = optionsTextEdit->toPlainText().trimmed();
            QStringList optionsList = optionsText.split("\n", Qt::SkipEmptyParts);

            QJsonArray optionsForThisBlank;
            for (const QString& option : optionsList) {
                optionsForThisBlank.append(option.trimmed());
            }
            optionsForBlanksArray.append(optionsForThisBlank);

            // Save the correct answer
            answersArray.append(answerCombo->currentText());
        }
    }

    m_currentQuestion["options_for_blanks"] = optionsForBlanksArray;
    m_currentQuestion["answers"] = answersArray;

    return m_currentQuestion;
}

void FillBlanksDropdownEditor::refreshPartsUI() 
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

        // Text input (using QTextEdit to support newlines)
        QTextEdit* textEdit = new QTextEdit();
        textEdit->setPlainText(partText);
        textEdit->setPlaceholderText("Sentence part text (can include \\n for newlines)...");
        textEdit->setMaximumHeight(80);
        layout->addWidget(textEdit);

        m_partsLayout->addWidget(row);
        m_partsWidgets.append(row);
    }
}

void FillBlanksDropdownEditor::refreshBlanksUI() 
{
    clearBlanks();

    QJsonArray optionsForBlanks = m_currentQuestion["options_for_blanks"].toArray();
    QJsonArray answers = m_currentQuestion["answers"].toArray();

    for (int i = 0; i < optionsForBlanks.size(); ++i) {
        QJsonArray optionsArray = optionsForBlanks[i].toArray();
        QString correctAnswer = (i < answers.size()) ? answers[i].toString() : "";

        QWidget* row = new QWidget();
        row->setStyleSheet("QWidget { border: 1px solid #FF69B4; border-radius: 5px; margin: 2px; padding: 5px; }");
        auto layout = new QVBoxLayout(row);

        // Header
        auto headerLayout = new QHBoxLayout();
        auto label = new QLabel(QString("⬇️ Dropdown %1 ⬇️").arg(i + 1));
        label->setStyleSheet("font-weight: bold; color: #8B008B;");

        QPushButton* deleteButton = new QPushButton("Delete 🗑️");
        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray currentOpts = m_currentQuestion["options_for_blanks"].toArray();
            QJsonArray currentAns = m_currentQuestion["answers"].toArray();

            if (currentOpts.size() > 1) {
                currentOpts.removeAt(i);
                if (i < currentAns.size()) {
                    currentAns.removeAt(i);
                }
                m_currentQuestion["options_for_blanks"] = currentOpts;
                m_currentQuestion["answers"] = currentAns;
                refreshBlanksUI();
            }
        });

        headerLayout->addWidget(label);
        headerLayout->addStretch();
        headerLayout->addWidget(deleteButton);
        layout->addLayout(headerLayout);

        // Options input  
        auto optionsLabel = new QLabel("📝 Dropdown Options (one per line):");
        optionsLabel->setStyleSheet("font-weight: bold;");
        layout->addWidget(optionsLabel);

        QTextEdit* optionsTextEdit = new QTextEdit();
        QStringList optionsList;
        for (const QJsonValue& optVal : optionsArray) {
            optionsList.append(optVal.toString());
        }
        optionsTextEdit->setPlainText(optionsList.join("\n"));
        optionsTextEdit->setPlaceholderText(" \nOption 1\nOption 2\nOption 3");
        optionsTextEdit->setMaximumHeight(100);
        layout->addWidget(optionsTextEdit);

        // Correct answer selection
        auto answerLayout = new QHBoxLayout();
        answerLayout->addWidget(new QLabel("✅ Correct Answer:"));

        QComboBox* answerCombo = new QComboBox();
        answerCombo->setEditable(true);
        for (const QString& option : optionsList) {
            answerCombo->addItem(option);
        }
        answerCombo->setCurrentText(correctAnswer);

        answerLayout->addWidget(answerCombo, 1);
        layout->addLayout(answerLayout);

        m_blanksLayout->addWidget(row);
        m_blanksWidgets.append(row);
    }
}

void FillBlanksDropdownEditor::addSentencePart() 
{
    QJsonArray parts = m_currentQuestion["sentence_parts"].toArray();
    parts.append("new part...");
    m_currentQuestion["sentence_parts"] = parts;
    refreshPartsUI();
}

void FillBlanksDropdownEditor::addBlank() 
{
    QJsonArray optionsForBlanks = m_currentQuestion["options_for_blanks"].toArray();
    QJsonArray answers = m_currentQuestion["answers"].toArray();

    optionsForBlanks.append(QJsonArray{" ", "New Option 1", "New Option 2"});
    answers.append("New Option 1");

    m_currentQuestion["options_for_blanks"] = optionsForBlanks;
    m_currentQuestion["answers"] = answers;
    refreshBlanksUI();
}

void FillBlanksDropdownEditor::clearParts()
{
    for (QWidget* widget : m_partsWidgets) {
        m_partsLayout->removeWidget(widget);
        widget->deleteLater();
    }
    m_partsWidgets.clear();
}

void FillBlanksDropdownEditor::clearBlanks()
{
    for (QWidget* widget : m_blanksWidgets) {
        m_blanksLayout->removeWidget(widget);
        widget->deleteLater();
    }
    m_blanksWidgets.clear();
}

void FillBlanksDropdownEditor::browseMedia()
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
