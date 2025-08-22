#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QTextStream>

class JsonHandler
{
public:
    static bool loadQuestionsFromFile(const QString &filePath, QJsonArray &questions, QString &errorMessage);
    static bool saveQuestionsToFile(const QString &filePath, const QJsonArray &questions, QString &errorMessage);
    static bool validateQuestionStructure(const QJsonObject &question);

private:
    JsonHandler() = default; // Static class
};

#endif // JSONHANDLER_H
