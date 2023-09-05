# RISK ANALYSIS of FX and Interest Rates with Monte-Carlo Simulation and AAD

## Abstract
A Monte-Carlo simulation serves as an established algorithmic framework, offering robust capabilities in the realms of financial and quantitative analysis—particularly in the pricing of complex derivatives and in navigating intricate market dynamics. This project delves into the conjunction of Monte-Carlo simulation and an advanced computational method known as Algorithmic Adjoint Differentiation (AAD), intending to enhance the per- formance of the Monte-Carlo simulations. Initially, we examine the foun- dational principles of both Monte-Carlo simulation and AAD, explaining these concepts through practical examples. Subsequently, we conduct a risk analysis focusing on foreign exchange and interest rates, deploying four distinct simulation methodologies. We check that parallel Monte-Carlo simulation emerges as the most efficient technique. Moreover, we check that Monte-Carlo simulations incorporating AAD require significantly greater computational time and memory resources compared to their classical counterparts.


## Getting Started
### Step.1: Cleaning and Building
make clean
make

### Step.2: Serial Monte-Carlo Simulation
To run a serial Monte-Carlo simulation, use the following command:
./main

### Step.3: Serial Monte-Carlo Simulation with AAD
To perform a serial Monte-Carlo simulation that incorporates AAD, use the following command:
./main_mcaad

### Step.4: Parallel Monte-Carlo Simulation
For a parallel version of the Monte-Carlo simulation, execute the following command:
./main_pthread

### Step.5: Parallel Monte-Carlo Simulation with AAD
Before running this simulation, make sure to check the 'reset_adjoint()' function in the 'Node_v1.h' header file (lines 52-79). Use the following command to run the simulation:
./main_multiaad

## Note
Ensure to review and possibly adjust the 'reset_adjoint()' function in the 'Node_v1.h' header file (lines 52-79) before initiating the parallel Monte-Carlo simulation with AAD.

## Link
Project Link: [https://github.com/STEPHAN-EOM/RISK_ANALYSIS](https://github.com/STEPHAN-EOM/RISK_ANALYSIS)