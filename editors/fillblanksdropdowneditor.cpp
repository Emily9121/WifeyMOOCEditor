// =======================================================================
// editors/fillblanksdropdowneditor.cpp
// =======================================================================
#include "fillblanksdropdowneditor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QJsonArray>
#include <QMessageBox>

FillBlanksDropdownEditor::FillBlanksDropdownEditor(QWidget* parent) : BaseQuestionEditor(parent) {
    auto mainLayout = new QVBoxLayout(this); setLayout(mainLayout);
    auto questionGroup = new QGroupBox("📝 Question Text 📝");
    auto questionLayout = new QVBoxLayout(questionGroup);
    m_questionTextEdit = new QTextEdit(); m_questionTextEdit->setFixedHeight(80);
    questionLayout->addWidget(m_questionTextEdit); mainLayout->addWidget(questionGroup);
    m_partsGroup = new QGroupBox("💬 Sentence Parts 💬");
    m_partsLayout = new QVBoxLayout(m_partsGroup); mainLayout->addWidget(m_partsGroup);
    m_blanksGroup = new QGroupBox("⬇️ Blanks & Options ⬇️");
    m_blanksLayout = new QVBoxLayout(m_blanksGroup); mainLayout->addWidget(m_blanksGroup);
    auto buttonLayout = new QHBoxLayout();
    auto addPartButton = new QPushButton("➕ Add Part"); auto addBlankButton = new QPushButton("➕ Add Blank");
    auto saveButton = new QPushButton("💾 Save Question");
    buttonLayout->addWidget(addPartButton); buttonLayout->addWidget(addBlankButton); buttonLayout->addWidget(saveButton);
    mainLayout->addLayout(buttonLayout); mainLayout->addStretch();
    connect(addPartButton, &QPushButton::clicked, this, &FillBlanksDropdownEditor::addPart);
    connect(addBlankButton, &QPushButton::clicked, this, &FillBlanksDropdownEditor::addBlank);
    connect(saveButton, &QPushButton::clicked, [this](){ getSavedQuestion(); QMessageBox::information(this, "Success! 💖", "Question saved beautifully!"); });
}
void FillBlanksDropdownEditor::loadQuestion(const QJsonObject& question) { m_currentQuestion = question; m_questionTextEdit->setText(question["question"].toString()); refreshPartsUI(); refreshBlanksUI(); }
QJsonObject FillBlanksDropdownEditor::getSavedQuestion() {
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    QJsonArray partsArray; for(auto* edit : m_partEdits) { partsArray.append(edit->toPlainText()); }
    m_currentQuestion["sentence_parts"] = partsArray;
    QJsonArray optionsForBlanksArray; QJsonArray answersArray;
    for(const auto& blankWidgets : m_blankWidgets) {
        QStringList options = blankWidgets.optionsEdit->text().split(';', Qt::SkipEmptyParts);
        for(auto& opt : options) { opt = opt.trimmed(); }
        optionsForBlanksArray.append(QJsonArray::fromStringList(options));
        answersArray.append(blankWidgets.answerCombo->currentText());
    }
    m_currentQuestion["options_for_blanks"] = optionsForBlanksArray; m_currentQuestion["answers"] = answersArray;
    return m_currentQuestion;
}
void FillBlanksDropdownEditor::refreshPartsUI() {
    qDeleteAll(m_partWidgets); m_partWidgets.clear(); m_partEdits.clear();
    QJsonArray parts = m_currentQuestion["sentence_parts"].toArray();
    for (int i = 0; i < parts.size(); ++i) {
        auto partWidget = new QWidget(); auto layout = new QHBoxLayout(partWidget);
        auto textEdit = new QTextEdit(parts[i].toString()); textEdit->setFixedHeight(60);
        auto deleteButton = new QPushButton("🗑️");
        layout->addWidget(new QLabel(QString("Part %1:").arg(i+1))); layout->addWidget(textEdit); layout->addWidget(deleteButton);
        m_partsLayout->addWidget(partWidget); m_partWidgets.push_back(partWidget); m_partEdits.push_back(textEdit);
        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray current = m_currentQuestion["sentence_parts"].toArray(); if(current.size() > 1) {
                current.removeAt(i); m_currentQuestion["sentence_parts"] = current; refreshPartsUI(); }
        });
    }
}
void FillBlanksDropdownEditor::refreshBlanksUI() {
    for(auto& widgets : m_blankWidgets) { delete widgets.container; } m_blankWidgets.clear();
    QJsonArray optionsForBlanks = m_currentQuestion["options_for_blanks"].toArray(); QJsonArray answers = m_currentQuestion["answers"].toArray();
    for (int i = 0; i < optionsForBlanks.size(); ++i) {
        auto container = new QGroupBox(QString("💖 Blank %1 💖").arg(i+1)); auto layout = new QVBoxLayout(container);
        QStringList options; for(const auto& optVal : optionsForBlanks[i].toArray()) { options.append(optVal.toString()); }
        auto optionsLayout = new QHBoxLayout(); optionsLayout->addWidget(new QLabel("Options (use ; to separate):"));
        auto optionsEdit = new QLineEdit(options.join("; ")); optionsLayout->addWidget(optionsEdit); layout->addLayout(optionsLayout);
        auto answerLayout = new QHBoxLayout(); answerLayout->addWidget(new QLabel("✅ Correct Answer:"));
        auto answerCombo = new QComboBox(); answerCombo->addItems(options);
        if(i < answers.size()) { answerCombo->setCurrentText(answers[i].toString()); }
        answerLayout->addWidget(answerCombo); layout->addLayout(answerLayout);
        auto deleteButton = new QPushButton("🗑️ Delete Blank"); layout->addWidget(deleteButton, 0, Qt::AlignRight);
        m_blanksLayout->addWidget(container); m_blankWidgets.push_back({container, optionsEdit, answerCombo});
        connect(optionsEdit, &QLineEdit::textChanged, [answerCombo](const QString& text){
            QStringList opts = text.split(';', Qt::SkipEmptyParts);
            for(auto& opt : opts) { opt = opt.trimmed(); }
            answerCombo->clear(); answerCombo->addItems(opts);
        });
        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray currentOpts = m_currentQuestion["options_for_blanks"].toArray(); QJsonArray currentAns = m_currentQuestion["answers"].toArray();
            currentOpts.removeAt(i); if(i < currentAns.size()) currentAns.removeAt(i);
            m_currentQuestion["options_for_blanks"] = currentOpts; m_currentQuestion["answers"] = currentAns; refreshBlanksUI();
        });
    }
}
void FillBlanksDropdownEditor::addPart() { QJsonArray current = m_currentQuestion["sentence_parts"].toArray(); current.append("new part..."); m_currentQuestion["sentence_parts"] = current; refreshPartsUI(); }
void FillBlanksDropdownEditor::addBlank() {
    QJsonArray currentOpts = m_currentQuestion["options_for_blanks"].toArray(); QJsonArray currentAns = m_currentQuestion["answers"].toArray();
    currentOpts.append(QJsonArray{"Option A", "Option B"}); currentAns.append("Option A");
    m_currentQuestion["options_for_blanks"] = currentOpts; m_currentQuestion["answers"] = currentAns; refreshBlanksUI();
}
