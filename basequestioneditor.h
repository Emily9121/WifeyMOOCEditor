// =======================================================================
// basequestioneditor.h
// =======================================================================
#ifndef BASEQUESTIONEDITOR_H
#define BASEQUESTIONEDITOR_H

#include <QWidget>
#include <QJsonObject>

class BaseQuestionEditor : public QWidget
{
    Q_OBJECT
public:
    explicit BaseQuestionEditor(QWidget* parent = nullptr) : QWidget(parent) {}
    virtual void loadQuestion(const QJsonObject& question) = 0;
    virtual QJsonObject getSavedQuestion() = 0;

protected:
    QJsonObject m_currentQuestion;
};

#endif // BASEQUESTIONEDITOR_H
