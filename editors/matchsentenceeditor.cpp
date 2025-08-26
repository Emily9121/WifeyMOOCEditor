// =======================================================================
// editors/matchphraseseditor.cpp
// =======================================================================
#include "matchphraseseditor.h"
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

MatchPhrasesEditor::MatchPhrasesEditor(QWidget* parent) : BaseQuestionEditor(parent) {
    auto mainLayout = new QVBoxLayout(this); setLayout(mainLayout);
    auto questionGroup = new QGroupBox("📝 Question Text 📝");
    auto questionLayout = new QVBoxLayout(questionGroup);
    m_questionTextEdit = new QTextEdit(); m_questionTextEdit->setFixedHeight(80);
    questionLayout->addWidget(m_questionTextEdit); mainLayout->addWidget(questionGroup);
    m_pairsGroup = new QGroupBox("🔗 Phrase Pairs 🔗");
    m_pairsLayout = new QVBoxLayout(m_pairsGroup); mainLayout->addWidget(m_pairsGroup);
    auto buttonLayout = new QHBoxLayout();
    auto addPairButton = new QPushButton("➕ Add Pair");
    auto saveButton = new QPushButton("💾 Save Question");
    buttonLayout->addWidget(addPairButton); buttonLayout->addWidget(saveButton);
    mainLayout->addLayout(buttonLayout); mainLayout->addStretch();
    connect(addPairButton, &QPushButton::clicked, this, &MatchPhrasesEditor::addPair);
    connect(saveButton, &QPushButton::clicked, [this](){ getJson(); QMessageBox::information(this, "Success! 💖", "Question saved beautifully!"); });
}
void MatchPhrasesEditor::loadJson(const QJsonObject& question) { m_currentQuestion = question; m_questionTextEdit->setText(question["question"].toString()); refreshPairsUI(); }
QJsonObject MatchPhrasesEditor::getJson() {
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    QJsonArray pairsArray; QJsonObject answerObject;
    for(const auto& pairWidgets : m_pairWidgets) {
        QString source = pairWidgets.sourceEdit->text();
        QStringList targets = pairWidgets.targetsEdit->text().split(';', Qt::SkipEmptyParts);
        for(auto& t : targets) { t = t.trimmed(); }
        QString answer = pairWidgets.answerCombo->currentText();
        if (!source.isEmpty()) {
            pairsArray.append(QJsonObject{{"source", source}, {"targets", QJsonArray::fromStringList(targets)}});
            answerObject[source] = answer;
        }
    }
    m_currentQuestion["pairs"] = pairsArray; m_currentQuestion["answer"] = answerObject;
    return m_currentQuestion;
}
void MatchPhrasesEditor::refreshPairsUI() {
    for(auto& widgets : m_pairWidgets) { delete widgets.container; } m_pairWidgets.clear();
    QJsonArray pairs = m_currentQuestion["pairs"].toArray(); QJsonObject answer = m_currentQuestion["answer"].toObject();
    for (int i = 0; i < pairs.size(); ++i) {
        QJsonObject pairObj = pairs[i].toObject(); QString source = pairObj["source"].toString();
        QStringList targets; for(const auto& t : pairObj["targets"].toArray()) { targets.append(t.toString()); }
        auto container = new QGroupBox(QString("💖 Pair %1 💖").arg(i+1)); auto layout = new QVBoxLayout(container);
        auto sourceLayout = new QHBoxLayout(); sourceLayout->addWidget(new QLabel("Source Phrase:"));
        auto sourceEdit = new QLineEdit(source); sourceLayout->addWidget(sourceEdit); layout->addLayout(sourceLayout);
        auto targetsLayout = new QHBoxLayout(); targetsLayout->addWidget(new QLabel("Target Options (use ; to separate):"));
        auto targetsEdit = new QLineEdit(targets.join("; ")); targetsLayout->addWidget(targetsEdit); layout->addLayout(targetsLayout);
        auto answerLayout = new QHBoxLayout(); answerLayout->addWidget(new QLabel("✅ Correct Target:"));
        auto answerCombo = new QComboBox(); answerCombo->addItems(targets);
        answerCombo->setCurrentText(answer[source].toString());
        answerLayout->addWidget(answerCombo); layout->addLayout(answerLayout);
        auto deleteButton = new QPushButton("🗑️ Delete Pair"); layout->addWidget(deleteButton, 0, Qt::AlignRight);
        m_pairsLayout->addWidget(container); m_pairWidgets.push_back({container, sourceEdit, targetsEdit, answerCombo});
        connect(targetsEdit, &QLineEdit::textChanged, [answerCombo](const QString& text){
            QStringList opts = text.split(';', Qt::SkipEmptyParts); for(auto& opt : opts) { opt = opt.trimmed(); }
            answerCombo->clear(); answerCombo->addItems(opts);
        });
        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray current = m_currentQuestion["pairs"].toArray(); current.removeAt(i);
            m_currentQuestion["pairs"] = current; refreshPairsUI();
        });
    }
}
void MatchPhrasesEditor::addPair() {
    QJsonArray current = m_currentQuestion["pairs"].toArray();
    current.append(QJsonObject{{"source", "New Source..."}, {"targets", QJsonArray{"", "ending A", "ending B"}}});
    m_currentQuestion["pairs"] = current; refreshPairsUI();
}
