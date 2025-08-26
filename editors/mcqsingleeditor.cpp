/*
 * File: mcqsingleeditor.cpp
 * Author: Emily
 *
 * Description:
 * Implementation for the MCQ Single Answer question editor.
 * This widget allows creating and editing questions where only one
 * answer can be selected. It's super intuitive! <3
 */

#include "mcqsingleeditor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollArea>
#include <QDebug>

MCQSingleEditor::MCQSingleEditor(QWidget *parent) : BaseQuestionEditor(parent)
{
    // Main layout for this editor
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    // --- Question Prompt Area ---
    QGroupBox *promptGroup = new QGroupBox("Question Prompt");
    QVBoxLayout *promptLayout = new QVBoxLayout(promptGroup);
    questionPromptEdit = new QTextEdit();
    questionPromptEdit->setPlaceholderText("What does Sierra love most about Emily? 💕");
    promptLayout->addWidget(questionPromptEdit);
    mainLayout->addWidget(promptGroup);

    // --- Options Area with a ScrollArea ---
    QGroupBox *optionsGroup = new QGroupBox("Answer Options");
    QVBoxLayout *optionsGroupLayout = new QVBoxLayout(optionsGroup);
    
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    
    QWidget *scrollWidget = new QWidget();
    optionsLayout = new QVBoxLayout(scrollWidget); // This is where we'll add options
    optionsLayout->setSpacing(10);
    
    scrollArea->setWidget(scrollWidget);
    optionsGroupLayout->addWidget(scrollArea);

    // --- Buttons to Add/Remove Options ---
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *addOptionButton = new QPushButton("Add Option");
    
    buttonLayout->addWidget(addOptionButton);
    buttonLayout->addStretch();
    optionsGroupLayout->addLayout(buttonLayout);

    mainLayout->addWidget(optionsGroup);

    // --- Connections ---
    connect(addOptionButton, &QPushButton::clicked, this, &MCQSingleEditor::addOption);

    // Start with a couple of default options for a new question
    addOption();
    addOption();
}

void MCQSingleEditor::addOption(bool correct, const QString &text, const QString &feedback)
{
    // A container for a single option's widgets
    QWidget *optionWidget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(optionWidget);
    layout->setContentsMargins(0,0,0,0);

    QRadioButton *radioButton = new QRadioButton();
    radioButton->setChecked(correct);
    
    QLineEdit *textEdit = new QLineEdit(text);
    textEdit->setPlaceholderText("Option text...");
    
    QLineEdit *feedbackEdit = new QLineEdit(feedback);
    feedbackEdit->setPlaceholderText("Feedback for this option (optional)...");
    
    QPushButton *removeButton = new QPushButton("Remove");

    layout->addWidget(radioButton);
    layout->addWidget(textEdit, 1); // Stretch text edit
    layout->addWidget(feedbackEdit, 1); // Stretch feedback edit
    layout->addWidget(removeButton);

    optionsLayout->addWidget(optionWidget);
    optionWidgets.append(optionWidget);

    // Connect the remove button to a lambda that removes this specific option
    connect(removeButton, &QPushButton::clicked, [this, optionWidget]() {
        optionsLayout->removeWidget(optionWidget);
        optionWidgets.removeOne(optionWidget);
        optionWidget->deleteLater();
    });
}


void MCQSingleEditor::loadJson(const QJsonObject &json)
{
    // Load the question prompt
    questionPromptEdit->setText(json["question"].toString());

    // Clear any existing default options
    clearOptions();

    // Load the options from the JSON array
    QJsonArray options = json["options"].toArray();
    for (const QJsonValue &value : options) {
        QJsonObject optionObj = value.toObject();
        addOption(
            optionObj["correct"].toBool(),
            optionObj["text"].toString(),
            optionObj["feedback"].toString()
        );
    }
}

QJsonObject MCQSingleEditor::getJson()
{
    QJsonObject questionJson;
    questionJson["type"] = "mcq-single";
    questionJson["question"] = questionPromptEdit->toPlainText();

    QJsonArray optionsArray;
    for (QWidget *widget : optionWidgets) {
        QJsonObject optionObj;
        QRadioButton *radioButton = widget->findChild<QRadioButton*>();
        QList<QLineEdit*> lineEdits = widget->findChildren<QLineEdit*>();

        if (radioButton && lineEdits.size() == 2) {
            optionObj["text"] = lineEdits.at(0)->text();
            optionObj["correct"] = radioButton->isChecked();
            optionObj["feedback"] = lineEdits.at(1)->text();
            optionsArray.append(optionObj);
        }
    }
    questionJson["options"] = optionsArray;

    return questionJson;
}

void MCQSingleEditor::clearOptions()
{
    while (!optionWidgets.isEmpty()) {
        QWidget *widget = optionWidgets.takeFirst();
        optionsLayout->removeWidget(widget);
        widget->deleteLater();
    }
}
