// =======================================================================
// editors/multiquestionseditor.cpp
// =======================================================================
#include "multiquestionseditor.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QTextEdit>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QLabel>

MultiQuestionsEditor::MultiQuestionsEditor(QWidget* parent) : BaseQuestionEditor(parent) {
    auto mainLayout = new QVBoxLayout(this); setLayout(mainLayout);
    auto infoLabel = new QLabel("🎭 For this complex type, you can edit the raw JSON of the sub-questions directly, babe! 🎭");
    infoLabel->setWordWrap(true); mainLayout->addWidget(infoLabel);
    m_jsonEditor = new QTextEdit(); mainLayout->addWidget(m_jsonEditor);
    auto saveButton = new QPushButton("💾 Save Question"); mainLayout->addWidget(saveButton);
    connect(saveButton, &QPushButton::clicked, [this](){ getJson(); QMessageBox::information(this, "Success! 💖", "Question saved beautifully!"); });
}
void MultiQuestionsEditor::loadJson(const QJsonObject& question) {
    m_currentQuestion = question;
    QJsonDocument doc(question["questions"].toArray());
    m_jsonEditor->setText(doc.toJson(QJsonDocument::Indented));
}
QJsonObject MultiQuestionsEditor::getJson() {
    QJsonDocument doc = QJsonDocument::fromJson(m_jsonEditor->toPlainText().toUtf8());
    m_currentQuestion["questions"] = doc.array();
    return m_currentQuestion;
}
