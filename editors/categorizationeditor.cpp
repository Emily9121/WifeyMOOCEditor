/*
* File: categorizationeditor.cpp
* Author: Emily
*
* Description:
* The COMPLETE implementation for our categorization editor!
* Perfect for sorting cute items into adorable categories! 📂💖
*/

#include "categorizationeditor.h"
#include "../helpers.h" 
#include <QFileDialog>
#include <QMessageBox>

CategorizationEditor::CategorizationEditor(QWidget *parent) : BaseQuestionEditor(parent) 
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    // Question text section
    auto questionGroup = new QGroupBox("❓ Question Prompt ❓");
    auto questionLayout = new QVBoxLayout(questionGroup);

    m_questionTextEdit = new QTextEdit();
    m_questionTextEdit->setPlaceholderText("Categorize these cute items! 📂");
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
    connect(browseMediaBtn, &QPushButton::clicked, this, &CategorizationEditor::browseMedia);

    mediaRowLayout->addWidget(new QLabel("Type:"));
    mediaRowLayout->addWidget(m_mediaTypeCombo);
    mediaRowLayout->addWidget(new QLabel("File:"));
    mediaRowLayout->addWidget(m_mediaEdit, 1);
    mediaRowLayout->addWidget(browseMediaBtn);

    mediaLayout->addLayout(mediaRowLayout);
    mainLayout->addWidget(mediaGroup);

    // Categories section
    auto categoriesGroup = new QGroupBox("📂 Categories 📂");
    auto categoriesGroupLayout = new QVBoxLayout(categoriesGroup);

    auto categoriesLabel = new QLabel("💡 Create categories that students can drag items into!");
    categoriesLabel->setWordWrap(true);
    categoriesLabel->setStyleSheet("font-style: italic; color: #8B008B;");
    categoriesGroupLayout->addWidget(categoriesLabel);

    QScrollArea *categoriesScrollArea = new QScrollArea();
    categoriesScrollArea->setWidgetResizable(true);
    QWidget *categoriesScrollWidget = new QWidget();
    m_categoriesLayout = new QVBoxLayout(categoriesScrollWidget);
    categoriesScrollArea->setWidget(categoriesScrollWidget);

    categoriesGroupLayout->addWidget(categoriesScrollArea);

    auto addCategoryButton = new QPushButton("Add Category 📂");
    connect(addCategoryButton, &QPushButton::clicked, this, &CategorizationEditor::addCategory);
    categoriesGroupLayout->addWidget(addCategoryButton);

    mainLayout->addWidget(categoriesGroup);

    // Stimuli (items to categorize) section
    auto stimuliGroup = new QGroupBox("🎯 Items to Categorize 🎯");
    auto stimuliGroupLayout = new QVBoxLayout(stimuliGroup);

    auto stimuliLabel = new QLabel("💡 Add items that students will drag into categories!");
    stimuliLabel->setWordWrap(true);
    stimuliLabel->setStyleSheet("font-style: italic; color: #8B008B;");
    stimuliGroupLayout->addWidget(stimuliLabel);

    QScrollArea *stimuliScrollArea = new QScrollArea();
    stimuliScrollArea->setWidgetResizable(true);
    QWidget *stimuliScrollWidget = new QWidget();
    m_stimuliLayout = new QVBoxLayout(stimuliScrollWidget);
    stimuliScrollArea->setWidget(stimuliScrollWidget);

    stimuliGroupLayout->addWidget(stimuliScrollArea);

    auto addStimulusButton = new QPushButton("Add Item 🎯");
    connect(addStimulusButton, &QPushButton::clicked, this, &CategorizationEditor::addStimulus);
    stimuliGroupLayout->addWidget(addStimulusButton);

    mainLayout->addWidget(stimuliGroup);

    // Initialize with defaults
    m_currentQuestion["type"] = "categorization_multiple";
    m_currentQuestion["question"] = "";
    m_currentQuestion["stimuli"] = QJsonArray{
        QJsonObject{{"text", "Adorable Item 1"}, {"image", QJsonValue::Null}},
        QJsonObject{{"text", "Sweet Item 2"}, {"image", QJsonValue::Null}}
    };
    m_currentQuestion["categories"] = QJsonArray{" ", "Category A", "Category B"};
    m_currentQuestion["answer"] = QJsonObject{
        {"Adorable Item 1", "Category A"},
        {"Sweet Item 2", "Category B"}
    };
    m_currentQuestion["media"] = QJsonValue::Null;

    refreshCategoriesUI();
    refreshStimuliUI();
}

