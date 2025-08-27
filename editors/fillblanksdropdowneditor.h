/*
* File: fillblanksdropdowneditor.h
* Author: Emily
*
* Description:
* The COMPLETE header file for our fill-blanks dropdown editor!
* Students choose from adorable dropdown menus! ⬇️💖
*/

#ifndef FILLBLANKSDROPDOWNEDITOR_H
#define FILLBLANKSDROPDOWNEDITOR_H

#include "../basequestioneditor.h"
#include <QtWidgets>
#include <QJsonObject>
#include <QJsonArray>

class FillBlanksDropdownEditor : public BaseQuestionEditor
{
    Q_OBJECT

public:
    explicit FillBlanksDropdownEditor(QWidget *parent = nullptr);
    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private slots:
    void addSentencePart();
    void addBlank();
    void browseMedia();

private:
    void refreshPartsUI();
    void refreshBlanksUI();
    void clearParts();
    void clearBlanks();

    // UI Elements
    QTextEdit* m_questionTextEdit;
    QVBoxLayout* m_partsLayout;
    QVBoxLayout* m_blanksLayout;
    QLineEdit* m_mediaEdit;
    QComboBox* m_mediaTypeCombo;

    // Data storage
    QJsonObject m_currentQuestion;
    QList<QWidget*> m_partsWidgets;
    QList<QWidget*> m_blanksWidgets;
};

#endif // FILLBLANKSDROPDOWNEDITOR_H
