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

        // ... (rest of the constructor UI setup is the same) ...
    
    // 💖 ADD THIS SNIPPET FOR THE LESSON PDF SECTION 💖
    auto lessonGroup = new QGroupBox("📚 Lesson PDF (Optional) 📚");
    auto lessonLayout = new QVBoxLayout(lessonGroup);
    auto lessonRowLayout = new QHBoxLayout();
    
    m_lessonPdfEdit = new QLineEdit();
    m_lessonPdfEdit->setPlaceholderText("Select the PDF file for this lesson...");
    
    m_lessonPdfButton = new QPushButton("Browse 📁");
    connect(m_lessonPdfButton, &QPushButton::clicked, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "💖 Select Lesson PDF File 💖", "", "PDF Files (*.pdf);;All Files (*)");
        if (!filePath.isEmpty()) {
            m_lessonPdfEdit->setText(filePath);
        }
    });

    lessonRowLayout->addWidget(new QLabel("File:"));
    lessonRowLayout->addWidget(m_lessonPdfEdit, 1);
    lessonRowLayout->addWidget(m_lessonPdfButton);

    lessonLayout->addLayout(lessonRowLayout);
    mainLayout->addWidget(lessonGroup);
    // 💖 END SNIPPET 💖

    // Question text section
    auto questionGroup = new QGroupBox("❓ Question Prompt ❓");
    auto questionLayout = new QVBoxLayout(questionGroup);

    m_questionTextEdit = new QTextEdit();
    m_questionTextEdit->setPlaceholderText("Put these sweet sounds in order! 🎵");
    questionLayout->addWidget(m_questionTextEdit);

    mainLayout->addWidget(questionGroup);

        // ✨ Our new hint section! So cute! ✨
    auto hintGroup = new QGroupBox("💡 Hint (Optional) 💡");
    auto hintLayout = new QVBoxLayout(hintGroup);
    m_hintTextEdit = new QTextEdit();
    m_hintTextEdit->setPlaceholderText("A little hint for your love... 💕");
    m_hintTextEdit->setMaximumHeight(80); // Keep it smol
    hintLayout->addWidget(m_hintTextEdit);
    mainLayout->addWidget(hintGroup);


    // Main audio file section
    // 💖 We've made this a generic media group now to handle all types! 💖
    auto mediaGroup = new QGroupBox("🎧 Main Media File 🎧");
    auto mediaLayout = new QVBoxLayout(mediaGroup);

    auto mediaRowLayout = new QHBoxLayout();

    m_mediaTypeCombo = new QComboBox();
    m_mediaTypeCombo->addItems({"None", "Video", "Audio", "Image"});

    m_mediaEdit = new QLineEdit();
    m_mediaEdit->setPlaceholderText("Select media file path...");

    auto browseMediaBtn = new QPushButton("Browse Media 📁");
    connect(browseMediaBtn, &QPushButton::clicked, this, &SequenceAudioEditor::browseMedia);

    mediaRowLayout->addWidget(new QLabel("Type:"));
    mediaRowLayout->addWidget(m_mediaTypeCombo);
    mediaRowLayout->addWidget(new QLabel("File:"));
    mediaRowLayout->addWidget(m_mediaEdit, 1);
    mediaRowLayout->addWidget(browseMediaBtn);
    
    mediaLayout->addLayout(mediaRowLayout);
    mainLayout->addWidget(mediaGroup);

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

    // 💖 We're giving this section a stretch factor of 1 so it takes up all the space! 💖
    mainLayout->addWidget(optionsGroup, 1);

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

        // ✨ Load the hint text! ✨
    m_hintTextEdit->setText(question["hint"].toString());

    // Load main media file
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
        } else if (media.contains("image")) {
            m_mediaTypeCombo->setCurrentText("Image");
            m_mediaEdit->setText(media["image"].toString());
        }
    }

        // 💖 ADDED: Load the PDF path! 💖
    QJsonObject lessonObj = question["lesson"].toObject();
    if (m_lessonPdfEdit) {
        m_lessonPdfEdit->setText(lessonObj["pdf"].toString());
    }


    refreshOptionsUI();
}

QJsonObject SequenceAudioEditor::getJson() 
{
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    m_currentQuestion["type"] = "sequence_audio";

        // ✨ Save the hint text! ✨
    QString hintText = m_hintTextEdit->toPlainText().trimmed();
    if (!hintText.isEmpty()) {
        m_currentQuestion["hint"] = hintText;
    } else {
        m_currentQuestion.remove("hint");
    }

    // Save main media file
    QString mediaType = m_mediaTypeCombo->currentText();
    QString mediaPath = m_mediaEdit->text().trimmed();

    if (mediaType == "None" || mediaPath.isEmpty()) {
        m_currentQuestion["media"] = QJsonValue::Null;
    } else {
        QJsonObject media;
        media[mediaType.toLower()] = mediaPath;
        m_currentQuestion["media"] = media;
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

        // 💖 ADD THIS SNIPPET TO SAVE THE LESSON PDF 💖
    // Handle lesson PDF
    if (m_lessonPdfEdit) {
        QString pdfPath = m_lessonPdfEdit->text().trimmed();
        if (!pdfPath.isEmpty()) {
            QJsonObject lessonObj;
            lessonObj["pdf"] = pdfPath;
            m_currentQuestion["lesson"] = lessonObj;
        } else {
            m_currentQuestion.remove("lesson");
        }
    }
    // 💖 END SNIPPET 💖


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

void SequenceAudioEditor::browseMedia()
{
    QString filter;
    QString mediaType = m_mediaTypeCombo->currentText().toLower();

    if (mediaType == "video") {
        filter = "Video Files (*.mp4 *.avi *.mov *.mkv);;All Files (*)";
    } else if (mediaType == "audio") {
        filter = "Audio Files (*.mp3 *.wav *.ogg *.m4a);;All Files (*)";
    } else if (mediaType == "image") {
        filter = "Image Files (*.png *.jpg *.jpeg *.gif *.bmp);;All Files (*)";
    } else {
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this,
        "💖 Select Cute Media File 💖", "", filter);

    if (!fileName.isEmpty()) {
        m_mediaEdit->setText(fileName);
    }
}
