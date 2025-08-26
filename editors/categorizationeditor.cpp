// =======================================================================
// editors/categorizationeditor.cpp
// =======================================================================
#include "categorizationeditor.h"
#include "../helpers.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QJsonArray>
#include <QMessageBox>
#include <QFileInfo>

CategorizationEditor::CategorizationEditor(QWidget* parent) : BaseQuestionEditor(parent) {
    auto mainLayout = new QVBoxLayout(this); setLayout(mainLayout);
    auto questionGroup = new QGroupBox("📝 Question Text 📝");
    auto questionLayout = new QVBoxLayout(questionGroup);
    m_questionTextEdit = new QTextEdit(); m_questionTextEdit->setFixedHeight(80);
    questionLayout->addWidget(m_questionTextEdit); mainLayout->addWidget(questionGroup);
    m_categoriesGroup = new QGroupBox("📂 Categories 📂");
    m_categoriesLayout = new QVBoxLayout(m_categoriesGroup); mainLayout->addWidget(m_categoriesGroup);
    m_stimuliGroup = new QGroupBox("🎯 Items to Categorize 🎯");
    m_stimuliLayout = new QVBoxLayout(m_stimuliGroup); mainLayout->addWidget(m_stimuliGroup);
    auto buttonLayout = new QHBoxLayout();
    auto addCategoryButton = new QPushButton("➕ Add Category"); auto addStimulusButton = new QPushButton("➕ Add Item");
    auto saveButton = new QPushButton("💾 Save Question");
    buttonLayout->addWidget(addCategoryButton); buttonLayout->addWidget(addStimulusButton); buttonLayout->addWidget(saveButton);
    mainLayout->addLayout(buttonLayout); mainLayout->addStretch();
    connect(addCategoryButton, &QPushButton::clicked, this, &CategorizationEditor::addCategory);
    connect(addStimulusButton, &QPushButton::clicked, this, &CategorizationEditor::addStimulus);
    connect(saveButton, &QPushButton::clicked, [this](){ getSavedQuestion(); QMessageBox::information(this, "Success! 💖", "Question saved beautifully!"); });
}
void CategorizationEditor::loadQuestion(const QJsonObject& question) { m_currentQuestion = question; m_questionTextEdit->setText(question["question"].toString()); refreshCategoriesUI(); refreshStimuliUI(); }
QJsonObject CategorizationEditor::getSavedQuestion() {
    m_currentQuestion["question"] = m_questionTextEdit->toPlainText();
    QStringList categoriesList; for(auto* edit : m_categoryEdits) { categoriesList.append(edit->text()); }
    m_currentQuestion["categories"] = QJsonArray::fromStringList(categoriesList);
    QJsonArray stimuliArray; QJsonObject answerObject;
    for(const auto& stimWidgets : m_stimulusWidgets) {
        QString text = stimWidgets.textEdit->text(); QString image = stimWidgets.imageEdit->text(); QString category = stimWidgets.categoryCombo->currentText();
        QJsonObject stimObj; stimObj["text"] = text; stimObj["image"] = image; stimuliArray.append(stimObj);
        QString answerKey = !text.isEmpty() ? text : QFileInfo(image).fileName();
        if (!answerKey.isEmpty()) { answerObject[answerKey] = category; }
    }
    m_currentQuestion["stimuli"] = stimuliArray; m_currentQuestion["answer"] = answerObject;
    return m_currentQuestion;
}
void CategorizationEditor::refreshCategoriesUI() {
    qDeleteAll(m_categoryWidgets); m_categoryWidgets.clear(); m_categoryEdits.clear();
    QJsonArray categories = m_currentQuestion["categories"].toArray();
    for (int i = 0; i < categories.size(); ++i) {
        auto widget = new QWidget(); auto layout = new QHBoxLayout(widget);
        auto lineEdit = new QLineEdit(categories[i].toString()); auto deleteButton = new QPushButton("🗑️");
        layout->addWidget(new QLabel(QString("Category %1:").arg(i+1))); layout->addWidget(lineEdit); layout->addWidget(deleteButton);
        m_categoriesLayout->addWidget(widget); m_categoryWidgets.push_back(widget); m_categoryEdits.push_back(lineEdit);
        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray current = m_currentQuestion["categories"].toArray(); current.removeAt(i);
            m_currentQuestion["categories"] = current; refreshCategoriesUI(); refreshStimuliUI();
        });
    }
}
void CategorizationEditor::refreshStimuliUI() {
    for(auto& widgets : m_stimulusWidgets) { delete widgets.container; } m_stimulusWidgets.clear();
    QStringList categoryList; for(const auto& catVal : m_currentQuestion["categories"].toArray()) { categoryList.append(catVal.toString()); }
    QJsonObject answer = m_currentQuestion["answer"].toObject(); QJsonArray stimuli = m_currentQuestion["stimuli"].toArray();
    for (int i = 0; i < stimuli.size(); ++i) {
        QJsonObject stimObj = stimuli[i].toObject(); QString text = stimObj["text"].toString(); QString image = stimObj["image"].toString();
        auto container = new QGroupBox(QString("💖 Item %1 💖").arg(i+1)); auto layout = new QVBoxLayout(container);
        auto textLayout = new QHBoxLayout(); textLayout->addWidget(new QLabel("📝 Text:"));
        auto textEdit = new QLineEdit(text); textLayout->addWidget(textEdit); layout->addLayout(textLayout);
        auto imageLayout = new QHBoxLayout(); imageLayout->addWidget(new QLabel("🖼️ Image:"));
        auto imageEdit = new QLineEdit(image); auto browseButton = new QPushButton("Browse...");
        imageLayout->addWidget(imageEdit); imageLayout->addWidget(browseButton); layout->addLayout(imageLayout);
        auto answerLayout = new QHBoxLayout(); answerLayout->addWidget(new QLabel("✅ Correct Category:"));
        auto categoryCombo = new QComboBox(); categoryCombo->addItems(categoryList);
        QString answerKey = !text.isEmpty() ? text : QFileInfo(image).fileName();
        categoryCombo->setCurrentText(answer[answerKey].toString());
        answerLayout->addWidget(categoryCombo); layout->addLayout(answerLayout);
        auto deleteButton = new QPushButton("🗑️ Delete Item"); layout->addWidget(deleteButton, 0, Qt::AlignRight);
        m_stimuliLayout->addWidget(container); m_stimulusWidgets.push_back({container, textEdit, imageEdit, categoryCombo});
        connect(browseButton, &QPushButton::clicked, [this, imageEdit](){ browseForFile(this, imageEdit); });
        connect(deleteButton, &QPushButton::clicked, [this, i](){
            QJsonArray current = m_currentQuestion["stimuli"].toArray(); current.removeAt(i);
            m_currentQuestion["stimuli"] = current; refreshStimuliUI();
        });
    }
}
void CategorizationEditor::addCategory() { QJsonArray current = m_currentQuestion["categories"].toArray(); current.append("New Category"); m_currentQuestion["categories"] = current; refreshCategoriesUI(); refreshStimuliUI(); }
void CategorizationEditor::addStimulus() { QJsonArray current = m_currentQuestion["stimuli"].toArray(); current.append(QJsonObject{{"text", "New Item"}, {"image", ""}}); m_currentQuestion["stimuli"] = current; refreshStimuliUI(); }

