#pragma once

#include "calculator.h"

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum class Operation {
    NO_OPERATION,
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    POWER
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void OnDigitClicked();
    void OnDotClicked();
    void OnPlusMinusClicked();
    void OnBackspaceClicked();
    void OnOperationClicked();
    void OnEqualsClicked();
    void OnClearClicked();
    void OnMemorySaveClicked();
    void OnMemoryClearClicked();
    void OnMemoryRecallClicked();

private:
    void SetText(const QString& text);
    void AddText(const QString& suffix);
    void SetOperation(Operation op);
    QString OpToString(Operation op) const;
    QString RemoveTrailingZeroes(const QString &text) const;
    QString NormalizeNumber(const QString &text) const;

    Ui::MainWindow* ui;
    Calculator calculator_;
    QString input_number_;
    Number active_number_;
    Operation current_operation_;
    double memory_value_;
    bool has_memory_;
};
