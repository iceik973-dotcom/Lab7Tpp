#pragma once
#include <cstddef>
#include <functional>
#include <vector>
struct ODEPoint { double x; double y; };
class ODETask {
public:
 using Derivative = std::function<double(double,double)>;
 ODETask(double x0,double y0,double end,double step,std::size_t maxSteps=100000);
 void solveEuler(const Derivative& derivative);
 void solveHeun(const Derivative& derivative);
 void solveRK4(const Derivative& derivative);
 void replaceParameters(double x0,double y0,double end,double step,std::size_t maxSteps);
 const std::vector<ODEPoint>& trajectory() const noexcept { return trajectory_; }
 double start() const noexcept { return x0_; }
 double initialValue() const noexcept { return y0_; }
 double end() const noexcept { return end_; }
 double step() const noexcept { return step_; }
 std::size_t maxSteps() const noexcept { return maxSteps_; }
private:
 double x0_,y0_,end_,step_;
 std::size_t maxSteps_;
 std::vector<ODEPoint> trajectory_;
 static void validate(double,double,double,double,std::size_t);
 void solve(const Derivative&,int method);
};
