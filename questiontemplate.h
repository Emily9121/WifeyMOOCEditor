#ifndef QUESTIONTEMPLATE_H
#define QUESTIONTEMPLATE_H

#include <QJsonObject>
#include <QJsonArray>
#include "questiontypes.h"

class QuestionTemplate
{
public:
    static QJsonObject createTemplate(QuestionType type);
    static QJsonObject createListPickTemplate();
    static QJsonObject createMcqSingleTemplate();
    static QJsonObject createMcqMultipleTemplate();
    static QJsonObject createWordFillTemplate();
    static QJsonObject createMatchSentenceTemplate();
    static QJsonObject createSequenceAudioTemplate();
    static QJsonObject createOrderPhraseTemplate();
    static QJsonObject createCategorizationMultipleTemplate();
    static QJsonObject createFillBlanksDropdownTemplate();
    static QJsonObject createMatchPhrasesTemplate();

private:
    QuestionTemplate() = default; // Static class
};

#endif // QUESTIONTEMPLATE_H
