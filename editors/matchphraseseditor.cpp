/*
 * File: matchphraseseditor.cpp
 * Author: Emily
 *
 * Description:
 * The implementation for our phrase-matching editor!
 * It now knows how to use its purse to keep all the important
 * question data safe and sound! So clever! <3
 */

#include "matchphraseseditor.h"
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QJsonArray>
#include <QHBoxLayout>
#include <QComboBox>

// Assuming the constructor is set up correctly in your original file!
MatchPhrasesEditor::MatchPhrasesEditor(QWidget *parent) : BaseQuestionEditor(parent) {
    // This should initialize m_questionTextEdit, m_pairsLayout, etc.
}

void MatchPhrasesEditor::loadJson(const QJsonObject& question) {
    m_currentQuestion = question;
    m_questionTextEdit->setText(question["question"].toString());
    refreshPairsUI();
}

QJsonObject MatchPhrasesEditor::getJson() {
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    // Remember to save changes from the line edits here!
    return m_currentQuestion;
}

void MatchPhrasesEditor::refreshPairsUI() {
    QLayoutItem* item;
    while ((item = m_pairsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QJsonArray pairs = m_currentQuestion["pairs"].toArray();
    QJsonObject answer = m_currentQuestion["answer"].toObject();

    QStringList leftItems, rightItems;
    for (const auto& pairVal : pairs) {
        QJsonObject pairObj = pairVal.toObject();
        leftItems.append(pairObj["left"].toString());
        rightItems.append(pairObj["right"].toString());
    }

    for (int i = 0; i < leftItems.size(); ++i) {
        QWidget* row = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(row);
        QLineEdit* leftEdit = new QLineEdit(leftItems[i]);
        QComboBox* rightCombo = new QComboBox();
        rightCombo->addItems(rightItems);
        if (answer.contains(leftItems[i])) {
            rightCombo->setCurrentText(answer[leftItems[i]].toString());
        }
        QPushButton* deleteButton = new QPushButton("Delete");
        layout->addWidget(leftEdit);
        layout->addWidget(new QLabel("->"));
        layout->addWidget(rightCombo);
        layout->addWidget(deleteButton);
        m_pairsLayout->addWidget(row);

        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray current = m_currentQuestion["pairs"].toArray();
            current.removeAt(i);
            m_currentQuestion["pairs"] = current;
            refreshPairsUI();
        });
    }
}

void MatchPhrasesEditor::addPair() {
    QJsonArray current = m_currentQuestion["pairs"].toArray();
    current.append(QJsonObject{{"left", "New Left"}, {"right", "New Right"}});
    m_currentQuestion["pairs"] = current;
    refreshPairsUI();
}
