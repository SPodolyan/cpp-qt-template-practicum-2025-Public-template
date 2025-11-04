#include "calculator.h"
#include <cmath>

Calculator::Calculator() : number_(0.0) {
}

void Calculator::Set(Number value) {
    number_ = value;
}

Number Calculator::GetNumber() const {
    return number_;
}

void Calculator::Add(Number value) {
    number_ += value;
}

void Calculator::Sub(Number value) {
    number_ -= value;
}

void Calculator::Mul(Number value) {
    number_ *= value;
}

void Calculator::Div(Number value) {
    number_ /= value;
}

void Calculator::Pow(Number value) {
    number_ = std::pow(number_, value);
}

