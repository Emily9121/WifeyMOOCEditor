// =============================================================================================
// FILE: editors/mcqsingleeditor.cpp
//
// Description: The corrected implementation for our single-choice editor!
// =============================================================================================
/*
* File: mcqsingleeditor.cpp  
* Author: Emily
*
* Description:
* The COMPLETE implementation for our single-choice editor! Now supports
* both simple string options AND complex image+text options, plus media
* files, just like our perfect Python version! 💕
*/

#include "mcqsingleeditor.h"
#include "../helpers.h"
#include <QFileDialog>
#include <QMessageBox>

MCQSingleEditor::MCQSingleEditor(QWidget *parent) : BaseQuestionEditor(parent)
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

    // Question prompt section - so cute! 💖
    auto questionGroup = new QGroupBox("❓ Question Prompt ❓");
    auto questionLayout = new QVBoxLayout(questionGroup);

    questionPromptEdit = new QTextEdit();
    questionPromptEdit->setPlaceholderText("What does Sierra love most about Emily? 💕");
    questionLayout->addWidget(questionPromptEdit);

    mainLayout->addWidget(questionGroup);

        // ✨ Our new hint section! So cute! ✨
    auto hintGroup = new QGroupBox("💡 Hint (Optional) 💡");
    auto hintLayout = new QVBoxLayout(hintGroup);
    m_hintTextEdit = new QTextEdit();
    m_hintTextEdit->setPlaceholderText("A little hint for your love... 💕");
    m_hintTextEdit->setMaximumHeight(80); // Keep it smol
    hintLayout->addWidget(m_hintTextEdit);
    mainLayout->addWidget(hintGroup);


    // Media section - for videos and audio! 🎵
    auto mediaGroup = new QGroupBox("🎬 Media (Optional) 🎬");
    auto mediaLayout = new QVBoxLayout(mediaGroup);

    auto mediaRowLayout = new QHBoxLayout();
    mediaTypeCombo = new QComboBox();
    // 💖 I've added 'Image' here so you can select it from the dropdown! 💖
    mediaTypeCombo->addItems({"None", "Video", "Audio", "Image"});

    mediaEdit = new QLineEdit();
    mediaEdit->setPlaceholderText("Select media file path...");

    auto browseMediaBtn = new QPushButton("Browse 📁");
    connect(browseMediaBtn, &QPushButton::clicked, this, &MCQSingleEditor::browseMedia);

    mediaRowLayout->addWidget(new QLabel("Type:"));
    mediaRowLayout->addWidget(mediaTypeCombo);
    mediaRowLayout->addWidget(new QLabel("File:"));
    mediaRowLayout->addWidget(mediaEdit, 1);
    mediaRowLayout->addWidget(browseMediaBtn);

    mediaLayout->addLayout(mediaRowLayout);
    mainLayout->addWidget(mediaGroup);

    // Answer options section - the main event! ✨
    auto optionsGroup = new QGroupBox("💎 Answer Options (Pick one!) 💎");  
    auto optionsGroupLayout = new QVBoxLayout(optionsGroup);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    QWidget *scrollWidget = new QWidget();
    optionsLayout = new QVBoxLayout(scrollWidget);
    optionsLayout->setSpacing(10);
    scrollArea->setWidget(scrollWidget);

    optionsGroupLayout->addWidget(scrollArea);

    auto addOptionButton = new QPushButton("Add Cute Option 💖");
    optionsGroupLayout->addWidget(addOptionButton);

    // 💖 We're giving this section a stretch factor of 1 so it takes up all the space! 💖
    mainLayout->addWidget(optionsGroup, 1);

    connect(addOptionButton, &QPushButton::clicked, this, &MCQSingleEditor::addOption);

    // Initialize with default options
    m_currentQuestion["type"] = "mcq_single";
    m_currentQuestion["question"] = "";
    m_currentQuestion["options"] = QJsonArray();
    m_currentQuestion["answer"] = QJsonArray{0};
    m_currentQuestion["media"] = QJsonValue::Null;

    // Add some default options to start
    addOption();
    addOption();
}

void MCQSingleEditor::addOption()
{
    createOptionRow();
}

void MCQSingleEditor::createOptionRow(bool correct, const QString &text, const QString &image)
{
    QWidget *optionWidget = new QWidget();
    auto layout = new QVBoxLayout(optionWidget);
    layout->setContentsMargins(5, 5, 5, 5);

    // First row: radio button and text
    auto topRow = new QHBoxLayout();

    QRadioButton *radioButton = new QRadioButton();
    radioButton->setChecked(correct);

    QLineEdit *textEdit = new QLineEdit(text);
    textEdit->setPlaceholderText("Option text (like: Macaron à la framboise)...");

    QPushButton *removeButton = new QPushButton("Remove 🗑️");

    topRow->addWidget(radioButton);
    topRow->addWidget(new QLabel("Text:"));
    topRow->addWidget(textEdit, 1);
    topRow->addWidget(removeButton);

    // Second row: image path
    auto bottomRow = new QHBoxLayout();

    QLineEdit *imageEdit = new QLineEdit(image);
    imageEdit->setPlaceholderText("Image path (like: images/32.jpg) - optional");

    QPushButton *browseButton = new QPushButton("Browse Image 🖼️");
    connect(browseButton, &QPushButton::clicked, [this, imageEdit]() {
        browseImage(imageEdit);
    });

    bottomRow->addWidget(new QLabel("Image:"));
    bottomRow->addWidget(imageEdit, 1);
    bottomRow->addWidget(browseButton);

    layout->addLayout(topRow);
    layout->addLayout(bottomRow);

    optionsLayout->addWidget(optionWidget);
    optionWidgets.append(optionWidget);

    connect(removeButton, &QPushButton::clicked, [this, optionWidget]() {
        if (optionWidgets.size() > 1) {
            optionsLayout->removeWidget(optionWidget);
            optionWidgets.removeOne(optionWidget);
            optionWidget->deleteLater();
        }
    });
}

