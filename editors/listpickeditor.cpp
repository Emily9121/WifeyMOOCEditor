/*
 * File: listpickeditor.cpp
 * Author: Emily
 *
 * Description:
 * The implementation for our list-picking editor!
 * I've taught it how to use its new purse so it never forgets
 * the question data again! So responsible! <3
 */

#include "listpickeditor.h"
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QJsonArray>
#include <QHBoxLayout>

// Assuming the constructor is set up correctly in your original file!
ListPickEditor::ListPickEditor(QWidget *parent) : BaseQuestionEditor(parent) {
    // This should initialize m_questionTextEdit, m_optionsLayout, etc.
}

void ListPickEditor::loadJson(const QJsonObject& question) {
    m_currentQuestion = question;
    m_questionTextEdit->setText(m_currentQuestion["question"].toString());
    refreshOptionsUI();
}

QJsonObject ListPickEditor::getJson() {
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    // Remember to save the state of the checkboxes here!
    return m_currentQuestion;
}

void ListPickEditor::refreshOptionsUI() {
    QLayoutItem* item;
    while ((item = m_optionsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QJsonArray options = m_currentQuestion["options"].toArray();
    QJsonArray answerArray = m_currentQuestion["answer"].toArray();

    for (int i = 0; i < options.size(); ++i) {
        QWidget* row = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(row);
        QCheckBox* checkBox = new QCheckBox();
        for (const auto& ans : answerArray) {
            if (ans.toString() == options[i].toString()) {
                checkBox->setChecked(true);
            }
        }
        QLineEdit* lineEdit = new QLineEdit(options[i].toString());
        QPushButton* deleteButton = new QPushButton("Delete");
        layout->addWidget(checkBox);
        layout->addWidget(lineEdit);
        layout->addWidget(deleteButton);
        m_optionsLayout->addWidget(row);

        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray currentOptions = m_currentQuestion["options"].toArray();
            if(currentOptions.size() > 1) {
                currentOptions.removeAt(i);
                m_currentQuestion["options"] = currentOptions;
                m_currentQuestion["answer"] = QJsonArray{}; // Reset answer when an option is removed
                refreshOptionsUI();
            }
        });
    }
}

void ListPickEditor::addOption() {
    QJsonArray options = m_currentQuestion["options"].toArray();
    options.append("New Cute Option");
    m_currentQuestion["options"] = options;
    refreshOptionsUI();
}
