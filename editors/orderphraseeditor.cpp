// =======================================================================
// editors/orderphraseeditor.cpp
// =======================================================================
#include "orderphraseeditor.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QTextEdit>
#include <QPushButton>
#include <QJsonArray>
#include <QStringList>
#include <QMessageBox>

OrderPhraseEditor::OrderPhraseEditor(QWidget* parent) : BaseQuestionEditor(parent) {
    auto mainLayout = new QVBoxLayout(this); setLayout(mainLayout);
    auto questionGroup = new QGroupBox("📝 Question Text 📝");
    auto questionLayout = new QVBoxLayout(questionGroup);
    m_questionTextEdit = new QTextEdit(); m_questionTextEdit->setFixedHeight(80);
    questionLayout->addWidget(m_questionTextEdit); mainLayout->addWidget(questionGroup);
    auto shuffledGroup = new QGroupBox("🔀 Shuffled Phrases (One per line) 🔀");
    auto shuffledLayout = new QVBoxLayout(shuffledGroup);
    m_shuffledTextEdit = new QTextEdit(); shuffledLayout->addWidget(m_shuffledTextEdit); mainLayout->addWidget(shuffledGroup);
    auto answerGroup = new QGroupBox("✅ Correct Order (One per line) ✅");
    auto answerLayout = new QVBoxLayout(answerGroup);
    m_answerTextEdit = new QTextEdit(); answerLayout->addWidget(m_answerTextEdit); mainLayout->addWidget(answerGroup);
    auto saveButton = new QPushButton("💾 Save Question"); mainLayout->addWidget(saveButton); mainLayout->addStretch();
    connect(saveButton, &QPushButton::clicked, [this](){ getSavedQuestion(); QMessageBox::information(this, "Success! 💖", "Question saved beautifully!"); });
}
void OrderPhraseEditor::loadQuestion(const QJsonObject& question) {
    m_currentQuestion = question; m_questionTextEdit->setText(question["question"].toString());
    QStringList shuffledList; for (const auto& val : question["phrase_shuffled"].toArray()) { shuffledList.append(val.toString()); }
    m_shuffledTextEdit->setText(shuffledList.join("\n"));
    QStringList answerList; for (const auto& val : question["answer"].toArray()) { answerList.append(val.toString()); }
    m_answerTextEdit->setText(answerList.join("\n"));
}
QJsonObject OrderPhraseEditor::getSavedQuestion() {
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    QStringList shuffledList = m_shuffledTextEdit->toPlainText().split('\n', Qt::SkipEmptyParts);
    m_currentQuestion["phrase_shuffled"] = QJsonArray::fromStringList(shuffledList);
    QStringList answerList = m_answerTextEdit->toPlainText().split('\n', Qt::SkipEmptyParts);
    m_currentQuestion["answer"] = QJsonArray::fromStringList(answerList);
    return m_currentQuestion;
}
