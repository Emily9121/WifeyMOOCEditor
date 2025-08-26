// =======================================================================
// editors/wordfilleditor.cpp
// =======================================================================
#include "wordfilleditor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QJsonArray>
#include <QMessageBox>

WordFillEditor::WordFillEditor(QWidget* parent) : BaseQuestionEditor(parent) {
    auto mainLayout = new QVBoxLayout(this); setLayout(mainLayout);
    auto questionGroup = new QGroupBox("📝 Question Text 📝");
    auto questionLayout = new QVBoxLayout(questionGroup);
    m_questionTextEdit = new QTextEdit(); m_questionTextEdit->setFixedHeight(80);
    questionLayout->addWidget(m_questionTextEdit); mainLayout->addWidget(questionGroup);
    m_partsGroup = new QGroupBox("💬 Sentence Parts 💬");
    m_partsLayout = new QVBoxLayout(m_partsGroup); mainLayout->addWidget(m_partsGroup);
    m_answersGroup = new QGroupBox("✅ Correct Answers ✅");
    m_answersLayout = new QVBoxLayout(m_answersGroup); mainLayout->addWidget(m_answersGroup);
    auto buttonLayout = new QHBoxLayout();
    auto addPartButton = new QPushButton("➕ Add Part"); auto addAnswerButton = new QPushButton("➕ Add Answer");
    auto saveButton = new QPushButton("💾 Save Question");
    buttonLayout->addWidget(addPartButton); buttonLayout->addWidget(addAnswerButton); buttonLayout->addWidget(saveButton);
    mainLayout->addLayout(buttonLayout); mainLayout->addStretch();
    connect(addPartButton, &QPushButton::clicked, this, &WordFillEditor::addPart);
    connect(addAnswerButton, &QPushButton::clicked, this, &WordFillEditor::addAnswer);
    connect(saveButton, &QPushButton::clicked, [this](){ getJson(); QMessageBox::information(this, "Success! 💖", "Question saved beautifully!"); });
}
void WordFillEditor::loadJson(const QJsonObject& question) { m_currentQuestion = question; m_questionTextEdit->setText(m_currentQuestion["question"].toString()); refreshPartsUI(); refreshAnswersUI(); }
QJsonObject WordFillEditor::getJson() {
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    QJsonArray partsArray; for (auto* partEdit : m_partEdits) { partsArray.append(partEdit->toPlainText()); }
    m_currentQuestion["sentence_parts"] = partsArray;
    QJsonArray answersArray; for (auto* answerEdit : m_answerEdits) { answersArray.append(answerEdit->text()); }
    m_currentQuestion["answers"] = answersArray;
    return m_currentQuestion;
}
void WordFillEditor::refreshPartsUI() {
    qDeleteAll(m_partWidgets); m_partWidgets.clear(); m_partEdits.clear();
    QJsonArray parts = m_currentQuestion["sentence_parts"].toArray();
    for (int i = 0; i < parts.size(); ++i) {
        auto partWidget = new QWidget(); auto layout = new QHBoxLayout(partWidget);
        auto textEdit = new QTextEdit(parts[i].toString()); textEdit->setFixedHeight(60);
        auto deleteButton = new QPushButton("🗑️");
        layout->addWidget(new QLabel(QString("Part %1:").arg(i+1))); layout->addWidget(textEdit); layout->addWidget(deleteButton);
        m_partsLayout->addWidget(partWidget); m_partWidgets.push_back(partWidget); m_partEdits.push_back(textEdit);
        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray currentParts = m_currentQuestion["sentence_parts"].toArray(); if(currentParts.size() > 1) {
                currentParts.removeAt(i); m_currentQuestion["sentence_parts"] = currentParts; refreshPartsUI(); }
        });
    }
}
void WordFillEditor::refreshAnswersUI() {
    qDeleteAll(m_answerWidgets); m_answerWidgets.clear(); m_answerEdits.clear();
    QJsonArray answers = m_currentQuestion["answers"].toArray();
    for (int i = 0; i < answers.size(); ++i) {
        auto answerWidget = new QWidget(); auto layout = new QHBoxLayout(answerWidget);
        auto lineEdit = new QLineEdit(answers[i].toString()); auto deleteButton = new QPushButton("🗑️");
        layout->addWidget(new QLabel(QString("Answer %1:").arg(i+1))); layout->addWidget(lineEdit); layout->addWidget(deleteButton);
        m_answersLayout->addWidget(answerWidget); m_answerWidgets.push_back(answerWidget); m_answerEdits.push_back(lineEdit);
        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray currentAnswers = m_currentQuestion["answers"].toArray(); currentAnswers.removeAt(i);
            m_currentQuestion["answers"] = currentAnswers; refreshAnswersUI();
        });
    }
}
void WordFillEditor::addPart() { QJsonArray parts = m_currentQuestion["sentence_parts"].toArray(); parts.append("new part..."); m_currentQuestion["sentence_parts"] = parts; refreshPartsUI(); }
void WordFillEditor::addAnswer() { QJsonArray answers = m_currentQuestion["answers"].toArray(); answers.append("new answer"); m_currentQuestion["answers"] = answers; refreshAnswersUI(); }

