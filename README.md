![Microcontroller](https://img.shields.io/badge/Microcontroller-STM32L476RG-03234B?logo=stmicroelectronics)
![CPU Architecture](https://img.shields.io/badge/CPU_Architecture-ARM-0091BD?logo=arm)
![License](https://img.shields.io/badge/License-MIT-yellow)

# STM32L4-RobustRegression-EnergyAnalysis

## Quick Overview
This repository contains code for the STM32L476 microcontroller, aimed at investigating the performance and energy consumption of robust linear regression estimators.

## Project Description

This repository contains code for the STM32L476 microcontroller, aimed at investigating the performance and energy consumption of robust linear regression estimators. The study will focus on the energy usage of these estimators under various conditions, including different supply voltages, operating modes, clock frequencies, and CPU temperatures.

The following robust regression estimators will be tested:

- **Theil-Sen Estimator**
- **RANSAC (RANdom SAmple Consensus)**
- **Huber Regression**

Additionally, the **Ordinary Least Squares (OLS) Estimator** will be used as a reference to compare with the robust estimators.

The goal of this project is to provide insights into the energy efficiency of these estimators on the STM32L476 platform, which is crucial for optimizing performance in energy-constrained environments.

### Data Generation and Use
- Synthetic data will be generated using Python, leveraging the `make_regression` function from the `datasets` module in the `scikit-learn` package. Outliers will be added to the generated data to simulate real-world conditions.
- A real-world electrical use case will be examined by characterizing a diode using the DAC output and ADC input of the microcontroller. The regression estimators will be applied to a selected portion of the diode's characteristic curve.
- A public dataset from Kaggle may also be used to provide additional insights and validation.

### Measurement Setup
Energy consumption measurements will be conducted using the STM32 X-NUCLEO-LPM01A expansion board, which offers precise power measurement capabilities for low-power applications. This board allows for accurate measurement of the power consumption of the STM32L476 microcontroller during the execution of the regression algorithms under various conditions.

## Hardware Requirements
- STM32L476RG Microcontroller
- STM32 X-NUCLEO-LPM01A Power Measurement Expansion Board

## Software Requirements
- Python with `scikit-learn` package
- development environment for STM32 with `arm-none-eabi` toolchain installed

## License
This project is licensed under the MIT License.

 
