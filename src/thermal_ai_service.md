
Temperature-based behavior:
- 🟩 **Cool** → Full-speed AI inference
- 🟨 **Warm** → Reduced AI speed (thread throttling)
- 🟥 **Hot** → AI paused until temperature normalizes

This demonstrates direct interaction with **kernel-exposed hardware sensors**, which is not possible in normal applications.

---

### ⏱️ Real-Time (Soft Real-Time) AI Behavior

The service enforces a **real-time inference deadline**:

- Inference must complete within **50 ms**
- If the deadline is missed:
  - The inference result is **discarded**
  - A deadline violation is logged
- The system continues running safely

This implements **soft real-time constraints**, which are critical in edge AI systems such as:
- Robotics
- Automotive perception
- Industrial monitoring
- Thermal-constrained devices

---

### Why this matters

- Demonstrates **embedded systems thinking**
- Combines AI + hardware monitoring + timing guarantees
- Ensures predictable system behavior
- Cannot be achieved using normal PC apps, mobile apps, or cloud ML pipelines

---

## 🧠 Key Embedded Concepts Demonstrated

- Native ARM64 AI inference
- Linux sysfs hardware access
- Long-running system services
- Thermal safety and protection logic
- Soft real-time inference deadlines
- Result dropping on deadline miss
- Resource-aware AI execution
- Edge AI deployment principles

---

## 🛠️ Tech Stack

- **Language:** C++
- **AI Framework:** NCNN
- **Platform:** Android / Embedded Linux
- **Architecture:** ARM64
- **Image decoding:** stb_image
- **Build system:** CMake + Android NDK

---

## 🎯 How to Run (Summary)

1. Build using **Android NDK + CMake**
2. Push binaries, models, and images to the device
3. Set runtime library path:
   ```bash
   export LD_LIBRARY_PATH=/data/local/tmp



### 📟 Sample Runtime Output (Real Device)

t527-demo-car:/data/local/tmp $ ./thermal_ai_service                                                                                                    
Thermal-Aware AI Service starting...
AI model loaded
CPU Temp: 54.5°C
 Normal → Full AI speed
⏱ Inference OK: 21 ms (within deadline)
CPU Temp: 54.5°C
 Normal → Full AI speed
⏱ Inference OK: 37 ms (within deadline)
CPU Temp: 54.4°C
 Normal → Full AI speed
⏱ Inference OK: 28 ms (within deadline)
CPU Temp: 54.2°C
 Normal → Full AI speed
⏱ Inference OK: 23 ms (within deadline)
CPU Temp: 54.2°C
 Normal → Full AI speed
⏱ Inference OK: 36 ms (within deadline)
CPU Temp: 54.2°C
 Normal → Full AI speed
⏱ Inference OK: 27 ms (within deadline)
CPU Temp: 54.2°C
 Normal → Full AI speed
⏱ Inference OK: 27 ms (within deadline)
CPU Temp: 54.2°C
 Normal → Full AI speed
⏱ Inference OK: 28 ms (within deadline)


