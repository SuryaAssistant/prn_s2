<!-- Title -->
<span align = "center">

# PRN
  <b>Bidirectional SMPS Inverter</b>

STM32 Code

September - December 2021

</span>
<!-- End of Title -->

## Warning
The code and OpenHantek software are produced in Linux OS computer. We recommend you to use or edit the code in Linux computer for compatibility purpose. Nevertheless, you still can use this repo in Windows computer with some configuration but not for OpenHantek.

## Brief Overview of The Repository
This repository is used to save and documenting the development of bidirectional SMPS inverter project. The project was took place in Instrumentation and Control Laboratory, Universitas Sebelas Maret.

## Prequerities
Yout need to have this software before use this repository
- STM32CubeIDE v1.6.1 or above. Link : https://www.st.com/en/development-tools/stm32cubeide.html
- OpenHantek6022 (Linux). Link : https://github.com/OpenHantek/OpenHantek6022
- Autodesk Eagle v9.3.0 or above (Linux or Windows). Link : https://www.autodesk.com/products/eagle/free-download

## How To Use (Linux)
From Terminal, clone this repository to your folder
```
git clone https://github.com/SuryaAssistant/prn_s2
```

Open STM32CubeIDE and then open project `prn_s2.ioc` in `stm32` folder.

The main code is saved in `stm32/Core/Src/main.c`.

## How To Use (Windows)
Download this repository by clicking green button in right-top of repository.

Extract the zip file and then move the `stm32` folder into <b>STM32CubeIDE workspace</b> in (usually) `C:\Users\{Your_Computer_Name}\STM32CubeIDE\{Your_Workspace_Name}\`.

## Block Diagram

Tegangan keluaran dari inverter ditetapkan sebesar 220VAC. Tegangan ini dapat beruba

## Pinout
<span align="center">
  
 ![Logo](https://github.com/SuryaAssistant/prn_s2/blob/main/img/pinout.jpeg)
  
</span>

## Further Information
- Read the STM32CubeIDE timer and ADC configuration in https://github.com/SuryaAssistant/prn_s2/tutorial
- Read SPWM specific configuration in https://github.com/SuryaAssistant/prn_s2/spwm
- PCB schematic and board Design Documentation in https://github.com/SuryaAssistant/prn_s2/pcb
