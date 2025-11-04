#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QLabel>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      active_number_(0.0), current_operation_(Operation::NO_OPERATION),
      memory_value_(0.0), has_memory_(false) {
    ui->setupUi(this);
    
    ui->l_result->setText("0");
    ui->l_memory->setText("");
    ui->l_formula->setText("");
    
    SetText("0");
    
    QPushButton* digitButtons[] = {
        ui->btn0, ui->btn1, ui->btn2, ui->btn3, ui->btn4,
        ui->btn5, ui->btn6, ui->btn7, ui->btn8, ui->btn9
    };
    for (int i = 0; i < 10; ++i) {
        connect(digitButtons[i], &QPushButton::clicked, this, &MainWindow::OnDigitClicked);
    }
    
    connect(ui->btnDot, &QPushButton::clicked, this, &MainWindow::OnDotClicked);
    connect(ui->btnPlusMinus, &QPushButton::clicked, this, &MainWindow::OnPlusMinusClicked);
    connect(ui->btnBackspace, &QPushButton::clicked, this, &MainWindow::OnBackspaceClicked);
    
    connect(ui->btnAdd, &QPushButton::clicked, this, &MainWindow::OnOperationClicked);
    connect(ui->btnSub, &QPushButton::clicked, this, &MainWindow::OnOperationClicked);
    connect(ui->btnMul, &QPushButton::clicked, this, &MainWindow::OnOperationClicked);
    connect(ui->btnDiv, &QPushButton::clicked, this, &MainWindow::OnOperationClicked);
    connect(ui->btnPower, &QPushButton::clicked, this, &MainWindow::OnOperationClicked);
    
    connect(ui->btnEquals, &QPushButton::clicked, this, &MainWindow::OnEqualsClicked);
    connect(ui->btnC, &QPushButton::clicked, this, &MainWindow::OnClearClicked);
    
    connect(ui->btnMS, &QPushButton::clicked, this, &MainWindow::OnMemorySaveClicked);
    connect(ui->btnMC, &QPushButton::clicked, this, &MainWindow::OnMemoryClearClicked);
    connect(ui->btnMR, &QPushButton::clicked, this, &MainWindow::OnMemoryRecallClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

QString MainWindow::RemoveTrailingZeroes(const QString &text) const {
    for (qsizetype i = 0; i < text.size(); ++i) {
        if (text[i] != '0') {
            return text.mid(i);
        }
    }
    return "";
}

QString MainWindow::NormalizeNumber(const QString &text) const {
    if (text.isEmpty()) {
        return "0";
    }
    if (text.startsWith('.')) {
        return NormalizeNumber("0" + text);
    }
    if (text.startsWith('-')) {
        return "-" + NormalizeNumber(text.mid(1));
    }
    if (text.startsWith('0') && !text.startsWith("0.")) {
        return NormalizeNumber(RemoveTrailingZeroes(text));
    }
    return text;
}

void MainWindow::SetText(const QString& text) {
    QString normalized = NormalizeNumber(text);
    input_number_ = normalized;
    active_number_ = input_number_.toDouble();
    ui->l_result->setText(input_number_);
}

void MainWindow::AddText(const QString& suffix) {
    input_number_ += suffix;
    SetText(input_number_);
}

void MainWindow::OnDigitClicked() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString digit = button->text();
        AddText(digit);
    }
}

void MainWindow::OnDotClicked() {
    if (!input_number_.contains('.')) {
        AddText(".");
    }
}

void MainWindow::OnPlusMinusClicked() {
    if (input_number_.startsWith("-")) {
        QString newNumber = input_number_.mid(1);
        SetText(newNumber);
    } else {
        SetText("-" + input_number_);
    }
}

void MainWindow::OnBackspaceClicked() {
    if (!input_number_.isEmpty()) {
        input_number_.chop(1);
        SetText(input_number_);
    }
}

QString MainWindow::OpToString(Operation op) const {
    switch(op) {
    case Operation::NO_OPERATION: return "";
    case Operation::ADDITION: return "+";
    case Operation::DIVISION: return "÷";
    case Operation::MULTIPLICATION: return "×";
    case Operation::SUBTRACTION: return "−";
    case Operation::POWER: return "^";
    }
    return "";
}

void MainWindow::SetOperation(Operation op) {
    if (current_operation_ != Operation::NO_OPERATION) {
        if (input_number_.isEmpty()) {
            active_number_ = calculator_.GetNumber();
        }
        
        switch (current_operation_) {
        case Operation::ADDITION:
            calculator_.Add(active_number_);
            break;
        case Operation::SUBTRACTION:
            calculator_.Sub(active_number_);
            break;
        case Operation::MULTIPLICATION:
            calculator_.Mul(active_number_);
            break;
        case Operation::DIVISION:
            calculator_.Div(active_number_);
            break;
        case Operation::POWER:
            calculator_.Pow(active_number_);
            break;
        case Operation::NO_OPERATION:
            break;
        }
        active_number_ = calculator_.GetNumber();
        ui->l_result->setText(QString::number(active_number_));
    } else {
        calculator_.Set(active_number_);
    }
    
    current_operation_ = op;
    
    QString opStr = OpToString(op);
    QString formulaText = QString::number(calculator_.GetNumber()) + " " + opStr;
    ui->l_formula->setText(formulaText);
    
    input_number_ = "";
}

void MainWindow::OnOperationClicked() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    QString buttonText = button->text();
    
    if (buttonText == "+") {
        SetOperation(Operation::ADDITION);
    } else if (buttonText == "−") {
        SetOperation(Operation::SUBTRACTION);
    } else if (buttonText == "×") {
        SetOperation(Operation::MULTIPLICATION);
    } else if (buttonText == "÷") {
        SetOperation(Operation::DIVISION);
    } else if (buttonText == "xʸ") {
        SetOperation(Operation::POWER);
    }
}

void MainWindow::OnEqualsClicked() {
    if (current_operation_ == Operation::NO_OPERATION) {
        return;
    }
    
    if (input_number_.isEmpty()) {
        active_number_ = calculator_.GetNumber();
    }
    
    QString opStr = OpToString(current_operation_);
    QString formulaText = QString::number(calculator_.GetNumber()) + " " + opStr + " " + QString::number(active_number_) + " =";
    ui->l_formula->setText(formulaText);
    
    switch (current_operation_) {
    case Operation::ADDITION:
        calculator_.Add(active_number_);
        break;
    case Operation::SUBTRACTION:
        calculator_.Sub(active_number_);
        break;
    case Operation::MULTIPLICATION:
        calculator_.Mul(active_number_);
        break;
    case Operation::DIVISION:
        calculator_.Div(active_number_);
        break;
    case Operation::POWER:
        calculator_.Pow(active_number_);
        break;
    case Operation::NO_OPERATION:
        break;
    }
    
    active_number_ = calculator_.GetNumber();
    ui->l_result->setText(QString::number(active_number_));
    input_number_ = "";
    current_operation_ = Operation::NO_OPERATION;
}

void MainWindow::OnClearClicked() {
    current_operation_ = Operation::NO_OPERATION;
    ui->l_formula->setText("");
    SetText("0");
}

void MainWindow::OnMemorySaveClicked() {
    memory_value_ = active_number_;
    has_memory_ = true;
    ui->l_memory->setText("M");
}

void MainWindow::OnMemoryClearClicked() {
    has_memory_ = false;
    ui->l_memory->setText("");
}

void MainWindow::OnMemoryRecallClicked() {
    if (has_memory_) {
        active_number_ = memory_value_;
        ui->l_result->setText(QString::number(active_number_));
        input_number_ = "";
    }
}
