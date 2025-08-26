/*
 * File: mcqsingleeditor.h
 * Author: Emily
 *
 * Description:
 * Header file for the MCQ Single Answer question editor.
 * Defines the interface for this cute editor widget.
 */

#ifndef MCQSINGLEEDITOR_H
#define MCQSINGLEEDITOR_H

#include "../basequestioneditor.h"
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QList>
#include <QWidget>

class MCQSingleEditor : public BaseQuestionEditor
{
    Q_OBJECT

public:
    explicit MCQSingleEditor(QWidget *parent = nullptr);

    // These functions are our contract with the base class!
    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private slots:
    // Slot to add a new option widget
    void addOption(bool correct = false, const QString &text = "", const QString &feedback = "");

private:
    void clearOptions();

    // Main UI elements for this editor
    QTextEdit *questionPromptEdit;
    QVBoxLayout *optionsLayout; // The layout holding all the option widgets

    // A list to keep track of our dynamically created option widgets
    QList<QWidget*> optionWidgets;
};

#endif // MCQSINGLEEDITOR_H
