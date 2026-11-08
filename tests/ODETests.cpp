#include <gtest/gtest.h>
#include "ODETask.h"
#include "ODEExceptions.h"
#include <cmath>
#include <limits>
#include <stdexcept>
TEST(Validation, RejectsZeroStep){EXPECT_THROW(ODETask(0,1,1,0),std::invalid_argument);}
TEST(Validation, RejectsNegativeStep){EXPECT_THROW(ODETask(0,1,1,-0.1),std::invalid_argument);}
TEST(Validation, RejectsBadInterval){EXPECT_THROW(ODETask(1,1,0,0.1),std::invalid_argument);}
TEST(Validation, RejectsNaNInitial){EXPECT_THROW(ODETask(0,std::numeric_limits<double>::quiet_NaN(),1,0.1),std::invalid_argument);}
TEST(Validation, RejectsInfiniteStart){EXPECT_THROW(ODETask(std::numeric_limits<double>::infinity(),1,2,0.1),std::invalid_argument);}
TEST(Validation, RejectsZeroLimit){EXPECT_THROW(ODETask(0,1,1,0.1,0),std::invalid_argument);}
TEST(Methods, EulerConstant){ODETask t(0,1,1,0.2);t.solveEuler([](double,double){return 2.;});EXPECT_NEAR(t.trajectory().back().y,3,1e-12);}
TEST(Methods, HeunExponential){ODETask t(0,1,1,0.1);t.solveHeun([](double,double y){return y;});EXPECT_NEAR(t.trajectory().back().y,std::exp(1.),0.005);}
TEST(Methods, RK4Exponential){ODETask t(0,1,1,0.1);t.solveRK4([](double,double y){return y;});EXPECT_NEAR(t.trajectory().back().y,std::exp(1.),0.00001);}
TEST(Methods, LastStepClipped){ODETask t(0,1,1,0.3);t.solveEuler([](double,double){return 1.;});EXPECT_NEAR(t.trajectory().back().x,1,1e-12);EXPECT_EQ(t.trajectory().size(),5u);}
TEST(Errors, InvalidDerivative){ODETask t(0,1,1,0.1);EXPECT_THROW(t.solveEuler([](double,double){return std::numeric_limits<double>::quiet_NaN();}),InvalidDerivativeException);}
TEST(Errors, InfiniteDerivative){ODETask t(0,1,1,0.1);EXPECT_THROW(t.solveHeun([](double,double){return std::numeric_limits<double>::infinity();}),InvalidDerivativeException);}
TEST(Errors, StepLimit){ODETask t(0,1,1,0.1,2);EXPECT_THROW(t.solveRK4([](double,double){return 1.;}),StepLimitException);}
TEST(Errors, EmptyFunction){ODETask t(0,1,1,0.1);EXPECT_THROW(t.solveEuler({}),std::invalid_argument);}
TEST(Errors, Overflow){ODETask t(0,1e308,1,0.5);EXPECT_THROW(t.solveEuler([](double,double){return 1e308;}),NumericalOverflowException);}
TEST(Strong, FailedDerivativePreservesTrajectory){ODETask t(0,1,1,0.1);t.solveEuler([](double,double){return 1.;});auto before=t.trajectory();EXPECT_THROW(t.solveRK4([](double,double){throw InvalidDerivativeException("fail");return 0.;}),InvalidDerivativeException);ASSERT_EQ(t.trajectory().size(),before.size());EXPECT_DOUBLE_EQ(t.trajectory().back().y,before.back().y);}
TEST(Strong, FailedLimitPreservesTrajectory){ODETask t(0,1,1,0.1,20);t.solveEuler([](double,double){return 1.;});auto before=t.trajectory();t.replaceParameters(0,1,1,0.1,2);EXPECT_THROW(t.solveRK4([](double,double){return 1.;}),StepLimitException);EXPECT_TRUE(t.trajectory().empty());}
TEST(Strong, InvalidReplacePreservesParameters){ODETask t(0,1,1,0.1);EXPECT_THROW(t.replaceParameters(0,5,1,-1,10),std::invalid_argument);EXPECT_DOUBLE_EQ(t.initialValue(),1);EXPECT_DOUBLE_EQ(t.step(),0.1);}
TEST(Strong, FailedSolvePreservesOldResults){ODETask t(0,1,1,0.1);t.solveRK4([](double,double y){return y;});double old=t.trajectory().back().y;EXPECT_THROW(t.solveRK4([](double,double){return std::numeric_limits<double>::quiet_NaN();}),InvalidDerivativeException);EXPECT_DOUBLE_EQ(t.trajectory().back().y,old);}
TEST(Edge, SingleStep){ODETask t(0,2,0.1,0.1);t.solveEuler([](double,double){return 0.;});EXPECT_EQ(t.trajectory().size(),2u);}