void MCQSingleEditor::loadJson(const QJsonObject &question)
{
    m_currentQuestion = question;

    // Load question text
    questionPromptEdit->setText(question["question"].toString());

        // ✨ Load the hint text! ✨
    m_hintTextEdit->setText(question["hint"].toString());

    // Load media
    QJsonValue mediaValue = question["media"];
    if (mediaValue.isNull()) {
        mediaTypeCombo->setCurrentText("None");
        mediaEdit->clear();
    } else {
        QJsonObject media = mediaValue.toObject();
        if (media.contains("video")) {
            mediaTypeCombo->setCurrentText("Video");
            mediaEdit->setText(media["video"].toString());
        } else if (media.contains("audio")) {
            mediaTypeCombo->setCurrentText("Audio");  
            mediaEdit->setText(media["audio"].toString());
        // 💖 And our new 'image' type! So cute! 💖
        } else if (media.contains("image")) {
            mediaTypeCombo->setCurrentText("Image");
            mediaEdit->setText(media["image"].toString());
        }
    }

        // 💖 ADDED: Load the PDF path! 💖
    QJsonObject lessonObj = question["lesson"].toObject();
    if (m_lessonPdfEdit) {
        m_lessonPdfEdit->setText(lessonObj["pdf"].toString());
    }


    clearOptions();

    QJsonArray options = question["options"].toArray();
    QJsonArray answer = question["answer"].toArray();

    int correctAnswerIndex = -1;
    if (!answer.isEmpty()) {
        correctAnswerIndex = answer[0].toInt(-1);
    }

    for (int i = 0; i < options.size(); ++i) {
        QJsonValue optionValue = options[i];

        QString text, image;
        if (optionValue.isString()) {
            // Simple string option
            text = optionValue.toString();
        } else if (optionValue.isObject()) {
            // Complex option with text and image
            QJsonObject optionObj = optionValue.toObject();
            text = optionObj["text"].toString();
            image = optionObj["image"].toString();
        }

        createOptionRow(i == correctAnswerIndex, text, image);
    }

    // If no options were loaded, add defaults
    if (options.isEmpty()) {
        addOption();
        addOption();
    }
}

QJsonObject MCQSingleEditor::getJson()
{
    m_currentQuestion["question"] = questionPromptEdit->toPlainText();
    m_currentQuestion["type"] = "mcq_single";

        // ✨ Save the hint text! ✨
    QString hintText = m_hintTextEdit->toPlainText().trimmed();
    if (!hintText.isEmpty()) {
        m_currentQuestion["hint"] = hintText;
    } else {
        m_currentQuestion.remove("hint");
    }

    // Handle media
    QString mediaType = mediaTypeCombo->currentText();
    QString mediaPath = mediaEdit->text().trimmed();

    if (mediaType == "None" || mediaPath.isEmpty()) {
        m_currentQuestion["media"] = QJsonValue::Null;
    } else {
        QJsonObject media;
        media[mediaType.toLower()] = mediaPath;
        m_currentQuestion["media"] = media;
    }

    QJsonArray optionsArray;
    int correctAnswerIndex = -1;

    for (int i = 0; i < optionWidgets.size(); ++i) {
        QWidget *widget = optionWidgets[i];

        QRadioButton *radioButton = widget->findChild<QRadioButton*>();
        QLineEdit *textEdit = widget->findChildren<QLineEdit*>()[0]; // First is text
        QLineEdit *imageEdit = widget->findChildren<QLineEdit*>()[1]; // Second is image

        if (radioButton && textEdit && imageEdit) {
            QString text = textEdit->text().trimmed();
            QString image = imageEdit->text().trimmed();

            if (image.isEmpty()) {
                // Simple string option
                optionsArray.append(text);
            } else {
                // Complex option with text and image
                QJsonObject optionObj;
                optionObj["text"] = text;
                optionObj["image"] = image;
                optionsArray.append(optionObj);
            }

            if (radioButton->isChecked()) {
                correctAnswerIndex = i;
            }
        }
    }

    m_currentQuestion["options"] = optionsArray;
    m_currentQuestion["answer"] = QJsonArray{correctAnswerIndex >= 0 ? correctAnswerIndex : 0};

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

void MCQSingleEditor::clearOptions()
{
    while (!optionWidgets.isEmpty()) {
        QWidget *widget = optionWidgets.takeFirst();
        optionsLayout->removeWidget(widget);
        widget->deleteLater();
    }
}

void MCQSingleEditor::browseImage(QLineEdit* imageEdit)
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "💖 Select Adorable Image File 💖", "",
        "Image Files (*.png *.jpg *.jpeg *.gif *.bmp);;All Files (*)");

    if (!fileName.isEmpty()) {
        imageEdit->setText(fileName);
    }
}

void MCQSingleEditor::browseMedia()
{
    QString filter;
    QString mediaType = mediaTypeCombo->currentText().toLower();

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
        mediaEdit->setText(fileName);
    }
}
