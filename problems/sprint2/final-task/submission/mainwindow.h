#pragma once

#include "calculator.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
    namespace Ui {
  class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  static QString RemoveTrailingZeroes(const QString &text);
  static QString NormalizeNumber(const QString &text);

 private slots:
  void OnButtonZeroClicked();
  void OnButtonOneClicked();
  void OnButtonTwoClicked();
  void OnButtonThreeClicked();
  void OnButtonFourClicked();
  void OnButtonFiveClicked();
  void OnButtonSixClicked();
  void OnButtonSevenClicked();
  void OnButtonEightClicked();
  void OnButtonNineClicked();

  void OnButtonCommaClicked();
  void OnButtonNegateClicked();
  void OnButtonBackspaceClicked();

  void OnButtonPowerClicked();
  void OnButtonAddClicked();
  void OnButtonSubstractClicked();
  void OnButtonMultiplicateClicked();
  void OnButtonDivideClicked();

  void OnButtonEqualClicked();

  void OnButtonResetClicked();
  void OnButtonMemoryCrearClicked();
  void OnButtonMemoryRetrieveClicked();
  void OnButtonMemorySaveClicked();

 private:
  Ui::MainWindow* ui;

  enum class Operation{ NO_OPERATION, ADDITION, SUBTRACTION, MULTIPLICATION, DIVISION, POWER };
  Operation current_operation_{ Operation::NO_OPERATION };
  Calculator calculator_;
  QString input_number_;
  Number active_number_;
  double memory_;
  bool is_numer_in_memory_ = false;

  void SetText(const QString& text);
  void AddText(const QString& suffix);
  void SetOperation(Operation op);
  QString OpToString(Operation op) const;
};
