/*
* File: orderphraseeditor.h
* Author: Emily
*
* Description:
* The COMPLETE header file for our phrase-ordering editor!
* Students put adorable phrases in the perfect order! 📋💖
*/

#ifndef ORDERPHRASEEDITOR_H
#define ORDERPHRASEEDITOR_H

#include "../basequestioneditor.h"
#include <QtWidgets>
#include <QJsonObject>
#include <QJsonArray>

class OrderPhraseEditor : public BaseQuestionEditor
{
    Q_OBJECT

public:
    explicit OrderPhraseEditor(QWidget *parent = nullptr);
    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private slots:
    void addPhrase();
    void browseMedia();
    void moveUp(int index);
    void moveDown(int index);

private:
    void refreshPhrasesUI();
    void clearPhrases();

    // UI Elements
    QTextEdit* m_questionTextEdit;
    QVBoxLayout* m_phrasesLayout;
    QLineEdit* m_mediaEdit;
    QComboBox* m_mediaTypeCombo;

    // 💖 ADDED: UI elements for our lesson PDF! So cute! 💖
    QLineEdit* m_lessonPdfEdit;
    QPushButton* m_lessonPdfButton;

    // Data storage
    QJsonObject m_currentQuestion;
    QList<QWidget*> m_phraseWidgets;
};

#endif // ORDERPHRASEEDITOR_H
