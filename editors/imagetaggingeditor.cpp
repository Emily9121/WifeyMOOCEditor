/*
* File: imagetaggingeditor.cpp
* Author: Emily
*
* Description:
* The COMPLETE implementation for our image tagging editor!
* Students can tag coordinates on images with full alternatives support! 🏷️💖
*/

#include "imagetaggingeditor.h"
#include "../helpers.h" 
#include <QFileDialog>
#include <QMessageBox>

ImageTaggingEditor::ImageTaggingEditor(QWidget *parent) : BaseQuestionEditor(parent) 
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    // Question text section
    auto questionGroup = new QGroupBox("❓ Question Prompt ❓");
    auto questionLayout = new QVBoxLayout(questionGroup);

    m_questionTextEdit = new QTextEdit();
    m_questionTextEdit->setPlaceholderText("Tag the cute image! 💖");
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
    // 💖 I've added a media section for extra media, as you suggested! 💖
    auto extraMediaGroup = new QGroupBox("🎬 Extra Media (Optional) 🎬");
    auto extraMediaLayout = new QVBoxLayout(extraMediaGroup);

    auto extraMediaRowLayout = new QHBoxLayout();
    m_mediaTypeCombo = new QComboBox();
    m_mediaTypeCombo->addItems({"None", "Video", "Audio", "Image"});

    m_mediaEdit = new QLineEdit();
    m_mediaEdit->setPlaceholderText("Select media file path...");

    auto browseMediaBtn = new QPushButton("Browse 📁");
    connect(browseMediaBtn, &QPushButton::clicked, this, &ImageTaggingEditor::browseMedia);

    extraMediaRowLayout->addWidget(new QLabel("Type:"));
    extraMediaRowLayout->addWidget(m_mediaTypeCombo);
    extraMediaRowLayout->addWidget(new QLabel("File:"));
    extraMediaRowLayout->addWidget(m_mediaEdit, 1);
    extraMediaRowLayout->addWidget(browseMediaBtn);

    extraMediaLayout->addLayout(extraMediaRowLayout);
    mainLayout->addWidget(extraMediaGroup);

    // Main image configuration
    auto mainImageGroup = new QGroupBox("🖼️ Main Image Configuration 🖼️");
    auto mainImageLayout = new QVBoxLayout(mainImageGroup);

    // Image path
    auto imageRow = new QHBoxLayout();
    imageRow->addWidget(new QLabel("Main Image:"));
    m_mainImageEdit = new QLineEdit();
    m_mainImageEdit->setPlaceholderText("Select main image (like: images/body.jpg)");
    auto browseMainBtn = new QPushButton("Browse 📁");
    connect(browseMainBtn, &QPushButton::clicked, this, &ImageTaggingEditor::browseMainImage);

    imageRow->addWidget(m_mainImageEdit, 1);
    imageRow->addWidget(browseMainBtn);
    mainImageLayout->addLayout(imageRow);

    // Button label
    auto buttonRow = new QHBoxLayout();
    buttonRow->addWidget(new QLabel("Button Label:"));
    m_buttonLabelEdit = new QLineEdit();
    m_buttonLabelEdit->setPlaceholderText("Alternative View");
    buttonRow->addWidget(m_buttonLabelEdit, 1);
    mainImageLayout->addLayout(buttonRow);

    mainLayout->addWidget(mainImageGroup);

    // Main tags configuration
    auto mainTagsGroup = new QGroupBox("🏷️ Main Image Tags 🏷️");
    auto mainTagsGroupLayout = new QVBoxLayout(mainTagsGroup);

    auto tagsLabel = new QLabel("💡 Configure draggable tags and their coordinates for the main image!");
    tagsLabel->setStyleSheet("font-style: italic; color: #8B008B;");
    mainTagsGroupLayout->addWidget(tagsLabel);

    QScrollArea *mainTagsScrollArea = new QScrollArea();
    mainTagsScrollArea->setWidgetResizable(true);
    QWidget *mainTagsScrollWidget = new QWidget();
    m_mainTagsLayout = new QVBoxLayout(mainTagsScrollWidget);
    mainTagsScrollArea->setWidget(mainTagsScrollWidget);

    mainTagsGroupLayout->addWidget(mainTagsScrollArea);

    auto addTagButton = new QPushButton("Add Tag 🏷️");
    connect(addTagButton, &QPushButton::clicked, this, &ImageTaggingEditor::addTag);
    mainTagsGroupLayout->addWidget(addTagButton);
    
    // 💖 We're giving this section a stretch factor of 1 so it takes up all the space! 💖
    mainLayout->addWidget(mainTagsGroup, 1);
    
    // Alternatives section
    auto alternativesGroup = new QGroupBox("🌈 Alternative Images (Optional) 🌈");
    auto alternativesGroupLayout = new QVBoxLayout(alternativesGroup);
    
    auto altLabel = new QLabel("💡 Add alternative images with their own tag coordinates! Super advanced! ✨");
    altLabel->setWordWrap(true);
    altLabel->setStyleSheet("font-style: italic; color: #8B008B;");
    alternativesGroupLayout->addWidget(altLabel);
    
    QScrollArea *alternativesScrollArea = new QScrollArea();
    alternativesScrollArea->setWidgetResizable(true);
    alternativesScrollArea->setMinimumHeight(200);
    QWidget *alternativesScrollWidget = new QWidget();
    m_alternativesLayout = new QVBoxLayout(alternativesScrollWidget);
    alternativesScrollArea->setWidget(alternativesScrollWidget);

    alternativesGroupLayout->addWidget(alternativesScrollArea);

    auto addAlternativeButton = new QPushButton("Add Alternative 🌈");
    connect(addAlternativeButton, &QPushButton::clicked, this, &ImageTaggingEditor::addAlternative);
    alternativesGroupLayout->addWidget(addAlternativeButton);

    mainLayout->addWidget(alternativesGroup, 1);

    // Initialize with defaults
    m_currentQuestion["type"] = "image_tagging";
    m_currentQuestion["question"] = "";
    m_currentQuestion["media"] = QJsonObject{{"image", "images/body.jpg"}};
    // 💖 I've added a new key for optional media so we don't overwrite the main image! 💖
    m_currentQuestion["optional_media"] = QJsonValue::Null;
    m_currentQuestion["button_label"] = "Alternative View";
    m_currentQuestion["tags"] = QJsonArray{
        QJsonObject{{"label", "Tag 1"}, {"id", "tag1"}},
        QJsonObject{{"label", "Tag 2"}, {"id", "tag2"}}
    };
    m_currentQuestion["answer"] = QJsonObject{
        {"tag1", QJsonArray{100, 150}},
        {"tag2", QJsonArray{200, 250}}
    };
    m_currentQuestion["alternatives"] = QJsonArray();

    refreshUI();
}

