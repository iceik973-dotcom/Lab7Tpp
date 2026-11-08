#include "ODETask.h"
#include "ODEExceptions.h"
#include <iostream>
#include <stdexcept>
#include <string>
struct ScopeLogger {std::string name; explicit ScopeLogger(std::string n):name(std::move(n)){std::cout<<"Enter: "<<name<<'\n';} ~ScopeLogger() noexcept {std::cout<<"Exit: "<<name<<'\n';}};
void inner(ODETask& task){ScopeLogger guard("inner"); task.solveRK4([](double,double){throw InvalidDerivativeException("Artificial derivative failure");return 0.0;});}
void middle(ODETask& task){ScopeLogger guard("middle");try{inner(task);}catch(const ODEException&){std::cout<<"Rethrowing original exception\n";throw;}}
int main(){
 try{
  ODETask task(0,1,1,0.1);
  auto f=[](double,double y){return y;};
  task.solveEuler(f); std::cout<<"Euler: "<<task.trajectory().back().y<<'\n';
  task.solveHeun(f); std::cout<<"Heun: "<<task.trajectory().back().y<<'\n';
  task.solveRK4(f); std::cout<<"RK4: "<<task.trajectory().back().y<<'\n';
  auto previous=task.trajectory();
  try{middle(task);}catch(const InvalidDerivativeException& ex){std::cout<<"Caught: "<<ex.what()<<'\n';}
  std::cout<<"Strong guarantee: "<<(previous.size()==task.trajectory().size() && previous.back().y==task.trajectory().back().y ? "preserved":"FAILED")<<'\n';
  try{ODETask invalid(0,1,1,0);}catch(const std::invalid_argument& ex){std::cout<<"Input error: "<<ex.what()<<'\n';}
 }catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}
}
