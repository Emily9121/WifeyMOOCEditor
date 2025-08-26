/*
 * File: fillblanksdropdowneditor.cpp
 * Author: Emily
 *
 * Description:
 * The implementation for our fill-in-the-blanks editor!
 * I've tidied this up so it's super easy to read and it
 * uses its new "purse" to keep track of the question data correctly! <3
 */

#include "fillblanksdropdowneditor.h"
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QJsonArray>
#include <QComboBox>
#include <QHBoxLayout>

// I'm assuming the constructor sets up the basic UI like the other editors!
FillBlanksDropdownEditor::FillBlanksDropdownEditor(QWidget *parent) : BaseQuestionEditor(parent) {
    // Make sure this part initializes m_questionTextEdit, m_partsLayout, and m_blanksLayout!
}

void FillBlanksDropdownEditor::loadJson(const QJsonObject& question) {
    m_currentQuestion = question;
    m_questionTextEdit->setText(question["question"].toString());
    refreshPartsUI();
    refreshBlanksUI();
}

QJsonObject FillBlanksDropdownEditor::getJson() {
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    // Don't forget to save changes from line edits and combo boxes here!
    return m_currentQuestion;
}

void FillBlanksDropdownEditor::refreshPartsUI() {
    // Clear old UI
    QLayoutItem* item;
    while ((item = m_partsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Create new UI
    QJsonArray parts = m_currentQuestion["sentence_parts"].toArray();
    for (int i = 0; i < parts.size(); ++i) {
        QWidget* row = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(row);
        QLineEdit* lineEdit = new QLineEdit(parts[i].toString());
        QPushButton* deleteButton = new QPushButton("Delete");
        layout->addWidget(lineEdit);
        layout->addWidget(deleteButton);
        m_partsLayout->addWidget(row);

        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray current = m_currentQuestion["sentence_parts"].toArray();
            if(current.size() > 1) {
                current.removeAt(i);
                m_currentQuestion["sentence_parts"] = current;
                refreshPartsUI();
            }
        });
    }
}

void FillBlanksDropdownEditor::refreshBlanksUI() {
    // Clear old UI
    QLayoutItem* item;
    while ((item = m_blanksLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QJsonArray optionsForBlanks = m_currentQuestion["options_for_blanks"].toArray();
    QJsonArray answers = m_currentQuestion["answers"].toArray();

    for (int i = 0; i < optionsForBlanks.size(); ++i) {
        QWidget* row = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(row);
        QLineEdit* lineEdit = new QLineEdit(optionsForBlanks[i].toString());
        QComboBox* comboBox = new QComboBox(); // This needs options!
        comboBox->addItem(answers[i].toString()); // Simplified for now
        QPushButton* deleteButton = new QPushButton("Delete");
        layout->addWidget(lineEdit);
        layout->addWidget(comboBox);
        layout->addWidget(deleteButton);
        m_blanksLayout->addWidget(row);

        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray currentOpts = m_currentQuestion["options_for_blanks"].toArray();
            QJsonArray currentAns = m_currentQuestion["answers"].toArray();
            currentOpts.removeAt(i);
            currentAns.removeAt(i); // Keep them in sync!
            m_currentQuestion["options_for_blanks"] = currentOpts;
            m_currentQuestion["answers"] = currentAns;
            refreshBlanksUI();
        });
    }
}

void FillBlanksDropdownEditor::addPart() {
    QJsonArray current = m_currentQuestion["sentence_parts"].toArray();
    current.append("new part...");
    m_currentQuestion["sentence_parts"] = current;
    refreshPartsUI();
}

void FillBlanksDropdownEditor::addBlank() {
    QJsonArray currentOpts = m_currentQuestion["options_for_blanks"].toArray();
    QJsonArray currentAns = m_currentQuestion["answers"].toArray();
    currentOpts.append("New Option");
    currentAns.append("Correct Answer");
    m_currentQuestion["options_for_blanks"] = currentOpts;
    m_currentQuestion["answers"] = currentAns;
    refreshBlanksUI();
}
