/*
 * File: mcqmultipleeditor.cpp
 * Author: Emily
 *
 * Description:
 * The implementation for our multiple-choice, multiple-answer editor!
 * It's all grown up now and knows how to use its purse and follow
 * instructions! It's ready to make the best quizzes for Sierra! <3
 */

#include "mcqmultipleeditor.h"
#include "../helpers.h"
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QJsonArray>
#include <QHBoxLayout>
#include <QScrollArea>

McqMultipleEditor::McqMultipleEditor(QWidget *parent) : BaseQuestionEditor(parent) {
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    auto questionGroup = new QGroupBox("❓ Question Prompt ❓");
    auto questionLayout = new QVBoxLayout(questionGroup);
    m_questionTextEdit = new QTextEdit();
    m_questionTextEdit->setPlaceholderText("Which of these things does Emily love most about Sierra? (Choose many!) 💕");
    questionLayout->addWidget(m_questionTextEdit);
    mainLayout->addWidget(questionGroup);

    auto optionsGroup = new QGroupBox("☑ Answer Options (Pick many!) ☑");
    auto optionsGroupLayout = new QVBoxLayout(optionsGroup);
    
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    
    QWidget *scrollWidget = new QWidget();
    m_optionsLayout = new QVBoxLayout(scrollWidget);
    
    scrollArea->setWidget(scrollWidget);
    optionsGroupLayout->addWidget(scrollArea);

    auto addOptionButton = new QPushButton("Add Option");
    optionsGroupLayout->addWidget(addOptionButton);
    mainLayout->addWidget(optionsGroup);

    connect(addOptionButton, &QPushButton::clicked, this, &McqMultipleEditor::addOption);

    addOption();
    addOption();
}

void McqMultipleEditor::loadJson(const QJsonObject& question) {
    m_currentQuestion = question;
    m_questionTextEdit->setText(m_currentQuestion["question"].toString());
    refreshOptionsUI();
}

QJsonObject McqMultipleEditor::getJson() {
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();

    QJsonArray optionsArray;
    QJsonArray answerArray;
    for (int i = 0; i < m_optionWidgets.size(); ++i) {
        QWidget* widget = m_optionWidgets[i];
        QCheckBox* checkBox = widget->findChild<QCheckBox*>();
        QLineEdit* textEdit = widget->findChild<QLineEdit*>();
        
        if (checkBox && textEdit) {
            QJsonObject optionObj;
            optionObj["text"] = textEdit->text();
            // You might have an "image" field too, you can add it here!
            optionsArray.append(optionObj);
            if (checkBox->isChecked()) {
                answerArray.append(i);
            }
        }
    }
    m_currentQuestion["options"] = optionsArray;
    m_currentQuestion["answer"] = answerArray;

    return m_currentQuestion;
}

void McqMultipleEditor::clearOptions() {
    while (!m_optionWidgets.isEmpty()) {
        QWidget *widget = m_optionWidgets.takeFirst();
        m_optionsLayout->removeWidget(widget);
        widget->deleteLater();
    }
}

void McqMultipleEditor::refreshOptionsUI() {
    clearOptions();
    
    QJsonArray options = m_currentQuestion["options"].toArray();
    QJsonArray answer = m_currentQuestion["answer"].toArray();

    for (int i = 0; i < options.size(); ++i) {
        QJsonObject optionObj = options[i].toObject();
        QWidget* optionWidget = new QWidget();
        auto layout = new QHBoxLayout(optionWidget);

        QCheckBox* checkBox = new QCheckBox();
        for(const auto& ans : answer) {
            if (ans.toInt() == i) {
                checkBox->setChecked(true);
            }
        }

        QLineEdit* textEdit = new QLineEdit(optionObj["text"].toString());
        QPushButton* deleteButton = new QPushButton("Remove");

        layout->addWidget(checkBox);
        layout->addWidget(textEdit);
        layout->addWidget(deleteButton);
        m_optionsLayout->addWidget(optionWidget);
        m_optionWidgets.append(optionWidget);

        connect(deleteButton, &QPushButton::clicked, [this, optionWidget](){
            m_optionsLayout->removeWidget(optionWidget);
            m_optionWidgets.removeOne(optionWidget);
            optionWidget->deleteLater();
        });
    }
}

void McqMultipleEditor::addOption() {
    // This is a simplified addOption. We just refresh from an empty JSON object.
    // For a more advanced version, you could add a new object to m_currentQuestion
    // and then call refresh.
    QJsonObject newOption;
    newOption["text"] = "New adorable option...";
    
    QJsonArray options = m_currentQuestion["options"].toArray();
    options.append(newOption);
    m_currentQuestion["options"] = options;
    refreshOptionsUI();
}
