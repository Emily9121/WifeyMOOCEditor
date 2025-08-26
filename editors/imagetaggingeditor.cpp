// =======================================================================
// editors/imagetaggingeditor.cpp
// =======================================================================
#include "imagetaggingeditor.h"
#include "../helpers.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QJsonArray>
#include <QMessageBox>

ImageTaggingEditor::ImageTaggingEditor(QWidget* parent) : BaseQuestionEditor(parent) {
    auto mainLayout = new QVBoxLayout(this); setLayout(mainLayout);
    auto questionGroup = new QGroupBox("📝 Question Text 📝"); auto qLayout = new QVBoxLayout(questionGroup);
    m_questionTextEdit = new QTextEdit(); m_questionTextEdit->setFixedHeight(80); qLayout->addWidget(m_questionTextEdit); mainLayout->addWidget(questionGroup);
    auto mainGroup = new QGroupBox("🖼️ Main Image Config 🖼️"); auto mainLayoutGroup = new QVBoxLayout(mainGroup);
    auto imageLayout = new QHBoxLayout(); imageLayout->addWidget(new QLabel("Image Path:")); m_mainImageEdit = new QLineEdit(); imageLayout->addWidget(m_mainImageEdit);
    auto browseBtn = new QPushButton("Browse..."); imageLayout->addWidget(browseBtn); mainLayoutGroup->addLayout(imageLayout);
    auto btnLayout = new QHBoxLayout(); btnLayout->addWidget(new QLabel("Button Label:")); m_mainButtonLabelEdit = new QLineEdit(); btnLayout->addWidget(m_mainButtonLabelEdit);
    mainLayoutGroup->addLayout(btnLayout); mainLayout->addWidget(mainGroup);
    m_tagsGroup = new QGroupBox("🏷️ Draggable Tags 🏷️"); m_tagsLayout = new QVBoxLayout(m_tagsGroup); mainLayout->addWidget(m_tagsGroup);
    m_altsGroup = new QGroupBox("🔄 Alternatives 🔄"); m_altsLayout = new QVBoxLayout(m_altsGroup); mainLayout->addWidget(m_altsGroup);
    auto buttonLayout = new QHBoxLayout(); auto addTagBtn = new QPushButton("➕ Add Tag"); auto addAltBtn = new QPushButton("➕ Add Alternative"); auto saveBtn = new QPushButton("💾 Save Question");
    buttonLayout->addWidget(addTagBtn); buttonLayout->addWidget(addAltBtn); buttonLayout->addWidget(saveBtn); mainLayout->addLayout(buttonLayout); mainLayout->addStretch();
    connect(browseBtn, &QPushButton::clicked, [this](){ browseForFile(this, m_mainImageEdit); });
    connect(addTagBtn, &QPushButton::clicked, this, &ImageTaggingEditor::addTag);
    connect(addAltBtn, &QPushButton::clicked, this, &ImageTaggingEditor::addAlternative);
    connect(saveBtn, &QPushButton::clicked, [this](){ getJson(); QMessageBox::information(this, "Success! 💖", "Question saved beautifully!"); });
}
void ImageTaggingEditor::loadJson(const QJsonObject& question) { m_currentQuestion = question; refreshUI(); }
void ImageTaggingEditor::refreshUI() {
    m_questionTextEdit->setText(m_currentQuestion["question"].toString());
    m_mainImageEdit->setText(m_currentQuestion["media"].toObject()["image"].toString());
    m_mainButtonLabelEdit->setText(m_currentQuestion["button_label"].toString());
    refreshMainTagsUI();
    refreshAlternativesUI();
}
void ImageTaggingEditor::refreshMainTagsUI() {
    for(auto& widgets : m_tagWidgets) { delete widgets.container; } m_tagWidgets.clear();
    QJsonArray tags = m_currentQuestion["tags"].toArray(); QJsonObject answer = m_currentQuestion["answer"].toObject();
    for(int i = 0; i < tags.size(); ++i) {
        QJsonObject tag = tags[i].toObject(); QString id = tag["id"].toString();
        auto container = new QWidget(); auto layout = new QHBoxLayout(container);
        layout->addWidget(new QLabel(QString("Tag %1:").arg(i+1)));
        layout->addWidget(new QLabel("ID:")); auto idEdit = new QLineEdit(id); layout->addWidget(idEdit);
        layout->addWidget(new QLabel("Label:")); auto labelEdit = new QLineEdit(tag["label"].toString()); layout->addWidget(labelEdit);
        QJsonArray coords = answer[id].toArray();
        layout->addWidget(new QLabel("X:")); auto xEdit = new QLineEdit(QString::number(coords[0].toDouble())); layout->addWidget(xEdit);
        layout->addWidget(new QLabel("Y:")); auto yEdit = new QLineEdit(QString::number(coords[1].toDouble())); layout->addWidget(yEdit);
        auto deleteBtn = new QPushButton("🗑️"); layout->addWidget(deleteButton);
        m_tagsLayout->addWidget(container); m_tagWidgets.push_back({container, idEdit, labelEdit, xEdit, yEdit});
        connect(deleteBtn, &QPushButton::clicked, [this, i](){
            QJsonArray currentTags = m_currentQuestion["tags"].toArray(); currentTags.removeAt(i); m_currentQuestion["tags"] = currentTags; refreshUI();
        });
    }
}
void ImageTaggingEditor::refreshAlternativesUI() {
    for(auto& widgets : m_altWidgets) { delete widgets.container; } m_altWidgets.clear();
    QJsonArray alternatives = m_currentQuestion["alternatives"].toArray();
    for(int i = 0; i < alternatives.size(); ++i) {
        QJsonObject alt = alternatives[i].toObject();
        auto container = new QGroupBox(QString("💖 Alternative %1 💖").arg(i+1)); auto layout = new QVBoxLayout(container);
        auto imageLayout = new QHBoxLayout(); imageLayout->addWidget(new QLabel("Image:"));
        auto imageEdit = new QLineEdit(alt["media"].toObject()["image"].toString()); imageLayout->addWidget(imageEdit);
        auto browseBtn = new QPushButton("Browse..."); imageLayout->addWidget(browseBtn); layout->addLayout(imageLayout);
        auto btnLayout = new QHBoxLayout(); btnLayout->addWidget(new QLabel("Button Label:"));
        auto buttonLabelEdit = new QLineEdit(alt["button_label"].toString()); btnLayout->addWidget(buttonLabelEdit); layout->addLayout(btnLayout);
        AltWidgets altWidgetSet{container, imageEdit, buttonLabelEdit};
        QJsonObject altAnswer = alt["answer"].toObject();
        for(const auto& tagVal : m_currentQuestion["tags"].toArray()) {
            QJsonObject tagObj = tagVal.toObject(); QString id = tagObj["id"].toString();
            auto tagCoordWidget = new QWidget(); auto tagLayout = new QHBoxLayout(tagCoordWidget);
            tagLayout->addWidget(new QLabel(QString("Coords for '%1':").arg(tagObj["label"].toString())));
            QJsonArray coords = altAnswer[id].toArray();
            tagLayout->addWidget(new QLabel("X:")); auto xEdit = new QLineEdit(QString::number(coords[0].toDouble())); tagLayout->addWidget(xEdit);
            tagLayout->addWidget(new QLabel("Y:")); auto yEdit = new QLineEdit(QString::number(coords[1].toDouble())); tagLayout->addWidget(yEdit);
            layout->addWidget(tagCoordWidget);
            altWidgetSet.tagCoordWidgets.push_back(tagCoordWidget); altWidgetSet.xEdits.push_back(xEdit); altWidgetSet.yEdits.push_back(yEdit);
        }
        auto deleteBtn = new QPushButton("🗑️ Delete Alternative"); layout->addWidget(deleteBtn, 0, Qt::AlignRight);
        m_altsLayout->addWidget(container); m_altWidgets.push_back(altWidgetSet);
        connect(browseBtn, &QPushButton::clicked, [this, imageEdit](){ browseForFile(this, imageEdit); });
        connect(deleteBtn, &QPushButton::clicked, [this, i](){
            QJsonArray currentAlts = m_currentQuestion["alternatives"].toArray(); currentAlts.removeAt(i); m_currentQuestion["alternatives"] = currentAlts; refreshUI();
        });
    }
}
void ImageTaggingEditor::addTag() {
    QJsonArray tags = m_currentQuestion["tags"].toArray();
    tags.append(QJsonObject{{"id", QString("tag%1").arg(tags.size()+1)}, {"label", "New Tag"}});
    m_currentQuestion["tags"] = tags; refreshUI();
}
void ImageTaggingEditor::addAlternative() {
    QJsonArray alts = m_currentQuestion["alternatives"].toArray();
    alts.append(QJsonObject{{"media", QJsonObject{{"image", ""}}}, {"button_label", "New Alt"}, {"answer", QJsonObject()}});
    m_currentQuestion["alternatives"] = alts; refreshUI();
}
QJsonObject ImageTaggingEditor::getJson() {
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    m_currentQuestion["media"] = QJsonObject{{"image", m_mainImageEdit->text()}};
    m_currentQuestion["button_label"] = m_mainButtonLabelEdit->text();
    QJsonArray tagsArray; QJsonObject answerObject;
    for(const auto& tagWidgets : m_tagWidgets) {
        QString id = tagWidgets.idEdit->text();
        tagsArray.append(QJsonObject{{"id", id}, {"label", tagWidgets.labelEdit->text()}});
        answerObject[id] = QJsonArray{tagWidgets.xEdit->text().toDouble(), tagWidgets.yEdit->text().toDouble()};
    }
    m_currentQuestion["tags"] = tagsArray; m_currentQuestion["answer"] = answerObject;
    QJsonArray altsArray;
    for(size_t i = 0; i < m_altWidgets.size(); ++i) {
        QJsonObject altObj;
        altObj["media"] = QJsonObject{{"image", m_altWidgets[i].imageEdit->text()}};
        altObj["button_label"] = m_altWidgets[i].buttonLabelEdit->text();
        QJsonObject altAnswer;
        for(size_t j = 0; j < m_tagWidgets.size(); ++j) {
            QString id = m_tagWidgets[j].idEdit->text();
            altAnswer[id] = QJsonArray{m_altWidgets[i].xEdits[j]->text().toDouble(), m_altWidgets[i].yEdits[j]->text().toDouble()};
        }
        altObj["answer"] = altAnswer;
        altObj["tags"] = tagsArray; // Tags are the same for alternatives
        altsArray.append(altObj);
    }
    m_currentQuestion["alternatives"] = altsArray;
    return m_currentQuestion;
}
