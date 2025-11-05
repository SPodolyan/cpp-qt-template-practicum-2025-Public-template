#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->l_result->setText("0");
  ui->l_memory->setText("");
  ui->l_formula->setText("");

  connect(ui->tb_zero, &QPushButton::clicked, this, &MainWindow::OnButtonZeroClicked);
  connect(ui->tb_one, &QPushButton::clicked, this, &MainWindow::OnButtonOneClicked);
  connect(ui->tb_two, &QPushButton::clicked, this, &MainWindow::OnButtonTwoClicked);
  connect(ui->tb_three, &QPushButton::clicked, this, &MainWindow::OnButtonThreeClicked);
  connect(ui->tb_four, &QPushButton::clicked, this, &MainWindow::OnButtonFourClicked);
  connect(ui->tb_five, &QPushButton::clicked, this, &MainWindow::OnButtonFiveClicked);
  connect(ui->tb_six, &QPushButton::clicked, this, &MainWindow::OnButtonSixClicked);
  connect(ui->tb_seven, &QPushButton::clicked, this, &MainWindow::OnButtonSevenClicked);
  connect(ui->tb_eight, &QPushButton::clicked, this, &MainWindow::OnButtonEightClicked);
  connect(ui->tb_nine, &QPushButton::clicked, this, &MainWindow::OnButtonNineClicked);

  connect(ui->tb_comma, &QPushButton::clicked, this, &MainWindow::OnButtonCommaClicked);
  connect(ui->tb_negate, &QPushButton::clicked, this, &MainWindow::OnButtonNegateClicked);
  connect(ui->tb_backspace, &QPushButton::clicked, this, &MainWindow::OnButtonBackspaceClicked);

  connect(ui->tb_power, &QPushButton::clicked, this, &MainWindow::OnButtonPowerClicked);
  connect(ui->tb_add, &QPushButton::clicked, this, &MainWindow::OnButtonAddClicked);
  connect(ui->tb_substract, &QPushButton::clicked, this, &MainWindow::OnButtonSubstractClicked);
  connect(ui->tb_multiplicate, &QPushButton::clicked, this, &MainWindow::OnButtonMultiplicateClicked);
  connect(ui->tb_divide, &QPushButton::clicked, this, &MainWindow::OnButtonDivideClicked);

  connect(ui->tb_equal, &QPushButton::clicked, this, &MainWindow::OnButtonEqualClicked);

  connect(ui->tb_reset, &QPushButton::clicked, this, &MainWindow::OnButtonResetClicked);
  connect(ui->tb_mc, &QPushButton::clicked, this, &MainWindow::OnButtonMemoryCrearClicked);
  connect(ui->tn_mr, &QPushButton::clicked, this, &MainWindow::OnButtonMemoryRetrieveClicked);
  connect(ui->tb_ms, &QPushButton::clicked, this, &MainWindow::OnButtonMemorySaveClicked);

  SetText("0");
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::OnButtonZeroClicked() {
  AddText("0");
}

void MainWindow::OnButtonOneClicked() {
  AddText("1");
}

void MainWindow::OnButtonTwoClicked() {
  AddText("2");
}

void MainWindow::OnButtonThreeClicked() {
  AddText("3");
}

void MainWindow::OnButtonFourClicked() {
  AddText("4");
}

void MainWindow::OnButtonFiveClicked() {
  AddText("5");
}

void MainWindow::OnButtonSixClicked() {
  AddText("6");
}

void MainWindow::OnButtonSevenClicked() {
  AddText("7");
}

void MainWindow::OnButtonEightClicked() {
  AddText("8");
}

void MainWindow::OnButtonNineClicked() {
  AddText("9");
}

void MainWindow::OnButtonCommaClicked() {
  if (!input_number_.contains(".")) {
    AddText(".");
  }
}

void MainWindow::OnButtonNegateClicked() {
  if (input_number_.startsWith("-")) {
    SetText(input_number_.mid(1));
  }
  else {
    input_number_ = "-" + input_number_;
    SetText(input_number_);
  }
}

void MainWindow::OnButtonBackspaceClicked() {
  if (input_number_.size() != 0) {
    input_number_.chop(1);
    SetText(input_number_);
  }
}

void MainWindow::OnButtonPowerClicked() {
  SetOperation(Operation::POWER);
}

void MainWindow::OnButtonAddClicked() {
  SetOperation(Operation::ADDITION);
}

void MainWindow::OnButtonSubstractClicked() {
  SetOperation(Operation::SUBTRACTION);
}

void MainWindow::OnButtonMultiplicateClicked() {
  SetOperation(Operation::MULTIPLICATION);
}

void MainWindow::OnButtonDivideClicked() {
  SetOperation(Operation::DIVISION);
}

void MainWindow::OnButtonEqualClicked() {
  if (current_operation_ == Operation::NO_OPERATION) {
    return;
  }
  QString formula = QString("%1 %2 %3 =").arg(calculator_.GetNumber()).arg(OpToString(current_operation_)).arg(active_number_);
  ui->l_formula->setText(formula);

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
  }
  active_number_ = calculator_.GetNumber();
  ui->l_result->setText(QString::number(active_number_));
  input_number_.clear();
  current_operation_ = Operation::NO_OPERATION;
}

void MainWindow::OnButtonResetClicked() {
  current_operation_ = Operation::NO_OPERATION;
  ui->l_formula->setText("");
  SetText("0");
}

void MainWindow::OnButtonMemoryCrearClicked() {
  ui->l_memory->setText("");
  is_numer_in_memory_ = false;
}

void MainWindow::OnButtonMemoryRetrieveClicked() {
  if (!is_numer_in_memory_) {
    return;
  }
  active_number_ = memory_;
  ui->l_result->setText(QString::number(active_number_));
  input_number_.clear();
}

void MainWindow::OnButtonMemorySaveClicked() {
  memory_ = active_number_;
  ui->l_memory->setText("M");
  is_numer_in_memory_ = true;
}

void MainWindow::SetText(const QString &text) {
  input_number_ = NormalizeNumber(text);
  active_number_ = input_number_.toDouble();
  ui->l_result->setText(input_number_);
}

void MainWindow::AddText(const QString &suffix) {
  input_number_ += suffix;
  SetText(input_number_);
}

void MainWindow::SetOperation(Operation operation) {
  if (current_operation_ == Operation::NO_OPERATION) {
    calculator_.Set(active_number_);
  }
  current_operation_ = operation;
  QString formula = QString("%1 %2").arg(calculator_.GetNumber()).arg(OpToString(current_operation_));
  ui->l_formula->setText(formula);
  input_number_.clear();
}

QString MainWindow::OpToString(Operation op) const   {
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

QString MainWindow::RemoveTrailingZeroes(const QString &text) {
  for (qsizetype i = 0; i < text.size(); ++i) {
    if (text[i] != '0') {
      return text.mid(i);
    }
  }
  return "";
}

QString MainWindow::NormalizeNumber(const QString &text) {
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
