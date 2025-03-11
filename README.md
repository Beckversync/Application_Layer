

# 🛠️ Embedded System Project with STM32F1  

## 📝 Overview  
A **comprehensive embedded system** is developed using the **STM32F103RB microcontroller**, combining multiple hardware components and software layers to ensure **efficient control and communication**. This system encompasses **peripheral management**, **display control**, **media playback**, and **protocol encoding/decoding**, resulting in a **feature-rich and modular design**.

## 🧱 Project Structure  
- **Application Layer**: High-level control and system logic.  
- **Keypad 4x4**: Interface for **user input** through a 4x4 matrix keypad.  
- **LCD & OLED Display Control**: Functions to manage **LCD** and **OLED screens**, providing **real-time information display**.  
- **MP3 Player**: Module for **audio playback** and media control.  
- **Protocol Buffers (pb)**:  
  - `pb_common.c`: Common utilities for protocol buffer operations.  
  - `pb_encode.c` & `pb_decode.c`: Encoding and decoding data for **efficient communication**.  
- **STM32 HAL & System Files**:  
  - `stm32f1xx_hal_msp.c`, `stm32f1xx_it.c`: **HAL libraries** and **interrupt handling**.  
  - `syscalls.c`, `sysmem.c`, `system_stm32f1xx.c`: **System-level functions** and memory management.  
- **Main Application**: `main.c` – **Central control logic**, initialization, and task management.  

## 🌟 Features  
- **Real-time Display**: LCD and OLED integration for **visual feedback**.  
- **Media Playback**: MP3 player module for **audio output**.  
- **User Interaction**: **4x4 Keypad** for manual input.  
- **Protocol Buffers**: **Efficient data serialization** and parsing.  
- **Robust System Management**: **Interrupt handling**, **memory management**, and **system-level configuration**.  

## 🔧 Tools & Technologies  
- **STM32CubeIDE** / **Keil** for development.  
- **C Language** for firmware.  
- **HAL Libraries** for STM32 peripheral control.  

