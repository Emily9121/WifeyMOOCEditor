#include "questiontemplate.h"

QJsonObject QuestionTemplate::createTemplate(QuestionType type)
{
    switch (type) {
        case QuestionType::ListPick:
            return createListPickTemplate();
        case QuestionType::McqSingle:
            return createMcqSingleTemplate();
        case QuestionType::McqMultiple:
            return createMcqMultipleTemplate();
        case QuestionType::WordFill:
            return createWordFillTemplate();
        case QuestionType::MatchSentence:
            return createMatchSentenceTemplate();
        case QuestionType::SequenceAudio:
            return createSequenceAudioTemplate();
        case QuestionType::OrderPhrase:
            return createOrderPhraseTemplate();
        case QuestionType::CategorizationMultiple:
            return createCategorizationMultipleTemplate();
        case QuestionType::FillBlanksDropdown:
            return createFillBlanksDropdownTemplate();
        case QuestionType::MatchPhrases:
            return createMatchPhrasesTemplate();
        default:
            return QJsonObject();
    }
}

QJsonObject QuestionTemplate::createListPickTemplate()
{
    QJsonObject question;
    question["type"] = "list_pick";
    question["question"] = "Pick all the cute options you want! 💖";

    QJsonArray options;
    options.append("Option 1");
    options.append("Option 2");
    options.append("Option 3");
    question["options"] = options;

    QJsonArray answer;
    answer.append(0);
    question["answer"] = answer;

    return question;
}

QJsonObject QuestionTemplate::createMcqSingleTemplate()
{
    QJsonObject question;
    question["type"] = "mcq_single";
    question["question"] = "Choose the best answer, babe! 💕";

    QJsonArray options;
    options.append("Option A");
    options.append("Option B");
    options.append("Option C");
    question["options"] = options;

    QJsonArray answer;
    answer.append(0);
    question["answer"] = answer;

    question["media"] = QJsonValue::Null;

    return question;
}

QJsonObject QuestionTemplate::createMcqMultipleTemplate()
{
    QJsonObject question;
    question["type"] = "mcq_multiple";
    question["question"] = "Pick all the right answers, sweetie! 💖";

    QJsonArray options;
    options.append("Option A");
    options.append("Option B");
    options.append("Option C");
    question["options"] = options;

    QJsonArray answer;
    answer.append(0);
    answer.append(1);
    question["answer"] = answer;

    question["media"] = QJsonValue::Null;

    return question;
}

QJsonObject QuestionTemplate::createWordFillTemplate()
{
    QJsonObject question;
    question["type"] = "word_fill";
    question["question"] = "Fill in the cute blanks! 💕";

    QJsonArray sentenceParts;
    sentenceParts.append("Fill this ");
    sentenceParts.append(" with the perfect word ");
    sentenceParts.append(" please!");
    question["sentence_parts"] = sentenceParts;

    QJsonArray answers;
    answers.append("blank");
    answers.append("darling");
    question["answers"] = answers;

    question["media"] = QJsonValue::Null;

    return question;
}

QJsonObject QuestionTemplate::createMatchSentenceTemplate()
{
    QJsonObject question;
    question["type"] = "match_sentence";
    question["question"] = "Match the adorable sentences with images! 💖";

    QJsonArray pairs;

    QJsonObject pair1;
    pair1["sentence"] = "Cute sentence 1";
    pair1["image_path"] = "image1.jpg";
    pairs.append(pair1);

    QJsonObject pair2;
    pair2["sentence"] = "Sweet sentence 2";
    pair2["image_path"] = "image2.jpg";
    pairs.append(pair2);

    question["pairs"] = pairs;

    QJsonObject answer;
    answer["image1.jpg"] = "Cute sentence 1";
    answer["image2.jpg"] = "Sweet sentence 2";
    question["answer"] = answer;

    question["media"] = QJsonValue::Null;

    return question;
}

