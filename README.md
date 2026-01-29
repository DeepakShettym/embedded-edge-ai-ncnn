# Embedded Edge AI using NCNN

This repository demonstrates **on-device / embedded AI inference** using **NCNN** and **native C++**. The focus is on running AI models **without Python, Java, or Android app frameworks**, keeping the design close to **system-level and embedded engineering** practices.

The project is intentionally simple and transparent so that model loading, file I/O, and inference flow are easy to understand and extend.

---

## 🔹 What this repository contains

* Native C++ examples using **NCNN**
* Loading and running models using **`.param` + `.bin`**
* A clean **CMake-based build** (no Gradle, no APK)
* Code suitable for **Android NDK**, Linux, and embedded targets

This repo is meant as a **foundation** for building more advanced edge-AI services.

---

## 🔹 Why NCNN

NCNN is a high‑performance neural network inference framework written in C++ and optimized for mobile and embedded devices.

Key reasons for using NCNN:

* No Python runtime
* No JVM / Android framework dependency
* Small binary size
* Optimized for ARM (NEON)
* Predictable latency and memory usage

NCNN is well suited for **system services, daemons, and embedded pipelines**.

---

## 📁 Project Structure

```
embedded-edge-ai-ncnn/
│
├── build/                  # Build output and project binaries
│
├── models/                 # NCNN trained model files
│   ├── *.param             # NCNN network structure
│   └── *.bin               # NCNN trained weights
│
├── src/                    # Source code for the inference projects
│   ├── main.cpp            # Basic AI inference example
│   ├── image_infer.cpp     # Image classification pipeline
│   ├── thermal_ai_service.cpp  # Example of a hardware-aware AI service
│   └── ...                # Other source files (depends on what’s in the repo)
│
├── .gitignore              # Git ignore rules
├── CMakeLists.txt          # Top-level build configuration
└── README.md               # Top-level documentation
```

> **Note:** `build/` is generated locally by CMake and may not be committed to Git.

---

## 🔹 Model format (.param / .bin)

NCNN separates a model into two files:

* **`.param`** – describes the network structure (layers, connections)
* **`.bin`** – contains trained weights and biases (binary data)

Both files are required at runtime.

---

## 🔹 Build (Linux / Android NDK)

### Example (Linux)

```bash
mkdir -p build
cd build
cmake ..
make
```

### Example (Android NDK – concept)

```bash
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_PLATFORM=android-29

make
```

The output is a **native binary**, not an APK.

---

## 🔹 Runtime flow (high level)

1. Binary starts
2. NCNN network is initialized
3. `.param` and `.bin` are loaded from disk
4. Input data is prepared (image / tensor)
5. Inference is executed
6. Output is read and processed

This flow maps cleanly to **embedded and system-level deployments**.

---

## 🔹 Current status

* [x] Native C++ inference examples
* [x] NCNN model loading
* [x] CMake-based build

---

## 🔹 Author

**Deepak Shetty**
Software Engineer | Embedded & Edge AI

---

## 🔹 License

This repository is for learning, experimentation, and demonstration purposes.
