/*
* File: basequestioneditor.h
* Author: Emily
*
* Description:
* A lovely abstract base class for all our question editors.
* This ensures they all know how to load and save JSON. So organized! 💖
*/

#ifndef BASEQUESTIONEDITOR_H
#define BASEQUESTIONEDITOR_H

#include <QWidget>
#include <QJsonObject>

class BaseQuestionEditor : public QWidget
{
    Q_OBJECT

public:
    explicit BaseQuestionEditor(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~BaseQuestionEditor() = default;

    // Every editor MUST have these functions. It's our promise!
    virtual void loadJson(const QJsonObject &json) = 0;
    virtual QJsonObject getJson() = 0;
};

#endif // BASEQUESTIONEDITOR_H