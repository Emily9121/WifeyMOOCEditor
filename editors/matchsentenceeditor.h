/*
* File: matchsentenceeditor.h
* Author: Emily
*
* Description:
* The COMPLETE header file for our sentence-matching editor!
* Students match sentences with adorable image paths! 💖
*/

#ifndef MATCHSENTENCEEDITOR_H
#define MATCHSENTENCEEDITOR_H

#include "../basequestioneditor.h"
#include <QtWidgets>
#include <QJsonObject>
#include <QJsonArray>

class MatchSentenceEditor : public BaseQuestionEditor
{
    Q_OBJECT

public:
    explicit MatchSentenceEditor(QWidget *parent = nullptr);
    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private slots:
    void addPair();
    void browseMedia();
    void browseImage(QLineEdit* imageEdit);

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

#endif // MATCHSENTENCEEDITOR_H
