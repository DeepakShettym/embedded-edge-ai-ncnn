# Embedded Edge AI using NCNN (Android / Linux)

This repository contains **native C++ embedded AI projects** built using the **NCNN inference framework**, targeting **ARM-based embedded and Android systems**.

The focus is on **system-level, hardware-aware AI**, not application-level ML.

---

## 🚀 Projects Included

### 1️⃣ Basic AI Inference Service
**File:** `main.cpp`

- Loads NCNN framework
- Runs as a long-running native service
- Demonstrates system-level AI execution
- No UI, no Android app layer

**Concepts:**
- Native C++
- ARM64 execution
- Long-running daemon

---

### 2️⃣ Image Classification Pipeline
**File:** `image_infer.cpp`

- Loads a real JPG image from filesystem
- Decodes image using `stb_image`
- Resizes & normalizes input
- Runs inference using SqueezeNet (NCNN)
- Outputs top prediction & confidence

**Concepts:**
- Image → tensor pipeline
- On-device inference
- Memory handling
- No Python / no OpenCV

---

### 3️⃣ Thermal-Aware Embedded AI Service ⭐
**File:** `thermal_ai_service.cpp`

- Reads CPU temperature from Linux sysfs  
  `/sys/class/thermal/thermal_zone0/temp`
- Dynamically adapts AI behavior:
  - Full speed when cool
  - Throttled when warm
  - Paused when hot
- Runs continuously as a system service

**Why this matters:**
- Demonstrates **embedded / edge AI**
- Hardware-aware decision making
- Not possible in normal apps or cloud ML

---

## 🧠 Key Embedded Concepts Demonstrated

- Native ARM64 AI inference
- Linux sysfs hardware access
- Long-running daemons
- Thermal safety logic
- Resource-aware AI execution
- Edge AI deployment

---

## 🛠️ Tech Stack

- Language: **C++**
- AI Framework: **NCNN**
- Platform: **Android / Embedded Linux**
- Architecture: **ARM64**
- Image decoding: **stb_image**

---

## 🎯 How to Run (Summary)

1. Build using Android NDK + CMake
2. Push binaries, models, and images to device
3. Set:
   ```bash
   export LD_LIBRARY_PATH=/data/local/tmp
