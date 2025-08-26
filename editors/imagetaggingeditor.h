/*
 * File: imagetaggingeditor.h
 * Author: Emily
 *
 * Description:
 * The header file for our super-artistic image tagging editor!
 * I've given it a special purse (a QJsonObject member variable)
 * to hold onto all the tags and image info for Sierra! <3
 */

#ifndef IMAGETAGGINGEDITOR_H
#define IMAGETAGGINGEDITOR_H

#include "../basequestioneditor.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMap>
#include <QLineEdit>
#include <QJsonObject> // The header for our cute purse!

class ImageTaggingEditor : public BaseQuestionEditor
{
    Q_OBJECT
public:
    explicit ImageTaggingEditor(QWidget *parent = nullptr);

    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private:
    void refreshUI();
    void refreshMainTagsUI();
    void refreshAlternativesUI();
    void addTag();
    void addAlternative();

    // UI Elements
    QTextEdit* m_questionTextEdit;
    QVBoxLayout* m_mainTagsLayout;
    QVBoxLayout* m_alternativesLayout;

    // A special struct to keep alternative widgets organized!
    struct AltWidgets {
        QWidget* container;
        QLineEdit* imageEdit;
        QLineEdit* buttonLabelEdit;
        QMap<QString, QWidget*> tagCoordWidgets;
        QMap<QString, QLineEdit*> xEdits;
        QMap<QString, QLineEdit*> yEdits;
    };
    QList<AltWidgets> m_altWidgets;

    // The magical purse to hold our question data!
    QJsonObject m_currentQuestion;
};

#endif // IMAGETAGGINGEDITOR_H
