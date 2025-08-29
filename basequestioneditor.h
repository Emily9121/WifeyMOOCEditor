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
#include <QTextEdit> 
#include <QLineEdit>
#include <QPushButton>

class BaseQuestionEditor : public QWidget
{
    Q_OBJECT

public:
    explicit BaseQuestionEditor(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~BaseQuestionEditor() = default;

    virtual void loadJson(const QJsonObject &json) = 0;
    virtual QJsonObject getJson() = 0;
    
    // ✨ CORRECTED: Added a default empty implementation to solve the linker error! ✨
    // Now the compiler knows what to do even if a specific editor doesn't override it.
    virtual void setupLessonPdfEditor(const QJsonObject& json) {
        Q_UNUSED(json);
    }

protected:
    // ✨ Our hint box for all the cute editor babies! ✨
    QTextEdit* m_hintTextEdit;

    // These members are no longer needed here since each editor will manage its own UI.
    // QLineEdit* m_lessonPdfEdit;
    // QPushButton* m_lessonPdfButton;
};

#endif // BASEQUESTIONEDITOR_H
