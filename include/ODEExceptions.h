#pragma once
#include <stdexcept>
#include <string>
class ODEException : public std::runtime_error { public: using std::runtime_error::runtime_error; };
class InvalidDerivativeException : public ODEException { public: using ODEException::ODEException; };
class StepLimitException : public ODEException { public: using ODEException::ODEException; };
class NumericalOverflowException : public ODEException { public: using ODEException::ODEException; };