void ImageTaggingEditor::loadJson(const QJsonObject& question) 
{
    m_currentQuestion = question;
    // ✨ Load the hint text! uwu ✨
    m_hintTextEdit->setText(question["hint"].toString());
    refreshUI();
}

QJsonObject ImageTaggingEditor::getJson() 
{
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    m_currentQuestion["type"] = "image_tagging";

        // ✨ Save the hint text! ✨
    QString hintText = m_hintTextEdit->toPlainText().trimmed();
    if (!hintText.isEmpty()) {
        m_currentQuestion["hint"] = hintText;
    } else {
        m_currentQuestion.remove("hint");
    }

    // Save main image info
    QString mainImagePath = m_mainImageEdit->text().trimmed();
    if (!mainImagePath.isEmpty()) {
        QJsonObject media;
        media["image"] = mainImagePath;
        m_currentQuestion["media"] = media;
    }

    m_currentQuestion["button_label"] = m_buttonLabelEdit->text().trimmed();

    // Save main tags and coordinates
    QJsonArray tagsArray;
    QJsonObject answerObject;

    for (QWidget* widget : m_mainTagWidgets) {
        QLineEdit* idEdit = widget->findChildren<QLineEdit*>()[0];
        QLineEdit* labelEdit = widget->findChildren<QLineEdit*>()[1];
        QLineEdit* xEdit = widget->findChildren<QLineEdit*>()[2];
        QLineEdit* yEdit = widget->findChildren<QLineEdit*>()[3];

        if (idEdit && labelEdit && xEdit && yEdit) {
            QString id = idEdit->text().trimmed();
            QString label = labelEdit->text().trimmed();
            int x = xEdit->text().toInt();
            int y = yEdit->text().toInt();

            QJsonObject tagObj;
            tagObj["id"] = id;
            tagObj["label"] = label;
            tagsArray.append(tagObj);

            answerObject[id] = QJsonArray{x, y};
        }
    }

    m_currentQuestion["tags"] = tagsArray;
    m_currentQuestion["answer"] = answerObject;

    // Save alternatives
    QJsonArray alternativesArray;
    for (const AlternativeWidgets& alt : m_alternativeWidgets) {
        QJsonObject altObj;

        QString altImagePath = alt.imageEdit->text().trimmed();
        QString altButtonLabel = alt.buttonLabelEdit->text().trimmed();

        if (!altImagePath.isEmpty()) {
            QJsonObject altMedia;
            altMedia["image"] = altImagePath;
            altObj["media"] = altMedia;
        }

        altObj["button_label"] = altButtonLabel;

        // Save alternative tags (same as main tags)
        altObj["tags"] = tagsArray; // Same tags structure

        // Save alternative coordinates
        QJsonObject altAnswer;
        for (QWidget* widget : alt.tagWidgets) {
            if (widget->findChildren<QLineEdit*>().size() >= 4) {
                QLineEdit* idEdit = widget->findChildren<QLineEdit*>()[0];
                QLineEdit* xEdit = widget->findChildren<QLineEdit*>()[2];
                QLineEdit* yEdit = widget->findChildren<QLineEdit*>()[3];

                QString id = idEdit->text().trimmed();
                int x = xEdit->text().toInt();
                int y = yEdit->text().toInt();

                altAnswer[id] = QJsonArray{x, y};
            }
        }
        altObj["answer"] = altAnswer;

        alternativesArray.append(altObj);
    }

    m_currentQuestion["alternatives"] = alternativesArray;

    // Handle optional media
    QString mediaType = m_mediaTypeCombo->currentText();
    QString mediaPath = m_mediaEdit->text().trimmed();

    if (mediaType == "None" || mediaPath.isEmpty()) {
        m_currentQuestion["optional_media"] = QJsonValue::Null;
    } else {
        QJsonObject media;
        media[mediaType.toLower()] = mediaPath;
        m_currentQuestion["optional_media"] = media;
    }

    return m_currentQuestion;
}