void CategorizationEditor::loadJson(const QJsonObject& question) 
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

    refreshCategoriesUI();
    refreshStimuliUI();
}

QJsonObject CategorizationEditor::getJson() 
{
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    m_currentQuestion["type"] = "categorization_multiple";

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

    // Save categories
    QJsonArray categoriesArray;
    for (QWidget* widget : m_categoryWidgets) {
        QLineEdit* lineEdit = widget->findChild<QLineEdit*>();
        if (lineEdit) {
            categoriesArray.append(lineEdit->text().trimmed());
        }
    }
    m_currentQuestion["categories"] = categoriesArray;

    // Save stimuli and generate answer mapping
    QJsonArray stimuliArray;
    QJsonObject answerObject;

    for (QWidget* widget : m_stimuliWidgets) {
        QLineEdit* textEdit = widget->findChildren<QLineEdit*>()[0]; // First is text
        QLineEdit* imageEdit = widget->findChildren<QLineEdit*>()[1]; // Second is image
        QComboBox* categoryCombo = widget->findChild<QComboBox*>();

        if (textEdit && imageEdit && categoryCombo) {
            QString text = textEdit->text().trimmed();
            QString image = imageEdit->text().trimmed();
            QString category = categoryCombo->currentText();

            QJsonObject stimulusObj;
            stimulusObj["text"] = text;
            if (image.isEmpty())
                stimulusObj["image"] = QJsonValue::Null;
            else
                stimulusObj["image"] = image;

            stimuliArray.append(stimulusObj);

            // Build answer mapping - use the key that matches the JSON format
            QString answerKey = image.isEmpty() ? text : image;
            if (answerKey.contains("/")) {
                // If it's a path, use just the filename for the key
                answerKey = answerKey.split("/").last();
            }
            answerObject[answerKey] = category;
        }
    }

    m_currentQuestion["stimuli"] = stimuliArray;
    m_currentQuestion["answer"] = answerObject;

    return m_currentQuestion;
}

void CategorizationEditor::refreshCategoriesUI() 
{
    clearCategories();

    QJsonArray categories = m_currentQuestion["categories"].toArray();

    for (int i = 0; i < categories.size(); ++i) {
        QString categoryText = categories[i].toString();

        QWidget* row = new QWidget();
        auto layout = new QHBoxLayout(row);

        auto label = new QLabel(QString("Category %1:").arg(i + 1));
        label->setMinimumWidth(80);

        QLineEdit* lineEdit = new QLineEdit(categoryText);
        lineEdit->setPlaceholderText("Category name...");

        QPushButton* deleteButton = new QPushButton("Delete 🗑️");
        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray current = m_currentQuestion["categories"].toArray();
            if (current.size() > 1) {
                current.removeAt(i);
                m_currentQuestion["categories"] = current;
                refreshCategoriesUI();
                refreshStimuliUI(); // Refresh stimuli to update dropdowns
            }
        });

        layout->addWidget(label);
        layout->addWidget(lineEdit, 1);
        layout->addWidget(deleteButton);

        m_categoriesLayout->addWidget(row);
        m_categoryWidgets.append(row);
    }
}

