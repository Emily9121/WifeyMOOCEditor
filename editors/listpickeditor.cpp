// =======================================================================
// editors/listpickeditor.cpp
// =======================================================================
#include "listpickeditor.h"
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

ListPickEditor::ListPickEditor(QWidget* parent) : BaseQuestionEditor(parent) {
    auto mainLayout = new QVBoxLayout(this); setLayout(mainLayout);
    auto questionGroup = new QGroupBox("📝 Question Text 📝");
    auto questionLayout = new QVBoxLayout(questionGroup);
    m_questionTextEdit = new QTextEdit(); m_questionTextEdit->setFixedHeight(80);
    questionLayout->addWidget(m_questionTextEdit); mainLayout->addWidget(questionGroup);
    m_optionsGroup = new QGroupBox("📋 Answer Options (Pick many!) 📋");
    m_optionsLayout = new QVBoxLayout(m_optionsGroup); mainLayout->addWidget(m_optionsGroup);
    auto buttonLayout = new QHBoxLayout();
    auto addOptionButton = new QPushButton("➕ Add Option");
    auto saveButton = new QPushButton("💾 Save Question");
    buttonLayout->addWidget(addOptionButton); buttonLayout->addWidget(saveButton);
    mainLayout->addLayout(buttonLayout); mainLayout->addStretch();
    connect(addOptionButton, &QPushButton::clicked, this, &ListPickEditor::addOption);
    connect(saveButton, &QPushButton::clicked, [this](){ getSavedQuestion(); QMessageBox::information(this, "Success! �", "Question saved beautifully!"); });
}
void ListPickEditor::loadQuestion(const QJsonObject& question) { m_currentQuestion = question; m_questionTextEdit->setText(m_currentQuestion["question"].toString()); refreshOptionsUI(); }
QJsonObject ListPickEditor::getSavedQuestion() {
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    QJsonArray optionsArray; QJsonArray answerArray;
    for (size_t i = 0; i < m_optionEdits.size(); ++i) {
        optionsArray.append(m_optionEdits[i]->text());
        if (m_checkBoxes[i]->isChecked()) { answerArray.append(static_cast<int>(i)); }
    }
    m_currentQuestion["options"] = optionsArray; m_currentQuestion["answer"] = answerArray;
    return m_currentQuestion;
}
void ListPickEditor::refreshOptionsUI() {
    qDeleteAll(m_optionWidgets); m_optionWidgets.clear(); m_optionEdits.clear(); m_checkBoxes.clear();
    QJsonArray options = m_currentQuestion["options"].toArray(); QJsonArray answerArray = m_currentQuestion["answer"].toArray();
    QList<int> answers; for(const auto& val : answerArray) { answers.append(val.toInt()); }
    for (int i = 0; i < options.size(); ++i) {
        QString optionText = options[i].toString();
        auto optionWidget = new QWidget(); auto layout = new QHBoxLayout(optionWidget);
        auto checkBox = new QCheckBox(); if (answers.contains(i)) checkBox->setChecked(true);
        auto lineEdit = new QLineEdit(optionText); auto deleteButton = new QPushButton("🗑️");
        layout->addWidget(checkBox); layout->addWidget(new QLabel(QString("Option %1:").arg(i+1)));
        layout->addWidget(lineEdit); layout->addWidget(deleteButton);
        m_optionsLayout->addWidget(optionWidget); m_optionWidgets.push_back(optionWidget);
        m_optionEdits.push_back(lineEdit); m_checkBoxes.push_back(checkBox);
        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray currentOptions = m_currentQuestion["options"].toArray(); if(currentOptions.size() > 1) {
                currentOptions.removeAt(i); m_currentQuestion["options"] = currentOptions;
                m_currentQuestion["answer"] = QJsonArray{}; refreshOptionsUI(); }
        });
    }
}
void ListPickEditor::addOption() {
    QJsonArray options = m_currentQuestion["options"].toArray(); options.append("New Cute Option");
    m_currentQuestion["options"] = options; refreshOptionsUI();
}
