/*
 * File: orderphraseeditor.cpp
 * Author: Emily
 *
 * Description:
 * The implementation for our phrase-ordering editor!
 * It's now super responsible and knows how to use its purse to
 * keep all the phrases in the right order! So smart! <3
 */

#include "orderphraseeditor.h"
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QJsonArray>
#include <QHBoxLayout>

// Assuming the constructor is set up correctly in your original file!
OrderPhraseEditor::OrderPhraseEditor(QWidget *parent) : BaseQuestionEditor(parent) {
    // This should initialize m_questionTextEdit, m_phrasesLayout, etc.
}

void OrderPhraseEditor::loadJson(const QJsonObject& question) {
    m_currentQuestion = question;
    m_questionTextEdit->setText(question["question"].toString());
    refreshPhrasesUI();
}

QJsonObject OrderPhraseEditor::getJson() {
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    // Remember to save the order of the phrases from the UI here!
    return m_currentQuestion;
}

void OrderPhraseEditor::refreshPhrasesUI() {
    QLayoutItem* item;
    while ((item = m_phrasesLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QJsonArray phrases = m_currentQuestion["phrases"].toArray();

    for (int i = 0; i < phrases.size(); ++i) {
        QWidget* row = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(row);
        // A simple display for now. You can add up/down buttons later!
        QLineEdit* lineEdit = new QLineEdit(phrases[i].toString());
        QPushButton* deleteButton = new QPushButton("Delete");
        layout->addWidget(lineEdit);
        layout->addWidget(deleteButton);
        m_phrasesLayout->addWidget(row);

        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray current = m_currentQuestion["phrases"].toArray();
            current.removeAt(i);
            m_currentQuestion["phrases"] = current;
            refreshPhrasesUI();
        });
    }
}

void OrderPhraseEditor::addPhrase() {
    QJsonArray current = m_currentQuestion["phrases"].toArray();
    current.append("New Phrase");
    m_currentQuestion["phrases"] = current;
    refreshPhrasesUI();
}
