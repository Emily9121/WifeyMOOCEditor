// =======================================================================
// editors/sequenceaudioeditor.cpp
// =======================================================================
#include "sequenceaudioeditor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QJsonArray>
#include <QMessageBox>

SequenceAudioEditor::SequenceAudioEditor(QWidget* parent) : BaseQuestionEditor(parent) {
    auto mainLayout = new QVBoxLayout(this); setLayout(mainLayout);
    auto questionGroup = new QGroupBox("📝 Question Text 📝");
    auto questionLayout = new QVBoxLayout(questionGroup);
    m_questionTextEdit = new QTextEdit(); m_questionTextEdit->setFixedHeight(80);
    questionLayout->addWidget(m_questionTextEdit); mainLayout->addWidget(questionGroup);
    m_optionsGroup = new QGroupBox("🎵 Audio Options 🎵");
    m_optionsLayout = new QVBoxLayout(m_optionsGroup); mainLayout->addWidget(m_optionsGroup);
    auto answerGroup = new QGroupBox("✅ Correct Sequence (e.g., 0,2,1) ✅");
    auto answerLayout = new QHBoxLayout(answerGroup);
    m_answerEdit = new QLineEdit(); answerLayout->addWidget(m_answerEdit);
    mainLayout->addWidget(answerGroup);
    auto buttonLayout = new QHBoxLayout();
    auto addOptionButton = new QPushButton("➕ Add Audio Option");
    auto saveButton = new QPushButton("💾 Save Question");
    buttonLayout->addWidget(addOptionButton); buttonLayout->addWidget(saveButton);
    mainLayout->addLayout(buttonLayout); mainLayout->addStretch();
    connect(addOptionButton, &QPushButton::clicked, this, &SequenceAudioEditor::addOption);
    connect(saveButton, &QPushButton::clicked, [this](){ getSavedQuestion(); QMessageBox::information(this, "Success! 💖", "Question saved beautifully!"); });
}
void SequenceAudioEditor::loadQuestion(const QJsonObject& question) { m_currentQuestion = question; m_questionTextEdit->setText(question["question"].toString()); refreshOptionsUI(); }
QJsonObject SequenceAudioEditor::getSavedQuestion() {
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    QJsonArray optionsArray; for(auto* edit : m_optionEdits) { optionsArray.append(QJsonObject{{"option", edit->text()}}); }
    m_currentQuestion["audio_options"] = optionsArray;
    QJsonArray answerArray; QStringList parts = m_answerEdit->text().split(',', Qt::SkipEmptyParts);
    for(const QString& part : parts) { answerArray.append(part.trimmed().toInt()); }
    m_currentQuestion["answer"] = answerArray;
    return m_currentQuestion;
}
void SequenceAudioEditor::refreshOptionsUI() {
    qDeleteAll(m_optionWidgets); m_optionWidgets.clear(); m_optionEdits.clear();
    QJsonArray options = m_currentQuestion["audio_options"].toArray();
    for (int i = 0; i < options.size(); ++i) {
        auto widget = new QWidget(); auto layout = new QHBoxLayout(widget);
        auto lineEdit = new QLineEdit(options[i].toObject()["option"].toString());
        auto deleteButton = new QPushButton("🗑️");
        layout->addWidget(new QLabel(QString("Option %1:").arg(i))); layout->addWidget(lineEdit); layout->addWidget(deleteButton);
        m_optionsLayout->addWidget(widget); m_optionWidgets.push_back(widget); m_optionEdits.push_back(lineEdit);
        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray current = m_currentQuestion["audio_options"].toArray(); if(current.size() > 1) {
                current.removeAt(i); m_currentQuestion["audio_options"] = current; refreshOptionsUI(); }
        });
    }
    QStringList answerParts; for(const auto& val : m_currentQuestion["answer"].toArray()) { answerParts.append(QString::number(val.toInt())); }
    m_answerEdit->setText(answerParts.join(", "));
}
void SequenceAudioEditor::addOption() {
    QJsonArray options = m_currentQuestion["audio_options"].toArray(); options.append(QJsonObject{{"option", "New Sound"}});
    m_currentQuestion["audio_options"] = options; refreshOptionsUI();
}
