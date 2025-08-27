/*
* File: listpickeditor.cpp
* Author: Emily
*
* Description:
* The COMPLETE implementation for our list-picking editor!
* Perfect for multi-select checkbox lists! 📝💖
*/

#include "listpickeditor.h"
#include "../helpers.h"
#include <QMessageBox>

ListPickEditor::ListPickEditor(QWidget *parent) : BaseQuestionEditor(parent) 
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    // Question text section
    auto questionGroup = new QGroupBox("❓ Question Prompt ❓");
    auto questionLayout = new QVBoxLayout(questionGroup);

    m_questionTextEdit = new QTextEdit();
    m_questionTextEdit->setPlaceholderText("Pick all the cute options you want! 💖");
    m_questionTextEdit->setMaximumHeight(100);
    questionLayout->addWidget(m_questionTextEdit);

    mainLayout->addWidget(questionGroup);

    // Options section
    auto optionsGroup = new QGroupBox("☑️ Selectable Options ☑️");
    auto optionsGroupLayout = new QVBoxLayout(optionsGroup);

    auto optionsLabel = new QLabel("💡 Create options that students can select (multiple choice checkboxes)!");
    optionsLabel->setWordWrap(true);
    optionsLabel->setStyleSheet("font-style: italic; color: #8B008B;");
    optionsGroupLayout->addWidget(optionsLabel);

    QScrollArea *optionsScrollArea = new QScrollArea();
    optionsScrollArea->setWidgetResizable(true);
    QWidget *optionsScrollWidget = new QWidget();
    m_optionsLayout = new QVBoxLayout(optionsScrollWidget);
    optionsScrollArea->setWidget(optionsScrollWidget);

    optionsGroupLayout->addWidget(optionsScrollArea);

    auto addOptionButton = new QPushButton("Add Option ☑️");
    connect(addOptionButton, &QPushButton::clicked, this, &ListPickEditor::addOption);
    optionsGroupLayout->addWidget(addOptionButton);

    mainLayout->addWidget(optionsGroup);

    // Initialize with defaults
    m_currentQuestion["type"] = "list_pick";
    m_currentQuestion["question"] = "";
    m_currentQuestion["options"] = QJsonArray{"Option 1", "Option 2", "Option 3"};
    m_currentQuestion["answer"] = QJsonArray{0};

    refreshOptionsUI();
}

void ListPickEditor::loadJson(const QJsonObject& question) 
{
    m_currentQuestion = question;

    m_questionTextEdit->setText(question["question"].toString());

    refreshOptionsUI();
}

QJsonObject ListPickEditor::getJson() 
{
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    m_currentQuestion["type"] = "list_pick";

    // Save options and generate answer array
    QJsonArray optionsArray;
    QJsonArray answerArray;

    for (int i = 0; i < m_optionWidgets.size(); ++i) {
        QWidget* widget = m_optionWidgets[i];
        QCheckBox* checkBox = widget->findChild<QCheckBox*>();
        QLineEdit* lineEdit = widget->findChild<QLineEdit*>();

        if (checkBox && lineEdit) {
            QString optionText = lineEdit->text().trimmed();
            if (!optionText.isEmpty()) {
                optionsArray.append(optionText);

                if (checkBox->isChecked()) {
                    answerArray.append(i);
                }
            }
        }
    }

    m_currentQuestion["options"] = optionsArray;
    m_currentQuestion["answer"] = answerArray;

    return m_currentQuestion;
}

void ListPickEditor::refreshOptionsUI() 
{
    clearOptions();

    QJsonArray options = m_currentQuestion["options"].toArray();
    QJsonArray answerArray = m_currentQuestion["answer"].toArray();

    // Convert answer array to set for quick lookup
    QSet<int> correctIndices;
    for (const QJsonValue& ansVal : answerArray) {
        correctIndices.insert(ansVal.toInt());
    }

    for (int i = 0; i < options.size(); ++i) {
        QString optionText = options[i].toString();

        QWidget* row = new QWidget();
        row->setStyleSheet("QWidget { border: 1px solid #FF69B4; border-radius: 5px; margin: 2px; padding: 5px; }");
        auto layout = new QHBoxLayout(row);

        // Checkbox for "this is a correct answer"
        QCheckBox* checkBox = new QCheckBox();
        checkBox->setChecked(correctIndices.contains(i));
        checkBox->setToolTip("Check if this option should be selected by default");

        // Option text
        QLineEdit* lineEdit = new QLineEdit(optionText);
        lineEdit->setPlaceholderText("Option text...");

        // Delete button
        QPushButton* deleteButton = new QPushButton("Delete 🗑️");
        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray currentOptions = m_currentQuestion["options"].toArray();
            if (currentOptions.size() > 1) {
                currentOptions.removeAt(i);
                m_currentQuestion["options"] = currentOptions;

                // Update answer indices
                QJsonArray currentAnswers = m_currentQuestion["answer"].toArray();
                QJsonArray newAnswers;
                for (const QJsonValue& ansVal : currentAnswers) {
                    int ansIndex = ansVal.toInt();
                    if (ansIndex < i) {
                        newAnswers.append(ansIndex);
                    } else if (ansIndex > i) {
                        newAnswers.append(ansIndex - 1);
                    }
                    // Skip if ansIndex == i (the deleted option)
                }
                m_currentQuestion["answer"] = newAnswers;

                refreshOptionsUI();
            }
        });

        layout->addWidget(checkBox);
        layout->addWidget(new QLabel("✅"));
        layout->addWidget(lineEdit, 1);
        layout->addWidget(deleteButton);

        m_optionsLayout->addWidget(row);
        m_optionWidgets.append(row);
    }
}

void ListPickEditor::addOption() 
{
    QJsonArray options = m_currentQuestion["options"].toArray();
    options.append("New cute option");
    m_currentQuestion["options"] = options;
    refreshOptionsUI();
}

void ListPickEditor::clearOptions()
{
    for (QWidget* widget : m_optionWidgets) {
        m_optionsLayout->removeWidget(widget);
        widget->deleteLater();
    }
    m_optionWidgets.clear();
}
