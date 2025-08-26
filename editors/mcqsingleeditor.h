/*
 * File: mcqsingleeditor.h
 * Author: Emily
 *
 * Description:
 * The header file for our very first editor! I've taught it how to
 * listen to button clicks properly and made it super smart! This is
 * the final fix, I promise! <3
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

    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private slots:
    // This is our new, simple slot for the button click!
    void addOption();

private:
    // This is our helper function to create the actual widgets!
    void createOptionRow(bool correct = false, const QString &text = "", const QString &feedback = "");
    void clearOptions();

    // Main UI elements for this editor
    QTextEdit *questionPromptEdit;
    QVBoxLayout *optionsLayout;

    // A list to keep track of our dynamically created option widgets
    QList<QWidget*> optionWidgets;
};

#endif // MCQSINGLEEDITOR_H
