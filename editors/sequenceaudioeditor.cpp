/*
* File: sequenceaudioeditor.cpp
* Author: Emily
*
* Description:
* The COMPLETE implementation for our audio-sequencing editor!
* Perfect for ordering adorable audio clips! 🎵💖
*/

#include "sequenceaudioeditor.h"
#include "../helpers.h"
#include <QFileDialog>
#include <QMessageBox>

SequenceAudioEditor::SequenceAudioEditor(QWidget *parent) : BaseQuestionEditor(parent) 
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    // Question text section
    auto questionGroup = new QGroupBox("❓ Question Prompt ❓");
    auto questionLayout = new QVBoxLayout(questionGroup);

    m_questionTextEdit = new QTextEdit();
    m_questionTextEdit->setPlaceholderText("Put these sweet sounds in order! 🎵");
    m_questionTextEdit->setMaximumHeight(100);
    questionLayout->addWidget(m_questionTextEdit);

    mainLayout->addWidget(questionGroup);

    // Main audio file section
    auto audioGroup = new QGroupBox("🎧 Main Audio File 🎧");
    auto audioLayout = new QVBoxLayout(audioGroup);

    auto audioLabel = new QLabel("💡 Select the main audio file that contains all the sounds to sequence:");
    audioLabel->setWordWrap(true);
    audioLabel->setStyleSheet("font-style: italic; color: #8B008B;");
    audioLayout->addWidget(audioLabel);

    auto audioRowLayout = new QHBoxLayout();
    audioRowLayout->addWidget(new QLabel("Audio File:"));

    m_mainAudioEdit = new QLineEdit();
    m_mainAudioEdit->setPlaceholderText("Select main audio file (like: audios/audio3.mp3)");

    auto browseAudioBtn = new QPushButton("Browse Audio 🎵");
    connect(browseAudioBtn, &QPushButton::clicked, this, &SequenceAudioEditor::browseMainAudio);

    audioRowLayout->addWidget(m_mainAudioEdit, 1);
    audioRowLayout->addWidget(browseAudioBtn);

    audioLayout->addLayout(audioRowLayout);
    mainLayout->addWidget(audioGroup);

    // Audio options section
    auto optionsGroup = new QGroupBox("🎼 Audio Sequence Options 🎼");
    auto optionsGroupLayout = new QVBoxLayout(optionsGroup);

    auto optionsLabel = new QLabel("💡 Describe each sound segment - students will put them in correct order!");
    optionsLabel->setWordWrap(true);
    optionsLabel->setStyleSheet("font-style: italic; color: #8B008B;");
    optionsGroupLayout->addWidget(optionsLabel);

    QScrollArea *optionsScrollArea = new QScrollArea();
    optionsScrollArea->setWidgetResizable(true);
    QWidget *optionsScrollWidget = new QWidget();
    m_optionsLayout = new QVBoxLayout(optionsScrollWidget);
    optionsScrollArea->setWidget(optionsScrollWidget);

    optionsGroupLayout->addWidget(optionsScrollArea);

    auto addOptionButton = new QPushButton("Add Audio Option 🎵");
    connect(addOptionButton, &QPushButton::clicked, this, &SequenceAudioEditor::addAudioOption);
    optionsGroupLayout->addWidget(addOptionButton);

    mainLayout->addWidget(optionsGroup);

    // Initialize with defaults
    m_currentQuestion["type"] = "sequence_audio";
    m_currentQuestion["question"] = "";
    m_currentQuestion["audio_options"] = QJsonArray{
        QJsonObject{{"option", "First lovely sound"}},
        QJsonObject{{"option", "Second amazing sound"}},
        QJsonObject{{"option", "Third beautiful sound"}},
        QJsonObject{{"option", "Fourth perfect sound"}}
    };
    m_currentQuestion["answer"] = QJsonArray{2, 0, 3, 1}; // Correct sequence indices
    m_currentQuestion["media"] = QJsonObject{{"audio", "audios/audio3.mp3"}};

    refreshOptionsUI();
}

void SequenceAudioEditor::loadJson(const QJsonObject& question) 
{
    m_currentQuestion = question;

    m_questionTextEdit->setText(question["question"].toString());

    // Load main audio file
    QJsonObject media = question["media"].toObject();
    m_mainAudioEdit->setText(media["audio"].toString());

    refreshOptionsUI();
}

