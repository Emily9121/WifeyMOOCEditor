// =======================================================================
// editors/mcqmultipleeditor.cpp
// =======================================================================
#include "mcqmultipleeditor.h"
#include "../helpers.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QJsonArray>
#include <QMessageBox>

McqMultipleEditor::McqMultipleEditor(QWidget* parent) : BaseQuestionEditor(parent) {
    auto mainLayout = new QVBoxLayout(this); setLayout(mainLayout);
    auto questionGroup = new QGroupBox("📝 Question Text 📝");
    auto questionLayout = new QVBoxLayout(questionGroup);
    m_questionTextEdit = new QTextEdit(); m_questionTextEdit->setFixedHeight(80);
    questionLayout->addWidget(m_questionTextEdit); mainLayout->addWidget(questionGroup);
    m_optionsGroup = new QGroupBox("☑️ Answer Options (Pick many!) ☑️");
    m_optionsLayout = new QVBoxLayout(m_optionsGroup); mainLayout->addWidget(m_optionsGroup);
    auto buttonLayout = new QHBoxLayout();
    auto addOptionButton = new QPushButton("➕ Add Option");
    auto saveButton = new QPushButton("💾 Save Question");
    buttonLayout->addWidget(addOptionButton); buttonLayout->addWidget(saveButton);
    mainLayout->addLayout(buttonLayout); mainLayout->addStretch();
    connect(addOptionButton, &QPushButton::clicked, this, &McqMultipleEditor::addOption);
    connect(saveButton, &QPushButton::clicked, [this](){ getJson(); QMessageBox::information(this, "Success! 💖", "Question saved beautifully!"); });
}
void McqMultipleEditor::loadJson(const QJsonObject& question) { m_currentQuestion = question; m_questionTextEdit->setText(m_currentQuestion["question"].toString()); refreshOptionsUI(); }
QJsonObject McqMultipleEditor::getJson() {
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    QJsonArray optionsArray; QJsonArray answerArray;
    for (size_t i = 0; i < m_optionTextEdits.size(); ++i) {
        QJsonObject optionObj; optionObj["image"] = m_optionImageEdits[i]->text(); optionObj["text"] = m_optionTextEdits[i]->text();
        optionsArray.append(optionObj); if (m_checkBoxes[i]->isChecked()) { answerArray.append(static_cast<int>(i)); }
    }
    m_currentQuestion["options"] = optionsArray; m_currentQuestion["answer"] = answerArray;
    return m_currentQuestion;
}
void McqMultipleEditor::refreshOptionsUI() {
    qDeleteAll(m_optionWidgets); m_optionWidgets.clear(); m_optionTextEdits.clear(); m_optionImageEdits.clear(); m_checkBoxes.clear();
    QJsonArray options = m_currentQuestion["options"].toArray(); QJsonArray answerArray = m_currentQuestion["answer"].toArray();
    QList<int> answers; for(const auto& val : answerArray) { answers.append(val.toInt()); }
    for (int i = 0; i < options.size(); ++i) {
        QJsonValue optionVal = options[i]; QString optionText, imagePath;
        if (optionVal.isObject()) { optionText = optionVal.toObject()["text"].toString(); imagePath = optionVal.toObject()["image"].toString(); } else { optionText = optionVal.toString(); }
        auto optionContainer = new QGroupBox(QString("💖 Option %1 💖").arg(i+1)); auto optionLayout = new QVBoxLayout(optionContainer);
        auto topLayout = new QHBoxLayout(); auto checkBox = new QCheckBox("✅ Correct"); if (answers.contains(i)) checkBox->setChecked(true);
        auto deleteButton = new QPushButton("🗑️"); topLayout->addWidget(checkBox); topLayout->addStretch(); topLayout->addWidget(deleteButton); optionLayout->addLayout(topLayout);
        auto textLayout = new QHBoxLayout(); textLayout->addWidget(new QLabel("📝 Text:")); auto textEdit = new QLineEdit(optionText); textLayout->addWidget(textEdit); optionLayout->addLayout(textLayout);
        auto imageLayout = new QHBoxLayout(); imageLayout->addWidget(new QLabel("🖼️ Image:")); auto imageEdit = new QLineEdit(imagePath);
        auto browseButton = new QPushButton("Browse..."); imageLayout->addWidget(imageEdit); imageLayout->addWidget(browseButton); optionLayout->addLayout(imageLayout);
        m_optionsLayout->addWidget(optionContainer); m_optionWidgets.push_back(optionContainer); m_optionTextEdits.push_back(textEdit);
        m_optionImageEdits.push_back(imageEdit); m_checkBoxes.push_back(checkBox);
        connect(browseButton, &QPushButton::clicked, [this, imageEdit](){ browseForFile(this, imageEdit); });
        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray currentOptions = m_currentQuestion["options"].toArray(); if (currentOptions.size() > 1) { currentOptions.removeAt(i);
                m_currentQuestion["options"] = currentOptions; m_currentQuestion["answer"] = QJsonArray{}; refreshOptionsUI(); }
        });
    }
}
void McqMultipleEditor::addOption() {
    QJsonArray options = m_currentQuestion["options"].toArray(); QJsonObject newOption; newOption["image"] = ""; newOption["text"] = "New Fantastic Option";
    options.append(newOption); m_currentQuestion["options"] = options; refreshOptionsUI();
}
