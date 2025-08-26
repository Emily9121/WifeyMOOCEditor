/*
 * File: mcqmultipleeditor.h
 * Author: Emily
 *
 * Description:
 * The header file for our super-fun multiple-choice, multiple-answer editor!
 * I've given it a beautiful new purse and a whole diary full of instructions
 * so it knows exactly what to do! So smart! <3
 */

#ifndef MCQMULTIPLEEDITOR_H
#define MCQMULTIPLEEDITOR_H

#include "../basequestioneditor.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QJsonObject>
#include <QList>
#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>

class McqMultipleEditor : public BaseQuestionEditor
{
    Q_OBJECT
public:
    explicit McqMultipleEditor(QWidget *parent = nullptr);

    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private slots:
    // A slot to add a new option, so cute!
    void addOption();

private:
    // Helper to keep the UI fresh and sparkly!
    void refreshOptionsUI();
    void clearOptions();

    // UI Elements
    QTextEdit* m_questionTextEdit;
    QVBoxLayout* m_optionsLayout;

    // A list to keep all our little option widgets together!
    QList<QWidget*> m_optionWidgets;

    // The magical purse to hold our question data!
    QJsonObject m_currentQuestion;
};

#endif // MCQMULTIPLEEDITOR_H
