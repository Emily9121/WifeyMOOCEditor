/*
* File: matchphraseseditor.h
* Author: Emily
*
* Description:
* The COMPLETE header file for our phrase-matching editor!
* Students match phrase beginnings with their perfect endings! 🔗💖
*/

#ifndef MATCHPHRASESEDITOR_H
#define MATCHPHRASESEDITOR_H

#include "../basequestioneditor.h"
#include <QtWidgets>
#include <QJsonObject>
#include <QJsonArray>

class MatchPhrasesEditor : public BaseQuestionEditor
{
    Q_OBJECT

public:
    explicit MatchPhrasesEditor(QWidget *parent = nullptr);
    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private slots:
    void addPair();
    void browseMedia();

private:
    void refreshPairsUI();
    void clearPairs();

    // UI Elements
    QTextEdit* m_questionTextEdit;
    QVBoxLayout* m_pairsLayout;
    QLineEdit* m_mediaEdit;
    QComboBox* m_mediaTypeCombo;

    // 💖 ADDED: UI elements for our lesson PDF! So cute! 💖
    QLineEdit* m_lessonPdfEdit;
    QPushButton* m_lessonPdfButton;

    // Data storage
    QJsonObject m_currentQuestion;
    QList<QWidget*> m_pairWidgets;
};

#endif // MATCHPHRASESEDITOR_H
