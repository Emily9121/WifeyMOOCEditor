#ifndef QUESTIONTYPES_H
#define QUESTIONTYPES_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

enum class QuestionType {
    ListPick,
    McqSingle,
    McqMultiple,
    WordFill,
    MatchSentence,
    SequenceAudio,
    OrderPhrase,
    CategorizationMultiple,
    FillBlanksDropdown,
    MatchPhrases,
    Unknown
};

inline QuestionType stringToQuestionType(const QString& typeStr) {
    if (typeStr == "list_pick") return QuestionType::ListPick;
    if (typeStr == "mcq_single") return QuestionType::McqSingle;
    if (typeStr == "mcq_multiple") return QuestionType::McqMultiple;
    if (typeStr == "word_fill") return QuestionType::WordFill;
    if (typeStr == "match_sentence") return QuestionType::MatchSentence;
    if (typeStr == "sequence_audio") return QuestionType::SequenceAudio;
    if (typeStr == "order_phrase") return QuestionType::OrderPhrase;
    if (typeStr == "categorization_multiple") return QuestionType::CategorizationMultiple;
    if (typeStr == "fill_blanks_dropdown") return QuestionType::FillBlanksDropdown;
    if (typeStr == "match_phrases") return QuestionType::MatchPhrases;
    return QuestionType::Unknown;
}

inline QString questionTypeToString(QuestionType type) {
    switch (type) {
        case QuestionType::ListPick: return "list_pick";
        case QuestionType::McqSingle: return "mcq_single";
        case QuestionType::McqMultiple: return "mcq_multiple";
        case QuestionType::WordFill: return "word_fill";
        case QuestionType::MatchSentence: return "match_sentence";
        case QuestionType::SequenceAudio: return "sequence_audio";
        case QuestionType::OrderPhrase: return "order_phrase";
        case QuestionType::CategorizationMultiple: return "categorization_multiple";
        case QuestionType::FillBlanksDropdown: return "fill_blanks_dropdown";
        case QuestionType::MatchPhrases: return "match_phrases";
        case QuestionType::Unknown: return "unknown";
    }
    return "unknown";
}

inline QString questionTypeDisplayName(QuestionType type) {
    switch (type) {
        case QuestionType::ListPick: return "📝 List Pick (Select Multiple)";
        case QuestionType::McqSingle: return "🔘 Single Choice (MCQ)";
        case QuestionType::McqMultiple: return "☑️ Multiple Choice (MCQ)";
        case QuestionType::WordFill: return "📝 Word Fill (Fill Blanks)";
        case QuestionType::MatchSentence: return "🔗 Match Sentences to Images";
        case QuestionType::SequenceAudio: return "🎵 Audio Sequence Order";
        case QuestionType::OrderPhrase: return "📋 Order Phrases Correctly";
        case QuestionType::CategorizationMultiple: return "📂 Categorization Multiple";
        case QuestionType::FillBlanksDropdown: return "⬇️ Fill Blanks with Dropdowns";
        case QuestionType::MatchPhrases: return "🔗 Match Phrase Beginnings to Endings";
        case QuestionType::Unknown: return "❓ Unknown Question Type";
    }
    return "❓ Unknown Question Type";
}

#endif // QUESTIONTYPES_H
