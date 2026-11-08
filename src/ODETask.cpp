#include "ODETask.h"
#include "ODEExceptions.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <utility>
namespace {
 double checkedDerivative(const ODETask::Derivative& f,double x,double y) {
  if(!f) throw std::invalid_argument("Derivative function is empty");
  double v=f(x,y);
  if(!std::isfinite(v)) throw InvalidDerivativeException("Derivative returned NaN/Inf");
  return v;
 }
 double checked(double v) { if(!std::isfinite(v)) throw NumericalOverflowException("Nonfinite numerical result"); return v; }
}
void ODETask::validate(double x,double y,double end,double h,std::size_t max) {
 if(!std::isfinite(x)||!std::isfinite(y)||!std::isfinite(end)) throw std::invalid_argument("Initial values and interval must be finite");
 if(!std::isfinite(h)||h<=0) throw std::invalid_argument("Step must be positive and finite");
 if(end<=x) throw std::invalid_argument("Interval must satisfy end > start");
 if(max==0) throw std::invalid_argument("Maximum steps must be positive");
}
ODETask::ODETask(double x,double y,double end,double h,std::size_t max):x0_(x),y0_(y),end_(end),step_(h),maxSteps_(max) {validate(x,y,end,h,max);}
void ODETask::replaceParameters(double x,double y,double end,double h,std::size_t max) {
 validate(x,y,end,h,max);
 x0_=x; y0_=y; end_=end; step_=h; maxSteps_=max;
 trajectory_.clear(); // clear() for trivially destructible point is noexcept
}
void ODETask::solve(const Derivative& f,int method) {
 if(!f) throw std::invalid_argument("Derivative function is empty");
 // The complete trajectory is built before committing; errors leave old state unchanged.
 std::vector<ODEPoint> next;
 double x=x0_,y=y0_;
 next.push_back({x,y});
 for(std::size_t i=0; x<end_; ++i) {
  if(i>=maxSteps_) throw StepLimitException("Maximum number of steps exceeded");
  double h=std::min(step_,end_-x);
  if(!(h>0)||!std::isfinite(h)||x+h<=x) throw NumericalOverflowException("Step cannot advance x");
  double k1=checkedDerivative(f,x,y), yn=0;
  if(method==0) yn=checked(y+h*k1);
  else if(method==1) {
   double predictor=checked(y+h*k1);
   double k2=checkedDerivative(f,checked(x+h),predictor);
   yn=checked(y+h*(k1+k2)/2.0);
  } else {
   double k2=checkedDerivative(f,checked(x+h/2),checked(y+h*k1/2));
   double k3=checkedDerivative(f,checked(x+h/2),checked(y+h*k2/2));
   double k4=checkedDerivative(f,checked(x+h),checked(y+h*k3));
   yn=checked(y+(h/6)*(k1+2*k2+2*k3+k4));
  }
  x=checked(x+h); y=yn;
  next.push_back({x,y});
 }
 trajectory_.swap(next);
}
void ODETask::solveEuler(const Derivative& f){solve(f,0);}
void ODETask::solveHeun(const Derivative& f){solve(f,1);}
void ODETask::solveRK4(const Derivative& f){solve(f,2);}
