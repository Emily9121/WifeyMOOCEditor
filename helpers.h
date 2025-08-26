// =======================================================================
// helpers.h
// =======================================================================
#ifndef HELPERS_H
#define HELPERS_H

#include <QWidget>
#include <QLineEdit>
#include <QFileDialog>

static void browseForFile(QWidget* parent, QLineEdit* targetLineEdit) {
    QString filePath = QFileDialog::getOpenFileName(parent, "💖 Select Your Cute Media File 💖", "", "All Files (*)");
    if (!filePath.isEmpty()) {
        targetLineEdit->setText(filePath);
    }
}

#endif // HELPERS_H