/*
* File: multiquestionseditor.h  
* Author: Emily
*
* Description:
* The COMPLETE header file for our multi-questions editor!
* This creates a container that holds multiple nested questions
* with their own individual editors! Super advanced! 🌟💖
*/

#ifndef MULTIQUESTIONSEDITOR_H
#define MULTIQUESTIONSEDITOR_H

#include "../basequestioneditor.h"
#include <QtWidgets>
#include <QJsonObject>
#include <QJsonArray>

// Forward declarations
class MCQSingleEditor;
class McqMultipleEditor;
class WordFillEditor;
class ListPickEditor;
class SequenceAudioEditor;
class MatchSentenceEditor;
class OrderPhraseEditor;
class CategorizationEditor;
class FillBlanksDropdownEditor;
class MatchPhrasesEditor;
class ImageTaggingEditor;

class MultiQuestionsEditor : public BaseQuestionEditor
{
    Q_OBJECT

public:
    explicit MultiQuestionsEditor(QWidget *parent = nullptr);
    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private slots:
    void addNestedQuestion();
    void removeNestedQuestion(int index);
    void changeQuestionType(int index, const QString& newType);

private:
    void refreshQuestionsUI();
    void clearQuestions();
    BaseQuestionEditor* createEditorForType(const QString& type, QWidget* parent = nullptr);

    // UI Elements
    QTextEdit* m_questionTextEdit;
    QVBoxLayout* m_questionsLayout;
    QScrollArea* m_questionsScrollArea;

    // Data storage
    QJsonObject m_currentQuestion;

    struct NestedQuestionWidget {
        QWidget* container;
        QComboBox* typeSelector;
        BaseQuestionEditor* editor;
        QPushButton* removeButton;
        int index;
    };

    QList<NestedQuestionWidget> m_nestedWidgets;
};

#endif // MULTIQUESTIONSEDITOR_H
