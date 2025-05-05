# SCK_V1_main

## 📌 Description
Firmware for the **main controller** of a modular super-custom keyboard.  
Handles USB HID communication and I2C messaging with attached key modules.

> Fully original implementation — no QMK/KMK framework used.

---

## 🔧 Development Environment

- **IDE**: Arduino IDE  
- **Board**: Arduino Pro Micro  
  - ATmega32U4, 5V, 16MHz

---

## 🔌 Connected Modules

Each module has its own firmware repo. Flash separately.
- [SCK_modules](https://github.com/Crem2y/SCK_modules)

---

## 📡 Communication (I2C)

- Standard I2C master-slave communication  
- Each module is assigned a unique fixed I2C address  
- Main controller polls each module individually for input states

---

## 📚 Used Libraries

- [**NicoHood/HID**](https://github.com/NicoHood/HID):  
  Used for enhanced USB HID support on ATmega32U4 boards.
