# HAWAII : Hardware Accelerated Intermittent Deep Inference

<!-- ABOUT THE PROJECT -->
## Overview

HAWAII is a middleware stack for doing inference on energy-harvesting intermittent system. We implemented our HAWAII design on the Texas Instruments MSP430FR5994 LaunchPad, and used the internal low-energy accelerator (LEA) hardware for DNN inference acceleration. HAWAII consists of multiple inference functions, which are exposed via an intuitive API. Each inference function contains four major design components: 
* the footprint variable.
* the footprint monitor.
* the recovery handler.
* the function body. 

The footprint variable indicates the current progress of each network/layer-level operation. The function body is the actual operation in the inference pipeline (e.g., convolution/pooling). The recovery handler performs footprint-aware initialization to correctly resume an interrupted inference task upon power resumption. The footprint monitor works closely with the function body to perform footprint preservation, where it periodically captures the number of completed accelerator/CPU-based sub-operations as the inference execution progresses.

<!-- TABLE OF CONTENTS -->
## Table of Contents
* [Directory/File Structure](#directory/file-structure)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Setup and Build](#setup-and-build)
* [Using HAWAII](#using-hawaii)
  
<!--* [Contributing](#contributing)-->

## Directory/File Structure
Below is an explanation of the directories/files found in this repo. 
```
├── driverlib
│   └── ...
├── dsplib
│   └── ...
├── libHAWAII
│   ├── HAWAII.h
│   ├── convolution.h
│   ├── fc.h
│   ├── footprinting.h
│   └── nonlinear.h
├── main.c
├── main.h
└── model.h
```
`driverlib/` is a set of drivers for accessing the peripherals found on the MSP430 family of microcontrollers. 

`dsplib/` is a set of highly optimized functions to perform many common signal processing operations on fixed-point numbers for MSP430 microcontrollers. 

`libHAWAII/` contains source code for basic neural network operations for HAWAII. 

`main.c` an example code to run a DNN inference.


<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

Here is the basic software and hardware you need to build/run the provided example. 

* [Code composer studio](http://www.ti.com/tool/CCSTUDIO "link") (recommended versions: > 7.0)
* [MSP Driver Library](http://www.ti.com/tool/MSPDRIVERLIB "link")
* [MSP DSP Library](http://www.ti.com/tool/MSP-DSPLIB "link")
* [MSP-EXP430FR5994 LaunchPad](http://www.ti.com/tool/MSP-EXP430FR5994 "link")

### Setup and Build

1. Download/clone this repository
2. Import this project to your workspace of code composer studio (CCS). 

Now, the demo project is ready to go. Just launch the demo application by clicking the debug button. In CCS, you can trace how the design work step by step. 

## Using HAWAII






