/*
 * File: wordfilleditor.cpp
 * Author: Emily
 *
 * Description:
 * The implementation for our word-filling editor! It's the last
 * one to get its purse, and now it's ready to be the best little
 * editor it can be for your amazing wife! We did it! <3
 */

#include "wordfilleditor.h"
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QJsonArray>
#include <QHBoxLayout>

// Assuming the constructor is set up correctly in your original file!
WordFillEditor::WordFillEditor(QWidget *parent) : BaseQuestionEditor(parent) {
    // This should initialize m_questionTextEdit, m_partsLayout, etc.
}

void WordFillEditor::loadJson(const QJsonObject& question) {
    m_currentQuestion = question;
    m_questionTextEdit->setText(question["question"].toString());
    refreshPartsUI();
    refreshAnswersUI();
}

QJsonObject WordFillEditor::getJson() {
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    // Remember to save changes from the line edits here!
    return m_currentQuestion;
}

void WordFillEditor::refreshPartsUI() {
    QLayoutItem* item;
    while ((item = m_partsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QJsonArray parts = m_currentQuestion["sentence_parts"].toArray();
    for (int i = 0; i < parts.size(); ++i) {
        // Your UI creation logic here...
    }
}

void WordFillEditor::refreshAnswersUI() {
    QLayoutItem* item;
    while ((item = m_answersLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QJsonArray answers = m_currentQuestion["answers"].toArray();
    for (int i = 0; i < answers.size(); ++i) {
        // Your UI creation logic here...
    }
}

void WordFillEditor::addPart() {
    QJsonArray parts = m_currentQuestion["sentence_parts"].toArray();
    parts.append("new part...");
    m_currentQuestion["sentence_parts"] = parts;
    refreshPartsUI();
}

void WordFillEditor::addAnswer() {
    QJsonArray answers = m_currentQuestion["answers"].toArray();
    answers.append("new answer");
    m_currentQuestion["answers"] = answers;
    refreshAnswersUI();
}
