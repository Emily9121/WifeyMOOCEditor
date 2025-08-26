/*
 * File: categorizationeditor.cpp
 * Author: Emily
 *
 * Description:
 * The implementation for our categorization editor!
 * I've tidied this up so it's super easy to read and it
 * uses its new "purse" to keep track of the question data correctly! <3
 */

#include "categorizationeditor.h"
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QJsonArray>
#include <QComboBox>
#include <QHBoxLayout>

// Constructor and other functions are in your original file...
// I'm just fixing the ones that had the little bugs!

CategorizationEditor::CategorizationEditor(QWidget *parent) : BaseQuestionEditor(parent) {
    // This function is already set up in your file, so I'll leave it be!
    // Just make sure it initializes the layouts and widgets.
}


void CategorizationEditor::loadJson(const QJsonObject& question) {
    m_currentQuestion = question;
    m_questionTextEdit->setText(question["question"].toString());
    refreshCategoriesUI();
    refreshStimuliUI();
}

QJsonObject CategorizationEditor::getJson() {
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    return m_currentQuestion;
}

void CategorizationEditor::refreshCategoriesUI() {
    // Clear old UI
    QLayoutItem* item;
    while ((item = m_categoriesLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Create new UI
    QJsonArray categories = m_currentQuestion["categories"].toArray();
    for (int i = 0; i < categories.size(); ++i) {
        QWidget* row = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(row);
        QLineEdit* lineEdit = new QLineEdit(categories[i].toString());
        QPushButton* deleteButton = new QPushButton("Delete");
        layout->addWidget(lineEdit);
        layout->addWidget(deleteButton);
        m_categoriesLayout->addWidget(row);

        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray current = m_currentQuestion["categories"].toArray();
            current.removeAt(i);
            m_currentQuestion["categories"] = current;
            refreshCategoriesUI();
            refreshStimuliUI();
        });
    }
}

void CategorizationEditor::refreshStimuliUI() {
    // Clear old UI
    QLayoutItem* item;
    while ((item = m_stimuliLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Get categories for the dropdown
    QStringList categoryList;
    for(const auto& catVal : m_currentQuestion["categories"].toArray()) {
        categoryList.append(catVal.toString());
    }

    QJsonObject answer = m_currentQuestion["answer"].toObject();
    QJsonArray stimuli = m_currentQuestion["stimuli"].toArray();

    for (int i = 0; i < stimuli.size(); ++i) {
        QJsonObject stimulus = stimuli[i].toObject();
        QWidget* row = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(row);
        QLineEdit* lineEdit = new QLineEdit(stimulus["text"].toString());
        QComboBox* comboBox = new QComboBox();
        comboBox->addItems(categoryList);
        if (answer.contains(stimulus["text"].toString())) {
            comboBox->setCurrentText(answer[stimulus["text"].toString()].toString());
        }
        QPushButton* deleteButton = new QPushButton("Delete");
        layout->addWidget(lineEdit);
        layout->addWidget(comboBox);
        layout->addWidget(deleteButton);
        m_stimuliLayout->addWidget(row);

        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray current = m_currentQuestion["stimuli"].toArray();
            current.removeAt(i);
            m_currentQuestion["stimuli"] = current;
            refreshStimuliUI();
        });
    }
}

void CategorizationEditor::addCategory() {
    QJsonArray current = m_currentQuestion["categories"].toArray();
    current.append("New Category");
    m_currentQuestion["categories"] = current;
    refreshCategoriesUI();
    refreshStimuliUI();
}

void CategorizationEditor::addStimulus() {
    QJsonArray current = m_currentQuestion["stimuli"].toArray();
    current.append(QJsonObject{{"text", "New Item"}, {"image", ""}});
    m_currentQuestion["stimuli"] = current;
    refreshStimuliUI();
}
