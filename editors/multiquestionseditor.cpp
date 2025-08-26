/*
 * File: multiquestionseditor.cpp
 * Author: Emily
 *
 * Description:
 * The implementation for our multi-question editor! It now knows
 * how to use its purse and is ready to handle the most complex and
 * fun quizzes for your wifey! We did it! <3
 */

#include "multiquestionseditor.h"
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonArray>

MultiQuestionsEditor::MultiQuestionsEditor(QWidget *parent) : BaseQuestionEditor(parent)
{
    auto layout = new QVBoxLayout(this);
    m_jsonTextEdit = new QTextEdit();
    m_jsonTextEdit->setPlaceholderText("This editor is super special! Edit the JSON for the sub-questions directly here, princess! 💕");
    layout->addWidget(m_jsonTextEdit);
}

void MultiQuestionsEditor::loadJson(const QJsonObject& question)
{
    m_currentQuestion = question;
    QJsonDocument doc(m_currentQuestion["questions"].toArray());
    m_jsonTextEdit->setText(doc.toJson(QJsonDocument::Indented));
}

QJsonObject MultiQuestionsEditor::getJson()
{
    QJsonDocument doc = QJsonDocument::fromJson(m_jsonTextEdit->toPlainText().toUtf8());
    m_currentQuestion["questions"] = doc.array();
    return m_currentQuestion;
}
