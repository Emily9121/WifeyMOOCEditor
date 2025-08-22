#include "jsonhandler.h"
#include <QJsonParseError>
#include <QDebug>

bool JsonHandler::loadQuestionsFromFile(const QString &filePath, QJsonArray &questions, QString &errorMessage)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        errorMessage = QString("Cannot open file: %1").arg(file.errorString());
        return false;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        errorMessage = QString("JSON parse error: %1").arg(parseError.errorString());
        return false;
    }

    if (!document.isArray()) {
        errorMessage = "JSON document must contain an array of questions";
        return false;
    }

    questions = document.array();

    // Validate each question
    for (int i = 0; i < questions.size(); ++i) {
        if (!questions[i].isObject()) {
            errorMessage = QString("Question %1 is not a valid object").arg(i + 1);
            return false;
        }

        QJsonObject question = questions[i].toObject();
        if (!validateQuestionStructure(question)) {
            qDebug() << "Warning: Question" << (i + 1) << "has invalid structure, but loading anyway";
        }
    }

    return true;
}

bool JsonHandler::saveQuestionsToFile(const QString &filePath, const QJsonArray &questions, QString &errorMessage)
{
    QJsonDocument document(questions);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        errorMessage = QString("Cannot open file for writing: %1").arg(file.errorString());
        return false;
    }

    QByteArray jsonData = document.toJson(QJsonDocument::Indented);
    qint64 bytesWritten = file.write(jsonData);
    file.close();

    if (bytesWritten == -1) {
        errorMessage = QString("Error writing to file: %1").arg(file.errorString());
        return false;
    }

    return true;
}

bool JsonHandler::validateQuestionStructure(const QJsonObject &question)
{
    // Basic validation - check for required fields
    if (!question.contains("type") || !question["type"].isString()) {
        return false;
    }

    if (!question.contains("question") || !question["question"].isString()) {
        return false;
    }

    // Type-specific validation could be added here
    QString type = question["type"].toString();

    // For now, just check that the type is one of the known types
    QStringList knownTypes = {
        "list_pick", "mcq_single", "mcq_multiple", "word_fill",
        "match_sentence", "sequence_audio", "order_phrase",
        "categorization_multiple", "fill_blanks_dropdown", "match_phrases"
    };

    return knownTypes.contains(type);
}
