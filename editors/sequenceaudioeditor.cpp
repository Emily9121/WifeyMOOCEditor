/*
 * File: sequenceaudioeditor.cpp
 * Author: Emily
 *
 * Description:
 * The implementation for our audio-sequencing editor!
 * It now knows how to use its purse to keep all the important
 * audio data safe and sound! So clever! <3
 */

#include "sequenceaudioeditor.h"
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QJsonArray>
#include <QHBoxLayout>
#include <QJsonObject>

// Assuming the constructor is set up correctly in your original file!
SequenceAudioEditor::SequenceAudioEditor(QWidget *parent) : BaseQuestionEditor(parent) {
    // This should initialize m_questionTextEdit, m_optionsLayout, etc.
}

void SequenceAudioEditor::loadJson(const QJsonObject& question) {
    m_currentQuestion = question;
    m_questionTextEdit->setText(question["question"].toString());
    refreshOptionsUI();
}

QJsonObject SequenceAudioEditor::getJson() {
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    // Remember to save changes from the line edits here!
    return m_currentQuestion;
}

void SequenceAudioEditor::refreshOptionsUI() {
    QLayoutItem* item;
    while ((item = m_optionsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QJsonArray options = m_currentQuestion["audio_options"].toArray();

    for (int i = 0; i < options.size(); ++i) {
        QWidget* row = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(row);
        QLineEdit* lineEdit = new QLineEdit(options[i].toObject()["option"].toString());
        QPushButton* deleteButton = new QPushButton("Delete");
        layout->addWidget(lineEdit);
        layout->addWidget(deleteButton);
        m_optionsLayout->addWidget(row);

        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray current = m_currentQuestion["audio_options"].toArray();
            if(current.size() > 1) {
                current.removeAt(i);
                m_currentQuestion["audio_options"] = current;
                refreshOptionsUI();
            }
        });
    }
}

void SequenceAudioEditor::addOption() {
    QJsonArray options = m_currentQuestion["audio_options"].toArray();
    options.append(QJsonObject{{"option", "New Sound"}});
    m_currentQuestion["audio_options"] = options;
    refreshOptionsUI();
}
