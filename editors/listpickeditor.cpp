/*
* File: listpickeditor.cpp
* Author: Emily
*
* Description:
* The COMPLETE implementation for our list-picking editor!
* Perfect for multi-select checkbox lists! 📝💖
*/

#include "listpickeditor.h"
#include "../helpers.h"
#include <QFileDialog>
#include <QMessageBox>

ListPickEditor::ListPickEditor(QWidget *parent) : BaseQuestionEditor(parent) 
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
    m_questionTextEdit->setPlaceholderText("Pick all the cute options you want! 💖");
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
    m_mediaTypeCombo->addItems({"None", "Video", "Audio", "Image"});

    m_mediaEdit = new QLineEdit();
    m_mediaEdit->setPlaceholderText("Select media file path...");

    auto browseMediaBtn = new QPushButton("Browse 📁");
    connect(browseMediaBtn, &QPushButton::clicked, this, &ListPickEditor::browseMedia);

    mediaRowLayout->addWidget(new QLabel("Type:"));
    mediaRowLayout->addWidget(m_mediaTypeCombo);
    mediaRowLayout->addWidget(new QLabel("File:"));
    mediaRowLayout->addWidget(m_mediaEdit, 1);
    mediaRowLayout->addWidget(browseMediaBtn);

    mediaLayout->addLayout(mediaRowLayout);
    mainLayout->addWidget(mediaGroup);

    // Options section
    auto optionsGroup = new QGroupBox("☑️ Selectable Options ☑️");
    auto optionsGroupLayout = new QVBoxLayout(optionsGroup);

    auto optionsLabel = new QLabel("💡 Create options that students can select (multiple choice checkboxes)!");
    optionsLabel->setWordWrap(true);
    optionsLabel->setStyleSheet("font-style: italic; color: #8B008B;");
    optionsGroupLayout->addWidget(optionsLabel);

    QScrollArea *optionsScrollArea = new QScrollArea();
    optionsScrollArea->setWidgetResizable(true);
    QWidget *optionsScrollWidget = new QWidget();
    m_optionsLayout = new QVBoxLayout(optionsScrollWidget);
    optionsScrollArea->setWidget(optionsScrollWidget);

    optionsGroupLayout->addWidget(optionsScrollArea);

    auto addOptionButton = new QPushButton("Add Option ☑️");
    connect(addOptionButton, &QPushButton::clicked, this, &ListPickEditor::addOption);
    optionsGroupLayout->addWidget(addOptionButton);

    // 💖 We're giving this section a stretch factor of 1 so it takes up all the space! 💖
    mainLayout->addWidget(optionsGroup, 1);

    // Initialize with defaults
    m_currentQuestion["type"] = "list_pick";
    m_currentQuestion["question"] = "";
    m_currentQuestion["options"] = QJsonArray{"Option 1", "Option 2", "Option 3"};
    m_currentQuestion["answer"] = QJsonArray{0};
    m_currentQuestion["media"] = QJsonValue::Null;

    refreshOptionsUI();
}

void ListPickEditor::loadJson(const QJsonObject& question) 
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

QJsonObject ListPickEditor::getJson() 
{
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    m_currentQuestion["type"] = "list_pick";

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

    // Save options and generate answer array
    QJsonArray optionsArray;
    QJsonArray answerArray;

    for (int i = 0; i < m_optionWidgets.size(); ++i) {
        QWidget* widget = m_optionWidgets[i];
        QCheckBox* checkBox = widget->findChild<QCheckBox*>();
        QLineEdit* lineEdit = widget->findChild<QLineEdit*>();

        if (checkBox && lineEdit) {
            QString optionText = lineEdit->text().trimmed();
            if (!optionText.isEmpty()) {
                optionsArray.append(optionText);

                if (checkBox->isChecked()) {
                    answerArray.append(i);
                }
            }
        }
    }

    m_currentQuestion["options"] = optionsArray;
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

void ListPickEditor::refreshOptionsUI() 
{
    clearOptions();

    QJsonArray options = m_currentQuestion["options"].toArray();
    QJsonArray answerArray = m_currentQuestion["answer"].toArray();

    // Convert answer array to set for quick lookup
    QSet<int> correctIndices;
    for (const QJsonValue& ansVal : answerArray) {
        correctIndices.insert(ansVal.toInt());
    }

    for (int i = 0; i < options.size(); ++i) {
        QString optionText = options[i].toString();

        QWidget* row = new QWidget();
        row->setStyleSheet("QWidget { border: 1px solid #FF69B4; border-radius: 5px; margin: 2px; padding: 5px; }");
        auto layout = new QHBoxLayout(row);

        // Checkbox for "this is a correct answer"
        QCheckBox* checkBox = new QCheckBox();
        checkBox->setChecked(correctIndices.contains(i));
        checkBox->setToolTip("Check if this option should be selected by default");

        // Option text
        QLineEdit* lineEdit = new QLineEdit(optionText);
        lineEdit->setPlaceholderText("Option text...");

        // Delete button
        QPushButton* deleteButton = new QPushButton("Delete 🗑️");
        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray currentOptions = m_currentQuestion["options"].toArray();
            if (currentOptions.size() > 1) {
                currentOptions.removeAt(i);
                m_currentQuestion["options"] = currentOptions;

                // Update answer indices
                QJsonArray currentAnswers = m_currentQuestion["answer"].toArray();
                QJsonArray newAnswers;
                for (const QJsonValue& ansVal : currentAnswers) {
                    int ansIndex = ansVal.toInt();
                    if (ansIndex < i) {
                        newAnswers.append(ansIndex);
                    } else if (ansIndex > i) {
                        newAnswers.append(ansIndex - 1);
                    }
                    // Skip if ansIndex == i (the deleted option)
                }
                m_currentQuestion["answer"] = newAnswers;

                refreshOptionsUI();
            }
        });

        layout->addWidget(checkBox);
        layout->addWidget(new QLabel("✅"));
        layout->addWidget(lineEdit, 1);
        layout->addWidget(deleteButton);

        m_optionsLayout->addWidget(row);
        m_optionWidgets.append(row);
    }
}

void ListPickEditor::addOption() 
{
    QJsonArray options = m_currentQuestion["options"].toArray();
    options.append("New cute option");
    m_currentQuestion["options"] = options;
    refreshOptionsUI();
}

void ListPickEditor::clearOptions()
{
    for (QWidget* widget : m_optionWidgets) {
        m_optionsLayout->removeWidget(widget);
        widget->deleteLater();
    }
    m_optionWidgets.clear();
}

void ListPickEditor::browseMedia()
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