QJsonObject SequenceAudioEditor::getJson() 
{
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    m_currentQuestion["type"] = "sequence_audio";

    // Save main audio file
    QString audioPath = m_mainAudioEdit->text().trimmed();
    if (!audioPath.isEmpty()) {
        QJsonObject media;
        media["audio"] = audioPath;
        m_currentQuestion["media"] = media;
    } else {
        m_currentQuestion["media"] = QJsonValue::Null;
    }

    // Save audio options in the current order
    QJsonArray audioOptionsArray;
    QJsonArray answerArray;

    for (int i = 0; i < m_optionWidgets.size(); ++i) {
        QWidget* widget = m_optionWidgets[i];
        QLineEdit* lineEdit = widget->findChild<QLineEdit*>();

        if (lineEdit) {
            QString optionText = lineEdit->text().trimmed();
            if (!optionText.isEmpty()) {
                QJsonObject optionObj;
                optionObj["option"] = optionText;
                audioOptionsArray.append(optionObj);

                // The answer array represents the correct sequence
                answerArray.append(i);
            }
        }
    }

    m_currentQuestion["audio_options"] = audioOptionsArray;
    m_currentQuestion["answer"] = answerArray;

    return m_currentQuestion;
}

void SequenceAudioEditor::refreshOptionsUI() 
{
    clearOptions();

    QJsonArray audioOptions = m_currentQuestion["audio_options"].toArray();

    for (int i = 0; i < audioOptions.size(); ++i) {
        QJsonObject optionObj = audioOptions[i].toObject();
        QString optionText = optionObj["option"].toString();

        QWidget* row = new QWidget();
        row->setStyleSheet("QWidget { border: 1px solid #FF69B4; border-radius: 5px; margin: 2px; padding: 5px; }");
        auto layout = new QHBoxLayout(row);

        // Sequence number
        auto orderLabel = new QLabel(QString("🎵 %1.").arg(i + 1));
        orderLabel->setStyleSheet("font-weight: bold; color: #8B008B; font-size: 14px;");
        orderLabel->setMinimumWidth(50);
        layout->addWidget(orderLabel);

        // Option description
        QLineEdit* lineEdit = new QLineEdit(optionText);
        lineEdit->setPlaceholderText("Describe this audio segment...");
        layout->addWidget(lineEdit, 1);

        // Move up button
        QPushButton* upButton = new QPushButton("↑");
        upButton->setMaximumWidth(30);
        upButton->setEnabled(i > 0);
        upButton->setToolTip("Move this sound earlier in sequence");
        connect(upButton, &QPushButton::clicked, [this, i](){
            moveUp(i);
        });
        layout->addWidget(upButton);

        // Move down button
        QPushButton* downButton = new QPushButton("↓");
        downButton->setMaximumWidth(30);
        downButton->setEnabled(i < audioOptions.size() - 1);
        downButton->setToolTip("Move this sound later in sequence");
        connect(downButton, &QPushButton::clicked, [this, i](){
            moveDown(i);
        });
        layout->addWidget(downButton);

        // Delete button
        QPushButton* deleteButton = new QPushButton("🗑️");
        deleteButton->setMaximumWidth(40);
        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray current = m_currentQuestion["audio_options"].toArray();
            if (current.size() > 1) {
                current.removeAt(i);
                m_currentQuestion["audio_options"] = current;
                refreshOptionsUI();
            }
        });
        layout->addWidget(deleteButton);

        m_optionsLayout->addWidget(row);
        m_optionWidgets.append(row);
    }
}

void SequenceAudioEditor::addAudioOption() 
{
    QJsonArray audioOptions = m_currentQuestion["audio_options"].toArray();

    QJsonObject newOption;
    newOption["option"] = "New sweet sound";
    audioOptions.append(newOption);

    m_currentQuestion["audio_options"] = audioOptions;
    refreshOptionsUI();
}

void SequenceAudioEditor::moveUp(int index) 
{
    if (index <= 0) return;

    QJsonArray audioOptions = m_currentQuestion["audio_options"].toArray();

    // Swap with previous
    QJsonValue temp = audioOptions[index];
    audioOptions[index] = audioOptions[index - 1];
    audioOptions[index - 1] = temp;

    m_currentQuestion["audio_options"] = audioOptions;
    refreshOptionsUI();
}

void SequenceAudioEditor::moveDown(int index) 
{
    QJsonArray audioOptions = m_currentQuestion["audio_options"].toArray();
    if (index >= audioOptions.size() - 1) return;

    // Swap with next
    QJsonValue temp = audioOptions[index];
    audioOptions[index] = audioOptions[index + 1];
    audioOptions[index + 1] = temp;

    m_currentQuestion["audio_options"] = audioOptions;
    refreshOptionsUI();
}

void SequenceAudioEditor::clearOptions()
{
    for (QWidget* widget : m_optionWidgets) {
        m_optionsLayout->removeWidget(widget);
        widget->deleteLater();
    }
    m_optionWidgets.clear();
}

void SequenceAudioEditor::browseMainAudio()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "💖 Select Cute Audio File 💖", "",
        "Audio Files (*.mp3 *.wav *.ogg *.m4a);;All Files (*)");

    if (!fileName.isEmpty()) {
        m_mainAudioEdit->setText(fileName);
    }
}
