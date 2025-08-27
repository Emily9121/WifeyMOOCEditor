/*
* File: mcqmultipleeditor.cpp
* Author: Emily
*
* Description:
* The COMPLETE implementation for our multiple-choice, multiple-answer editor!
* Now with full media support and image handling! ☑️💖
*/

#include "mcqmultipleeditor.h"
#include "../helpers.h"
#include <QFileDialog>
#include <QMessageBox>

McqMultipleEditor::McqMultipleEditor(QWidget *parent) : BaseQuestionEditor(parent) 
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    // Question prompt section
    auto questionGroup = new QGroupBox("❓ Question Prompt ❓");
    auto questionLayout = new QVBoxLayout(questionGroup);

    m_questionTextEdit = new QTextEdit();
    m_questionTextEdit->setPlaceholderText("Which of these things does Emily love most about Sierra? (Choose many!) 💕");
    m_questionTextEdit->setMaximumHeight(100);
    questionLayout->addWidget(m_questionTextEdit);

    mainLayout->addWidget(questionGroup);

    // Media section - for videos and audio! 🎵
    auto mediaGroup = new QGroupBox("🎬 Media (Optional) 🎬");
    auto mediaLayout = new QVBoxLayout(mediaGroup);

    auto mediaRowLayout = new QHBoxLayout();
    m_mediaTypeCombo = new QComboBox();
    m_mediaTypeCombo->addItems({"None", "Video", "Audio"});

    m_mediaEdit = new QLineEdit();
    m_mediaEdit->setPlaceholderText("Select media file path...");

    auto browseMediaBtn = new QPushButton("Browse 📁");
    connect(browseMediaBtn, &QPushButton::clicked, this, &McqMultipleEditor::browseMedia);

    mediaRowLayout->addWidget(new QLabel("Type:"));
    mediaRowLayout->addWidget(m_mediaTypeCombo);
    mediaRowLayout->addWidget(new QLabel("File:"));
    mediaRowLayout->addWidget(m_mediaEdit, 1);
    mediaRowLayout->addWidget(browseMediaBtn);

    mediaLayout->addLayout(mediaRowLayout);
    mainLayout->addWidget(mediaGroup);

    // Answer options section
    auto optionsGroup = new QGroupBox("☑ Answer Options (Pick many!) ☑");
    auto optionsGroupLayout = new QVBoxLayout(optionsGroup);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    QWidget *scrollWidget = new QWidget();
    m_optionsLayout = new QVBoxLayout(scrollWidget);
    m_optionsLayout->setSpacing(10);
    scrollArea->setWidget(scrollWidget);

    optionsGroupLayout->addWidget(scrollArea);

    auto addOptionButton = new QPushButton("Add Adorable Option 💖");
    optionsGroupLayout->addWidget(addOptionButton);

    mainLayout->addWidget(optionsGroup);

    connect(addOptionButton, &QPushButton::clicked, this, &McqMultipleEditor::addOption);

    // Initialize with defaults
    m_currentQuestion["type"] = "mcq_multiple";
    m_currentQuestion["question"] = "";
    m_currentQuestion["options"] = QJsonArray();
    m_currentQuestion["answer"] = QJsonArray{0, 1};
    m_currentQuestion["media"] = QJsonValue::Null;

    // Add some default options to start
    addOption();
    addOption();
}

void McqMultipleEditor::addOption()
{
    createOptionRow();
}

void McqMultipleEditor::createOptionRow(bool correct, const QString &text, const QString &image)
{
    QWidget *optionWidget = new QWidget();
    auto layout = new QVBoxLayout(optionWidget);
    layout->setContentsMargins(5, 5, 5, 5);

    // First row: checkbox and text
    auto topRow = new QHBoxLayout();

    QCheckBox *checkBox = new QCheckBox();
    checkBox->setChecked(correct);

    QLineEdit *textEdit = new QLineEdit(text);
    textEdit->setPlaceholderText("Option text (like: Faire du shopping)...");

    QPushButton *removeButton = new QPushButton("Remove 🗑️");

    topRow->addWidget(checkBox);
    topRow->addWidget(new QLabel("Text:"));
    topRow->addWidget(textEdit, 1);
    topRow->addWidget(removeButton);

    // Second row: image path
    auto bottomRow = new QHBoxLayout();

    QLineEdit *imageEdit = new QLineEdit(image);
    imageEdit->setPlaceholderText("Image path (like: images/35.jpg) - optional");

    QPushButton *browseButton = new QPushButton("Browse Image 🖼️");
    connect(browseButton, &QPushButton::clicked, [this, imageEdit]() {
        browseImage(imageEdit);
    });

    bottomRow->addWidget(new QLabel("Image:"));
    bottomRow->addWidget(imageEdit, 1);
    bottomRow->addWidget(browseButton);

    layout->addLayout(topRow);
    layout->addLayout(bottomRow);

    m_optionsLayout->addWidget(optionWidget);
    m_optionWidgets.append(optionWidget);

    connect(removeButton, &QPushButton::clicked, [this, optionWidget]() {
        if (m_optionWidgets.size() > 1) {
            m_optionsLayout->removeWidget(optionWidget);
            m_optionWidgets.removeOne(optionWidget);
            optionWidget->deleteLater();
        }
    });
}

void McqMultipleEditor::loadJson(const QJsonObject& question) 
{
    m_currentQuestion = question;

    // Load question text
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

    clearOptions();

    QJsonArray options = question["options"].toArray();
    QJsonArray answer = question["answer"].toArray();

    // Convert answer array to set for quick lookup
    QSet<int> correctIndices;
    for (const QJsonValue& ansVal : answer) {
        correctIndices.insert(ansVal.toInt());
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

        createOptionRow(correctIndices.contains(i), text, image);
    }

    // If no options were loaded, add defaults
    if (options.isEmpty()) {
        addOption();
        addOption();
    }
}

QJsonObject McqMultipleEditor::getJson() 
{
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    m_currentQuestion["type"] = "mcq_multiple";

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

    QJsonArray optionsArray;
    QJsonArray answerArray;

    for (int i = 0; i < m_optionWidgets.size(); ++i) {
        QWidget *widget = m_optionWidgets[i];

        QCheckBox *checkBox = widget->findChild<QCheckBox*>();
        QLineEdit *textEdit = widget->findChildren<QLineEdit*>()[0]; // First is text
        QLineEdit *imageEdit = widget->findChildren<QLineEdit*>()[1]; // Second is image

        if (checkBox && textEdit && imageEdit) {
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

            if (checkBox->isChecked()) {
                answerArray.append(i);
            }
        }
    }

    m_currentQuestion["options"] = optionsArray;
    m_currentQuestion["answer"] = answerArray;

    return m_currentQuestion;
}

void McqMultipleEditor::clearOptions()
{
    while (!m_optionWidgets.isEmpty()) {
        QWidget *widget = m_optionWidgets.takeFirst();
        m_optionsLayout->removeWidget(widget);
        widget->deleteLater();
    }
}

void McqMultipleEditor::browseImage(QLineEdit* imageEdit)
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "💖 Select Adorable Image File 💖", "",
        "Image Files (*.png *.jpg *.jpeg *.gif *.bmp);;All Files (*)");

    if (!fileName.isEmpty()) {
        imageEdit->setText(fileName);
    }
}

void McqMultipleEditor::browseMedia()
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
