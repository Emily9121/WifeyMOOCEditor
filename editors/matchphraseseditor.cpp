// =============================================================================================
// FILE: editors/matchphraseseditor.cpp
//
// Description: The corrected implementation for our phrase-matching editor!
// =============================================================================================
/*
* File: matchphraseseditor.cpp
* Author: Emily
*
* Description:
* The COMPLETE implementation for our phrase-matching editor!
* Perfect for matching phrase sources to their target endings! 🔗💖
*/

#include "matchphraseseditor.h"
#include "../helpers.h"
#include <QFileDialog>
#include <QMessageBox>

MatchPhrasesEditor::MatchPhrasesEditor(QWidget *parent) : BaseQuestionEditor(parent) 
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
    m_questionTextEdit->setPlaceholderText("Match the phrase beginnings with their perfect endings! 💖");
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


    // Media section  
    auto mediaGroup = new QGroupBox("🎬 Media (Optional) 🎬");
    auto mediaLayout = new QVBoxLayout(mediaGroup);

    auto mediaRowLayout = new QHBoxLayout();
    m_mediaTypeCombo = new QComboBox();
    // 💖 I've added 'Image' here so you can select it from the dropdown! 💖
    m_mediaTypeCombo->addItems({"None", "Video", "Audio", "Image"});

    m_mediaEdit = new QLineEdit();
    m_mediaEdit->setPlaceholderText("Select media file path...");

    auto browseMediaBtn = new QPushButton("Browse 📁");
    connect(browseMediaBtn, &QPushButton::clicked, this, &MatchPhrasesEditor::browseMedia);

    mediaRowLayout->addWidget(new QLabel("Type:"));
    mediaRowLayout->addWidget(m_mediaTypeCombo);
    mediaRowLayout->addWidget(new QLabel("File:"));
    mediaRowLayout->addWidget(m_mediaEdit, 1);
    mediaRowLayout->addWidget(browseMediaBtn);

    mediaLayout->addLayout(mediaRowLayout);
    mainLayout->addWidget(mediaGroup);

    // Phrase pairs section
    auto pairsGroup = new QGroupBox("🔗 Phrase Matching Pairs 🔗");
    auto pairsGroupLayout = new QVBoxLayout(pairsGroup);

    auto pairsLabel = new QLabel("💡 Create source phrases with their possible target endings!");
    pairsLabel->setWordWrap(true);
    pairsLabel->setStyleSheet("font-style: italic; color: #8B008B;");
    pairsGroupLayout->addWidget(pairsLabel);

    QScrollArea *pairsScrollArea = new QScrollArea();
    pairsScrollArea->setWidgetResizable(true);
    QWidget *pairsScrollWidget = new QWidget();
    m_pairsLayout = new QVBoxLayout(pairsScrollWidget);
    pairsScrollArea->setWidget(pairsScrollWidget);

    pairsGroupLayout->addWidget(pairsScrollArea);

    auto addPairButton = new QPushButton("Add Phrase Pair 🔗");
    connect(addPairButton, &QPushButton::clicked, this, &MatchPhrasesEditor::addPair);
    pairsGroupLayout->addWidget(addPairButton);

    // 💖 We're giving this section a stretch factor of 1 so it takes up all the space! 💖
    mainLayout->addWidget(pairsGroup, 1);

    // Initialize with defaults
    m_currentQuestion["type"] = "match_phrases";
    m_currentQuestion["question"] = "";
    m_currentQuestion["pairs"] = QJsonArray{
        QJsonObject{
            {"source", "Beginning of cute phrase 1..."},
            {"targets", QJsonArray{" ", "ending A", "ending B", "ending C"}}
        }
    };
    m_currentQuestion["answer"] = QJsonObject{
        {"Beginning of cute phrase 1...", "ending A"}
    };
    m_currentQuestion["media"] = QJsonValue::Null;

    refreshPairsUI();
}

void MatchPhrasesEditor::loadJson(const QJsonObject& question) 
{
    m_currentQuestion = question;

    m_questionTextEdit->setText(question["question"].toString());

        // ✨ Load the hint text! ✨
    m_hintTextEdit->setText(question["hint"].toString());

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
        // 💖 And our new 'image' type! So cute! 💖
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

    refreshPairsUI();
}