void CategorizationEditor::refreshStimuliUI() 
{
    clearStimuli();

    // Get current categories for dropdowns
    QStringList categoryList;
    QJsonArray categories = m_currentQuestion["categories"].toArray();
    for (const QJsonValue& catVal : categories) {
        categoryList.append(catVal.toString());
    }

    QJsonArray stimuli = m_currentQuestion["stimuli"].toArray();
    QJsonObject answer = m_currentQuestion["answer"].toObject();

    for (int i = 0; i < stimuli.size(); ++i) {
        QJsonObject stimulus = stimuli[i].toObject();
        QString text = stimulus["text"].toString();
        QString image = stimulus["image"].toString();

        QWidget* row = new QWidget();
        row->setStyleSheet("QWidget { border: 1px solid #FF69B4; border-radius: 5px; margin: 2px; padding: 5px; }");
        auto layout = new QVBoxLayout(row);

        // Header
        auto headerLayout = new QHBoxLayout();
        auto itemLabel = new QLabel(QString("💖 Item %1 💖").arg(i + 1));
        itemLabel->setStyleSheet("font-weight: bold; color: #8B008B;");

        QPushButton* deleteButton = new QPushButton("Delete 🗑️");
        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray current = m_currentQuestion["stimuli"].toArray();
            if (current.size() > 1) {
                current.removeAt(i);
                m_currentQuestion["stimuli"] = current;
                refreshStimuliUI();
            }
        });

        headerLayout->addWidget(itemLabel);
        headerLayout->addStretch();
        headerLayout->addWidget(deleteButton);
        layout->addLayout(headerLayout);

        // Text input
        auto textLayout = new QHBoxLayout();
        textLayout->addWidget(new QLabel("📝 Text:"));
        QLineEdit* textEdit = new QLineEdit(text);
        textEdit->setPlaceholderText("Item text or description...");
        textLayout->addWidget(textEdit, 1);
        layout->addLayout(textLayout);

        // Image input
        auto imageLayout = new QHBoxLayout();
        imageLayout->addWidget(new QLabel("🖼️ Image:"));
        QLineEdit* imageEdit = new QLineEdit(image);
        imageEdit->setPlaceholderText("Image path (optional)");

        QPushButton* browseButton = new QPushButton("Browse 📁");
        connect(browseButton, &QPushButton::clicked, [this, imageEdit](){
            browseStimulusImage(imageEdit);
        });

        imageLayout->addWidget(imageEdit, 1);
        imageLayout->addWidget(browseButton);
        layout->addLayout(imageLayout);

        // Category assignment
        auto categoryLayout = new QHBoxLayout();
        categoryLayout->addWidget(new QLabel("📂 Category:"));
        QComboBox* categoryCombo = new QComboBox();
        categoryCombo->addItems(categoryList);

        // Set current category from answer
        QString answerKey = image.isEmpty() ? text : image.split("/").last();
        if (answer.contains(answerKey)) {
            categoryCombo->setCurrentText(answer[answerKey].toString());
        }

        categoryLayout->addWidget(categoryCombo);
        categoryLayout->addStretch();
        layout->addLayout(categoryLayout);

        m_stimuliLayout->addWidget(row);
        m_stimuliWidgets.append(row);
    }
}

void CategorizationEditor::addCategory() 
{
    QJsonArray categories = m_currentQuestion["categories"].toArray();
    categories.append("New Category");
    m_currentQuestion["categories"] = categories;
    refreshCategoriesUI();
    refreshStimuliUI(); // Refresh stimuli to update dropdowns
}

void CategorizationEditor::addStimulus() 
{
    QJsonArray stimuli = m_currentQuestion["stimuli"].toArray();

    QJsonObject newStimulus;
    newStimulus["text"] = "New Item";
    newStimulus["image"] = QJsonValue::Null;
    stimuli.append(newStimulus);

    m_currentQuestion["stimuli"] = stimuli;
    refreshStimuliUI();
}

void CategorizationEditor::clearCategories()
{
    for (QWidget* widget : m_categoryWidgets) {
        m_categoriesLayout->removeWidget(widget);
        widget->deleteLater();
    }
    m_categoryWidgets.clear();
}

void CategorizationEditor::clearStimuli()
{
    for (QWidget* widget : m_stimuliWidgets) {
        m_stimuliLayout->removeWidget(widget);
        widget->deleteLater();
    }
    m_stimuliWidgets.clear();
}

void CategorizationEditor::browseStimulusImage(QLineEdit* imageEdit)
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "💖 Select Cute Image File 💖", "",
        "Image Files (*.png *.jpg *.jpeg *.gif *.bmp);;All Files (*)");

    if (!fileName.isEmpty()) {
        imageEdit->setText(fileName);
    }
}

void CategorizationEditor::browseMedia()
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