QJsonObject QuestionTemplate::createSequenceAudioTemplate()
{
    QJsonObject question;
    question["type"] = "sequence_audio";
    question["question"] = "Put these sweet sounds in order! 🎵";

    QJsonArray audioOptions;

    QJsonObject option1;
    option1["option"] = "First lovely sound";
    audioOptions.append(option1);

    QJsonObject option2;
    option2["option"] = "Second amazing sound";
    audioOptions.append(option2);

    question["audio_options"] = audioOptions;

    QJsonArray answer;
    answer.append(0);
    answer.append(1);
    question["answer"] = answer;

    QJsonObject media;
    media["audio"] = "audio.mp3";
    question["media"] = media;

    return question;
}

QJsonObject QuestionTemplate::createOrderPhraseTemplate()
{
    QJsonObject question;
    question["type"] = "order_phrase";
    question["question"] = "Put these phrases in the right order, honey! 💕";

    QJsonArray phraseShuffled;
    phraseShuffled.append("Second phrase");
    phraseShuffled.append("First phrase");
    phraseShuffled.append("Third phrase");
    question["phrase_shuffled"] = phraseShuffled;

    QJsonArray answer;
    answer.append("First phrase");
    answer.append("Second phrase");
    answer.append("Third phrase");
    question["answer"] = answer;

    question["media"] = QJsonValue::Null;

    return question;
}

QJsonObject QuestionTemplate::createCategorizationMultipleTemplate()
{
    QJsonObject question;
    question["type"] = "categorization_multiple";
    question["question"] = "Categorize these cute items! 📂";

    QJsonArray stimuli;

    QJsonObject stimulus1;
    stimulus1["text"] = "Adorable Item 1";
    stimulus1["image"] = QJsonValue::Null;
    stimuli.append(stimulus1);

    QJsonObject stimulus2;
    stimulus2["text"] = "Sweet Item 2";
    stimulus2["image"] = QJsonValue::Null;
    stimuli.append(stimulus2);

    question["stimuli"] = stimuli;

    QJsonArray categories;
    categories.append(" ");
    categories.append("Category A");
    categories.append("Category B");
    question["categories"] = categories;

    QJsonObject answer;
    answer["Adorable Item 1"] = "Category A";
    answer["Sweet Item 2"] = "Category B";
    question["answer"] = answer;

    question["media"] = QJsonValue::Null;

    return question;
}

QJsonObject QuestionTemplate::createFillBlanksDropdownTemplate()
{
    QJsonObject question;
    question["type"] = "fill_blanks_dropdown";
    question["question"] = "Choose from the dropdowns, sweetie! ⬇️";

    QJsonArray sentenceParts;
    sentenceParts.append("Choose ");
    sentenceParts.append(" and then ");
    sentenceParts.append(" from these cute dropdowns.");
    question["sentence_parts"] = sentenceParts;

    QJsonArray optionsForBlanks;

    QJsonArray options1;
    options1.append(" ");
    options1.append("option1");
    options1.append("option2");
    optionsForBlanks.append(options1);

    QJsonArray options2;
    options2.append(" ");
    options2.append("choice1");
    options2.append("choice2");
    optionsForBlanks.append(options2);

    question["options_for_blanks"] = optionsForBlanks;

    QJsonArray answers;
    answers.append("option1");
    answers.append("choice1");
    question["answers"] = answers;

    question["media"] = QJsonValue::Null;

    return question;
}

QJsonObject QuestionTemplate::createMatchPhrasesTemplate()
{
    QJsonObject question;
    question["type"] = "match_phrases";
    question["question"] = "Match the phrase beginnings with their perfect endings! 💖";

    QJsonArray pairs;

    QJsonObject pair;
    pair["source"] = "Beginning of cute phrase 1...";

    QJsonArray targets;
    targets.append(" ");
    targets.append("ending A");
    targets.append("ending B");
    targets.append("ending C");
    pair["targets"] = targets;

    pairs.append(pair);
    question["pairs"] = pairs;

    QJsonObject answer;
    answer["Beginning of cute phrase 1..."] = "ending A";
    question["answer"] = answer;

    question["media"] = QJsonValue::Null;

    return question;
}
