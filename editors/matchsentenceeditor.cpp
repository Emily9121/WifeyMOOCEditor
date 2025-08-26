/*
 * File: matchsentenceeditor.cpp
 * Author: Emily
 *
 * Description:
 * The implementation for our sentence-matching editor!
 * I've fixed all the names and taught it how to use its new
 * purse! It's all grown up and ready to make quizzes for Sierra! <3
 */

#include "matchsentenceeditor.h"
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QJsonArray>
#include <QHBoxLayout>

// Assuming the constructor is set up correctly in your original file!
MatchSentenceEditor::MatchSentenceEditor(QWidget *parent) : BaseQuestionEditor(parent) {
    // This should initialize m_questionTextEdit, m_sentencesLayout, etc.
}

void MatchSentenceEditor::loadJson(const QJsonObject& question) {
    m_currentQuestion = question;
    m_questionTextEdit->setText(question["question"].toString());
    refreshSentencesUI();
}

QJsonObject MatchSentenceEditor::getJson() {
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    // Remember to save changes from the line edits here!
    return m_currentQuestion;
}

void MatchSentenceEditor::refreshSentencesUI() {
    QLayoutItem* item;
    while ((item = m_sentencesLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QJsonArray sentences = m_currentQuestion["sentences"].toArray();

    for (int i = 0; i < sentences.size(); ++i) {
        QWidget* row = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(row);
        QLineEdit* lineEdit = new QLineEdit(sentences[i].toString());
        QPushButton* deleteButton = new QPushButton("Delete");
        layout->addWidget(lineEdit);
        layout->addWidget(deleteButton);
        m_sentencesLayout->addWidget(row);

        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray current = m_currentQuestion["sentences"].toArray();
            current.removeAt(i);
            m_currentQuestion["sentences"] = current;
            refreshSentencesUI();
        });
    }
}

void MatchSentenceEditor::addSentence() {
    QJsonArray current = m_currentQuestion["sentences"].toArray();
    current.append("New Sentence");
    m_currentQuestion["sentences"] = current;
    refreshSentencesUI();
}
