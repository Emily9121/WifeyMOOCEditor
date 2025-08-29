/*
* File: imagetaggingeditor.h
* Author: Emily
*
* Description:
* The COMPLETE header file for our super-artistic image tagging editor!
* Now with FULL alternatives support and proper coordinate handling,
* just like our amazing Python version! 🏷️💖
*/

#ifndef IMAGETAGGINGEDITOR_H
#define IMAGETAGGINGEDITOR_H

#include "../basequestioneditor.h"
#include <QtWidgets>
#include <QJsonObject>
#include <QJsonArray>

class ImageTaggingEditor : public BaseQuestionEditor
{
    Q_OBJECT

public:
    explicit ImageTaggingEditor(QWidget *parent = nullptr);
    void loadJson(const QJsonObject &json) override;
    QJsonObject getJson() override;

private slots:
    void addTag();
    void addAlternative();
    void browseMainImage();
    void browseAlternativeImage(QLineEdit* imageEdit);
    // 💖 Our new slot to browse for optional media! 💖
    void browseMedia();

private:
    void refreshUI();
    void refreshMainTagsUI();
    void refreshAlternativesUI();
    void clearMainTags();
    void clearAlternatives();

    // UI Elements for main image
    QTextEdit* m_questionTextEdit;
    QLineEdit* m_mainImageEdit;
    QLineEdit* m_buttonLabelEdit;
    QVBoxLayout* m_mainTagsLayout;
    QVBoxLayout* m_alternativesLayout;
    
    // 💖 Our new UI elements for optional media! 💖
    QLineEdit* m_mediaEdit;
    QComboBox* m_mediaTypeCombo;

    // 💖 ADDED: UI elements for our lesson PDF! So cute! 💖
    QLineEdit* m_lessonPdfEdit;
    QPushButton* m_lessonPdfButton;

    // Data storage
    QJsonObject m_currentQuestion;

    // Widgets tracking
    QList<QWidget*> m_mainTagWidgets;

    // Structure for alternative widgets
    struct AlternativeWidgets {
        QWidget* container;
        QLineEdit* imageEdit;
        QLineEdit* buttonLabelEdit;
        QVBoxLayout* tagsLayout;
        QList<QWidget*> tagWidgets;
    };
    QList<AlternativeWidgets> m_alternativeWidgets;
};

#endif // IMAGETAGGINGEDITOR_H