void ImageTaggingEditor::refreshUI() 
{
    // Load question text
    m_questionTextEdit->setText(m_currentQuestion["question"].toString());

    // Load main image info
    QJsonObject media = m_currentQuestion["media"].toObject();
    m_mainImageEdit->setText(media["image"].toString());
    m_buttonLabelEdit->setText(m_currentQuestion["button_label"].toString());
    
    // Load optional media
    QJsonValue optionalMediaValue = m_currentQuestion["optional_media"];
    if (optionalMediaValue.isNull()) {
        m_mediaTypeCombo->setCurrentText("None");
        m_mediaEdit->clear();
    } else {
        QJsonObject media = optionalMediaValue.toObject();
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

    refreshMainTagsUI();
    refreshAlternativesUI();
}

void ImageTaggingEditor::refreshMainTagsUI() 
{
    clearMainTags();

    QJsonArray tags = m_currentQuestion["tags"].toArray();
    QJsonObject answer = m_currentQuestion["answer"].toObject();

    for (const QJsonValue& tagValue : tags) {
        QJsonObject tag = tagValue.toObject();
        QString id = tag["id"].toString();
        QString label = tag["label"].toString();

        QJsonArray coords = answer[id].toArray();
        int x = coords.size() > 0 ? coords[0].toInt() : 0;
        int y = coords.size() > 1 ? coords[1].toInt() : 0;

        QWidget* row = new QWidget();
        auto layout = new QHBoxLayout(row);

        // Tag ID
        layout->addWidget(new QLabel("ID:"));
        QLineEdit* idEdit = new QLineEdit(id);
        idEdit->setMaximumWidth(80);
        layout->addWidget(idEdit);

        // Tag Label
        layout->addWidget(new QLabel("Label:"));
        QLineEdit* labelEdit = new QLineEdit(label);
        labelEdit->setMinimumWidth(120);
        layout->addWidget(labelEdit);

        // Coordinates
        layout->addWidget(new QLabel("X:"));
        QLineEdit* xEdit = new QLineEdit(QString::number(x));
        xEdit->setMaximumWidth(60);
        layout->addWidget(xEdit);

        layout->addWidget(new QLabel("Y:"));
        QLineEdit* yEdit = new QLineEdit(QString::number(y));
        yEdit->setMaximumWidth(60);
        layout->addWidget(yEdit);

        // Delete button
        QPushButton* deleteBtn = new QPushButton("🗑️");
        connect(deleteBtn, &QPushButton::clicked, [this, row](){
            if (m_mainTagWidgets.size() > 1) {
                m_mainTagsLayout->removeWidget(row);
                m_mainTagWidgets.removeOne(row);
                row->deleteLater();
            }
        });
        layout->addWidget(deleteBtn);

        m_mainTagsLayout->addWidget(row);
        m_mainTagWidgets.append(row);
    }
}

void ImageTaggingEditor::refreshAlternativesUI() 
{
    clearAlternatives();

    QJsonArray alternatives = m_currentQuestion["alternatives"].toArray();

    for (int altIndex = 0; altIndex < alternatives.size(); ++altIndex) {
        QJsonObject alt = alternatives[altIndex].toObject();

        // Create alternative container
        QWidget* altContainer = new QWidget();
        altContainer->setStyleSheet("QWidget { border: 2px solid #FF69B4; border-radius: 10px; margin: 5px; padding: 10px; }");
        auto altLayout = new QVBoxLayout(altContainer);

        // Alternative header
        auto headerLayout = new QHBoxLayout();
        auto altLabel = new QLabel(QString("🌈 Alternative %1 🌈").arg(altIndex + 1));
        altLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #8B008B;");

        QPushButton* deleteAltBtn = new QPushButton("Delete Alternative 🗑️");
        connect(deleteAltBtn, &QPushButton::clicked, [this, altIndex](){
            // Remove from JSON and refresh
            QJsonArray current = m_currentQuestion["alternatives"].toArray();
            current.removeAt(altIndex);
            m_currentQuestion["alternatives"] = current;
            refreshAlternativesUI();
        });

        headerLayout->addWidget(altLabel);
        headerLayout->addStretch();
        headerLayout->addWidget(deleteAltBtn);
        altLayout->addLayout(headerLayout);

        // Alternative image and button label
        auto altConfigLayout = new QHBoxLayout();

        altConfigLayout->addWidget(new QLabel("Image:"));
        QLineEdit* altImageEdit = new QLineEdit();
        QJsonObject altMedia = alt["media"].toObject();
        altImageEdit->setText(altMedia["image"].toString());
        altImageEdit->setPlaceholderText("Alternative image path...");

        QPushButton* browseAltBtn = new QPushButton("Browse 📁");
        connect(browseAltBtn, &QPushButton::clicked, [this, altImageEdit](){
            browseAlternativeImage(altImageEdit);
        });

        altConfigLayout->addWidget(altImageEdit, 1);
        altConfigLayout->addWidget(browseAltBtn);

        altConfigLayout->addWidget(new QLabel("Button:"));
        QLineEdit* altButtonEdit = new QLineEdit();
        altButtonEdit->setText(alt["button_label"].toString());
        altButtonEdit->setPlaceholderText("Button label...");
        altButtonEdit->setMaximumWidth(150);
        altConfigLayout->addWidget(altButtonEdit);

        altLayout->addLayout(altConfigLayout);

        // Alternative coordinates section
        auto coordsLabel = new QLabel("🎯 Tag Coordinates for this Alternative:");
        coordsLabel->setStyleSheet("font-weight: bold; margin-top: 10px;");
        altLayout->addWidget(coordsLabel);

        QWidget* coordsWidget = new QWidget();
        QVBoxLayout* coordsLayout = new QVBoxLayout(coordsWidget);

        // Create coordinate inputs for each main tag
        QJsonObject altAnswer = alt["answer"].toObject();
        QJsonArray mainTags = m_currentQuestion["tags"].toArray();
        QList<QWidget*> altTagWidgets;

        for (const QJsonValue& tagValue : mainTags) {
            QJsonObject tag = tagValue.toObject();
            QString id = tag["id"].toString();
            QString label = tag["label"].toString();

            QJsonArray coords = altAnswer[id].toArray();
            int x = coords.size() > 0 ? coords[0].toInt() : 0;
            int y = coords.size() > 1 ? coords[1].toInt() : 0;

            QWidget* coordRow = new QWidget();
            auto coordRowLayout = new QHBoxLayout(coordRow);

            coordRowLayout->addWidget(new QLabel(QString("%1 (%2):").arg(label).arg(id)));

            QLineEdit* hiddenIdEdit = new QLineEdit(id);
            hiddenIdEdit->setVisible(false);
            QLineEdit* hiddenLabelEdit = new QLineEdit(label);
            hiddenLabelEdit->setVisible(false);

            coordRowLayout->addWidget(new QLabel("X:"));
            QLineEdit* xEdit = new QLineEdit(QString::number(x));
            xEdit->setMaximumWidth(60);
            coordRowLayout->addWidget(xEdit);

            coordRowLayout->addWidget(new QLabel("Y:"));
            QLineEdit* yEdit = new QLineEdit(QString::number(y));
            yEdit->setMaximumWidth(60);
            coordRowLayout->addWidget(yEdit);

            coordRowLayout->addWidget(hiddenIdEdit);
            coordRowLayout->addWidget(hiddenLabelEdit);
            coordRowLayout->addStretch();

            coordsLayout->addWidget(coordRow);
            altTagWidgets.append(coordRow);
        }

        altLayout->addWidget(coordsWidget);

        // Store alternative widgets
        AlternativeWidgets altWidgets;
        altWidgets.container = altContainer;
        altWidgets.imageEdit = altImageEdit;
        altWidgets.buttonLabelEdit = altButtonEdit;
        altWidgets.tagsLayout = coordsLayout;
        altWidgets.tagWidgets = altTagWidgets;

        m_alternativeWidgets.append(altWidgets);
        m_alternativesLayout->addWidget(altContainer);
    }
}

void ImageTaggingEditor::addTag() 
{
    QJsonArray tags = m_currentQuestion["tags"].toArray();
    QString newId = QString("tag%1").arg(tags.size() + 1);

    QJsonObject newTag;
    newTag["id"] = newId;
    newTag["label"] = "New Tag";
    tags.append(newTag);

    m_currentQuestion["tags"] = tags;

    // Add default coordinates
    QJsonObject answer = m_currentQuestion["answer"].toObject();
    answer[newId] = QJsonArray{100, 100};
    m_currentQuestion["answer"] = answer;

    refreshUI(); // Refresh everything to update alternatives too
}

void ImageTaggingEditor::addAlternative() 
{
    QJsonArray alternatives = m_currentQuestion["alternatives"].toArray();

    QJsonObject newAlt;
    newAlt["media"] = QJsonObject{{"image", ""}};
    newAlt["button_label"] = "New Alternative";
    newAlt["tags"] = m_currentQuestion["tags"]; // Same tags as main

    // Initialize with default coordinates for all tags
    QJsonObject altAnswer;
    QJsonArray tags = m_currentQuestion["tags"].toArray();
    for (const QJsonValue& tagValue : tags) {
        QString id = tagValue.toObject()["id"].toString();
        altAnswer[id] = QJsonArray{100, 100};
    }
    newAlt["answer"] = altAnswer;

    alternatives.append(newAlt);
    m_currentQuestion["alternatives"] = alternatives;

    refreshAlternativesUI();
}

void ImageTaggingEditor::clearMainTags()
{
    for (QWidget* widget : m_mainTagWidgets) {
        m_mainTagsLayout->removeWidget(widget);
        widget->deleteLater();
    }
    m_mainTagWidgets.clear();
}

void ImageTaggingEditor::clearAlternatives()
{
    for (const AlternativeWidgets& alt : m_alternativeWidgets) {
        m_alternativesLayout->removeWidget(alt.container);
        alt.container->deleteLater();
    }
    m_alternativeWidgets.clear();
}

void ImageTaggingEditor::browseMainImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "💖 Select Main Image File 💖", "",
        "Image Files (*.png *.jpg *.jpeg *.gif *.bmp);;All Files (*)");

    if (!fileName.isEmpty()) {
        m_mainImageEdit->setText(fileName);
    }
}

void ImageTaggingEditor::browseAlternativeImage(QLineEdit* imageEdit)
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "💖 Select Alternative Image File 💖", "",
        "Image Files (*.png *.jpg *.jpeg *.gif *.bmp);;All Files (*)");

    if (!fileName.isEmpty()) {
        imageEdit->setText(fileName);
    }
}

void ImageTaggingEditor::browseMedia()
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
