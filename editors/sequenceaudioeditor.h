/*
 * File: sequenceaudioeditor.h
 * Author: Emily
 *
 * Description:
 * The header file for our super-cool audio-sequencing editor!
 * I've given it a sound-proof purse (a QJsonObject member variable)
 * to remember all the audio files for Sierra's quizzes! <3
 */

#ifndef SEQUENCEAUDIOEDITOR_H
#define SEQUENCEAUDIOEDITOR_H

#include "../basequestioneditor.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QJsonObject> // The header for our purse!

class SequenceAudioEditor : public BaseQuestionEditor
{
    Q_OBJECT
public:
    explicit SequenceAudioEditor(QWidget *parent = nullptr);

    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private:
    // Helper functions to keep the UI in tune!
    void refreshOptionsUI();
    void addOption();

    // UI Elements
    QTextEdit* m_questionTextEdit;
    QVBoxLayout* m_optionsLayout;

    // The brand new purse for our editor!
    QJsonObject m_currentQuestion;
};

#endif // SEQUENCEAUDIOEDITOR_H