QJsonObject MatchPhrasesEditor::getJson() 
{
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    m_currentQuestion["type"] = "match_phrases";

        // ✨ Save the hint text! ✨
    QString hintText = m_hintTextEdit->toPlainText().trimmed();
    if (!hintText.isEmpty()) {
        m_currentQuestion["hint"] = hintText;
    } else {
        m_currentQuestion.remove("hint");
    }

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

    // Save pairs and generate answer mapping
    QJsonArray pairsArray;
    QJsonObject answerObject;

    for (QWidget* widget : m_pairWidgets) {
        QLineEdit* sourceEdit = widget->findChildren<QLineEdit*>()[0]; // First is source
        QTextEdit* targetsTextEdit = widget->findChild<QTextEdit*>();
        QComboBox* correctAnswerCombo = widget->findChild<QComboBox*>();

        if (sourceEdit && targetsTextEdit && correctAnswerCombo) {
            QString source = sourceEdit->text().trimmed();
            QString targetsText = targetsTextEdit->toPlainText().trimmed();
            QString correctAnswer = correctAnswerCombo->currentText();

            if (!source.isEmpty() && !targetsText.isEmpty()) {
                // Parse targets from text (one per line)
                QStringList targetsList = targetsText.split("\n", Qt::SkipEmptyParts);
                QJsonArray targetsArray;
                for (const QString& target : targetsList) {
                    targetsArray.append(target.trimmed());
                }

                QJsonObject pairObj;
                pairObj["source"] = source;
                pairObj["targets"] = targetsArray;
                pairsArray.append(pairObj);

                // Build answer mapping
                if (!correctAnswer.isEmpty() && correctAnswer != " ") {
                    answerObject[source] = correctAnswer;
                }
            }
        }
    }

    m_currentQuestion["pairs"] = pairsArray;
    m_currentQuestion["answer"] = answerObject;

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

void MatchPhrasesEditor::refreshPairsUI() 
{
    clearPairs();

    QJsonArray pairs = m_currentQuestion["pairs"].toArray();
    QJsonObject answer = m_currentQuestion["answer"].toObject();

    for (int i = 0; i < pairs.size(); ++i) {
        QJsonObject pair = pairs[i].toObject();
        QString source = pair["source"].toString();
        QJsonArray targets = pair["targets"].toArray();

        QWidget* row = new QWidget();
        row->setStyleSheet("QWidget { border: 2px solid #FF69B4; border-radius: 10px; margin: 5px; padding: 10px; }");
        auto layout = new QVBoxLayout(row);

        // Header
        auto headerLayout = new QHBoxLayout();
        auto pairLabel = new QLabel(QString("🔗 Phrase Pair %1 🔗").arg(i + 1));
        pairLabel->setStyleSheet("font-weight: bold; color: #8B008B; font-size: 14px;");

        QPushButton* deleteButton = new QPushButton("Delete Pair 🗑️");
        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray current = m_currentQuestion["pairs"].toArray();
            if (current.size() > 1) {
                current.removeAt(i);
                m_currentQuestion["pairs"] = current;
                refreshPairsUI();
            }
        });

        headerLayout->addWidget(pairLabel);
        headerLayout->addStretch();
        headerLayout->addWidget(deleteButton);
        layout->addLayout(headerLayout);

        // Source phrase input
        auto sourceLayout = new QHBoxLayout();
        sourceLayout->addWidget(new QLabel("📝 Source Phrase:"));
        QLineEdit* sourceEdit = new QLineEdit(source);
        sourceEdit->setPlaceholderText("Beginning of phrase...");
        sourceLayout->addWidget(sourceEdit, 1);
        layout->addLayout(sourceLayout);

        // Target options input
        auto targetsLabel = new QLabel("🎯 Target Options (one per line):");
        targetsLabel->setStyleSheet("font-weight: bold; margin-top: 10px;");
        layout->addWidget(targetsLabel);

        QTextEdit* targetsTextEdit = new QTextEdit();
        QStringList targetsList;
        for (const QJsonValue& targetVal : targets) {
            targetsList.append(targetVal.toString());
        }
        targetsTextEdit->setPlainText(targetsList.join("\n"));
        targetsTextEdit->setPlaceholderText(" \nending A\nending B\nending C");
        layout->addWidget(targetsTextEdit);

        // Correct answer selection
        auto answerLayout = new QHBoxLayout();
        answerLayout->addWidget(new QLabel("✅ Correct Match:"));

        QComboBox* correctAnswerCombo = new QComboBox();
        correctAnswerCombo->setEditable(true);
        for (const QString& target : targetsList) {
            correctAnswerCombo->addItem(target);
        }

        // Set current correct answer
        if (answer.contains(source)) {
            correctAnswerCombo->setCurrentText(answer[source].toString());
        }

        answerLayout->addWidget(correctAnswerCombo, 1);
        layout->addLayout(answerLayout);

        m_pairsLayout->addWidget(row);
        m_pairWidgets.append(row);
    }
}

void MatchPhrasesEditor::addPair() 
{
    QJsonArray pairs = m_currentQuestion["pairs"].toArray();

    QJsonObject newPair;
    newPair["source"] = "New phrase beginning...";
    newPair["targets"] = QJsonArray{" ", "ending A", "ending B", "ending C"};
    pairs.append(newPair);

    m_currentQuestion["pairs"] = pairs;
    refreshPairsUI();
}

void MatchPhrasesEditor::clearPairs()
{
    for (QWidget* widget : m_pairWidgets) {
        m_pairsLayout->removeWidget(widget);
        widget->deleteLater();
    }
    m_pairWidgets.clear();
}

void MatchPhrasesEditor::browseMedia()
{
    QString filter;
    QString mediaType = m_mediaTypeCombo->currentText().toLower();

    if (mediaType == "video") {
        filter = "Video Files (*.mp4 *.avi *.mov *.mkv);;All Files (*)";
    } else if (mediaType == "audio") {
        filter = "Audio Files (*.mp3 *.wav *.ogg *.m4a);;All Files (*)";
    // 💖 I've added our new image filter! 💖
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
