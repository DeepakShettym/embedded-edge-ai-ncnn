# Embedded Edge AI using NCNN (Android / Linux)

This repository contains **native C++ embedded AI projects** built using the **NCNN inference framework**, targeting **ARM-based embedded and Android systems**.

The focus is on **system-level, hardware-aware, and real-time aware AI**, not application-level or cloud-based ML.

---

## 🚀 Projects Included

---

## 1️⃣ Basic AI Inference Service  
**File:** `src/main.cpp`

A minimal native AI service demonstrating how NCNN can be used at **system level** without Android app components.

### Features
- Loads NCNN framework
- Runs as a long-running native process
- No UI, no Android framework dependency

### Concepts
- Native C++
- ARM64 execution
- Long-running daemon-style program

---

## 2️⃣ Image Classification Pipeline  
**File:** `src/image_infer.cpp`

An end-to-end **on-device image inference pipeline** implemented fully in native C++.

### Features
- Loads a real JPG image from filesystem
- Decodes image using `stb_image`
- Resizes and normalizes input
- Runs inference using SqueezeNet (NCNN)
- Outputs top prediction and confidence

### Concepts
- Image → tensor preprocessing pipeline
- On-device inference
- Explicit memory handling
- No Python
- No OpenCV
- No Java/Kotlin

---

## 3️⃣ Thermal-Aware & Real-Time Embedded AI Service ⭐  
**File:** `src/thermal_ai_service.cpp`

A **hardware-aware, real-time aware AI service** designed for embedded and edge systems.

This service continuously monitors CPU temperature and dynamically adapts AI behavior to protect hardware and ensure predictable system behavior.

---

### 🔥 Thermal Awareness (Hardware-Level)

The service reads real CPU temperature from Linux sysfs:

