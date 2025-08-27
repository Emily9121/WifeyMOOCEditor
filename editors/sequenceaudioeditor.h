/*
* File: sequenceaudioeditor.h
* Author: Emily
*
* Description:
* The COMPLETE header file for our audio-sequencing editor!
* Students put sweet sounds in the perfect order! 🎵💖
*/

#ifndef SEQUENCEAUDIOEDITOR_H
#define SEQUENCEAUDIOEDITOR_H

#include "../basequestioneditor.h"
#include <QtWidgets>
#include <QJsonObject>
#include <QJsonArray>

class SequenceAudioEditor : public BaseQuestionEditor
{
    Q_OBJECT

public:
    explicit SequenceAudioEditor(QWidget *parent = nullptr);
    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private slots:
    void addAudioOption();
    void browseMainAudio();
    void moveUp(int index);
    void moveDown(int index);

private:
    void refreshOptionsUI();
    void clearOptions();

    // UI Elements
    QTextEdit* m_questionTextEdit;
    QVBoxLayout* m_optionsLayout;
    QLineEdit* m_mainAudioEdit;

    // Data storage
    QJsonObject m_currentQuestion;
    QList<QWidget*> m_optionWidgets;
};

#endif // SEQUENCEAUDIOEDITOR_H
