---

## 4️⃣ System-Aware AI Governor (Embedded Control AI) ⭐⭐

**File:** `src/system_ai_governor.cpp`

A system-level AI controller that continuously monitors **hardware and OS state** and dynamically controls how aggressively AI is allowed to run.

This project treats AI as a **resource-aware system component**, not a standalone task.

---

### 🔍 What it Monitors (Kernel-Level)

* **CPU temperature** via Linux sysfs
  `/sys/class/thermal/thermal_zone0/temp`

* **CPU utilization** via procfs
  `/proc/stat`

These values are read **directly from the Linux kernel**, without any framework or middleware.

---

### 🧠 AI Control Logic

The governor evaluates system health in real time and switches AI behavior modes:

| Mode          | Condition           | Behavior                     |
| ------------- | ------------------- | ---------------------------- |
| 🟢 FULL AI    | Cool + low CPU load | Max threads, fast execution  |
| 🟨 LIMITED AI | Warming system      | Reduced threads, slower loop |
| 🟥 SURVIVAL   | High temp / load    | AI paused, monitoring only   |

This ensures:

* Thermal safety
* Predictable performance
* System stability on embedded hardware

---

### 📟 Sample Runtime Output (Real Device)

```text
t527-demo-car:/data/local/tmp $ ./system_ai_governor
System AI Governor starting...
Temp: 35.1°C | CPU Load: 11.1% | MODE: FULL AI
AI inference executed
Temp: 34.6°C | CPU Load: 1.5% | MODE: FULL AI
AI inference executed
Temp: 34.2°C | CPU Load: 0.9% | MODE: FULL AI
AI inference executed
Temp: 34.5°C | CPU Load: 0.9% | MODE: FULL AI
AI inference executed
Temp: 34.4°C | CPU Load: 1.2% | MODE: FULL AI
AI inference executed
Temp: 34.2°C | CPU Load: 0.5% | MODE: FULL AI
AI inference executed
Temp: 34.3°C | CPU Load: 1.0% | MODE: FULL AI
AI inference executed
Temp: 34.2°C | CPU Load: 1.1% | MODE: FULL AI
AI inference executed
Temp: 34.6°C | CPU Load: 0.9% | MODE: FULL AI
AI inference executed
Temp: 34.6°C | CPU Load: 0.6% | MODE: FULL AI
AI inference executed
Temp: 34.4°C | CPU Load: 1.1% | MODE: FULL AI
AI inference executed
Temp: 34.4°C | CPU Load: 0.8% | MODE: FULL AI
AI inference executed
Temp: 34.3°C | CPU Load: 1.0% | MODE: FULL AI
AI inference executed
Temp: 34.7°C | CPU Load: 11.0% | MODE: FULL AI
AI inference executed
Temp: 35.1°C | CPU Load: 4.1% | MODE: FULL AI
AI inference executed
Temp: 34.9°C | CPU Load: 1.5% | MODE: FULL AI
AI inference executed
Temp: 36.3°C | CPU Load: 22.7% | MODE: FULL AI
AI inference executed
Temp: 34.9°C | CPU Load: 15.4% | MODE: FULL AI
AI inference executed
```

---

## 🧠 System AI Governor (Embedded On-Device AI)

### Overview

System AI Governor is a native, system-level AI control service written in **C++ for Android (ARM64)**. It runs as a long-lived native daemon and dynamically manages on-device AI inference based on **thermal conditions, CPU load, and external control commands**.

The project demonstrates **embedded systems design**, **on-device AI inference**, and **concurrency-safe control planes** using **Unix domain sockets**.

---

### Key Features

* ✅ Native C++ daemon (no app / no JNI / no Gradle)
* ✅ Runs continuously on Android using NDK
* ✅ On-device AI inference using NCNN
* ✅ Real-time monitoring of:

  * CPU temperature (`/sys/class/thermal`)
  * CPU load (`/proc/stat`)
* ✅ Dynamic AI throttling based on system conditions
* ✅ IPC control via Unix domain sockets
* ✅ Thread-safe design using atomics and mutexes
* ✅ ARM64-optimized, embedded-friendly architecture

---

### Architecture

```text
Client (CLI / Script)
        |
        |  Unix Domain Socket
        v
+----------------------------------+
| system_ai_governor (C++)         |
|                                  |
|  - Thermal monitoring            |
|  - CPU load monitoring           |
|  - AI policy engine              |
|  - NCNN inference runtime        |
|  - IPC command listener          |
+----------------------------------+
```

The AI model is loaded **once** and reused, avoiding repeated initialization overhead.

---

### AI Operating Modes

| Mode    | Description                                    |
| ------- | ---------------------------------------------- |
| AUTO    | System decides AI behavior based on temp & CPU |
| FULL    | Force maximum AI performance                   |
| LIMITED | Throttled AI execution                         |
| OFF     | AI completely disabled                         |

Mode switching is **lock-free** and immediately visible across threads using **atomic state variables**.

---

### IPC Control Interface

The service exposes a Unix domain socket at:

```text
/data/local/tmp/system_ai.sock
```

#### Supported Commands

```text
STATUS
SET_MODE AUTO
SET_MODE FULL
SET_MODE LIMITED
SET_MODE OFF
RUN_INFER
```

#### Example Usage

```bash
echo "STATUS" | nc -U /data/local/tmp/system_ai.sock
echo "SET_MODE OFF" | nc -U /data/local/tmp/system_ai.sock
echo "SET_MODE AUTO" | nc -U /data/local/tmp/system_ai.sock
```

---

### 📟 IPC & Runtime Logs (Real Device)

```text
t527-demo-car:/ $ ls -l /data/local/tmp/system_ai.sock
srwxrwxrwx 1 shell shell 0 2026-01-29 14:55 /data/local/tmp/system_ai.sock

t527-demo-car:/ $ echo "STATUS" | nc -U /data/local/tmp/system_ai.sock
TEMP=53.723999 CPU=0.752825 MODE=0

t527-demo-car:/ $ echo "SET_MODE OFF" | nc -U /data/local/tmp/system_ai.sock
OK MODE OFF

t527-demo-car:/ $ echo "SET_MODE AUTO" | nc -U /data/local/tmp/system_ai.sock
OK MODE AUTO

t527-demo-car:/ $ echo "STATUS" | nc -U /data/local/tmp/system_ai.sock                                                                       
TEMP=53.872002 CPU=1.002508 MODE=0


```

---

### 📟 Full Runtime Log – Governor Execution (Real Device)

```text
deepak@deepak-ThinkPad-E14-Gen-6:~/ai_inference_service/build$ adb shell
t527-demo-car:/ $ cd /data/local/tmp
t527-demo-car:/data/local/tmp $ export LD_LIBRARY_PATH=/data/local/tmp
t527-demo-car:/data/local/tmp $ ./system_ai_governor
System AI Governor starting...
[GOVERNOR] IPC socket ready
Temp: 53.8°C | CPU: 1.1% | MODE: AUTO → FULL
AI inference executed
Temp: 53.6°C | CPU: 0.9% | MODE: AUTO → FULL
AI inference executed
Temp: 53.2°C | CPU: 1.1% | MODE: AUTO → FULL
AI inference executed
Temp: 53.4°C | CPU: 0.9% | MODE: AUTO → FULL
AI inference executed
Temp: 53.5°C | CPU: 0.5% | MODE: AUTO → FULL
AI inference executed
Temp: 53.2°C | CPU: 1.0% | MODE: AUTO → FULL
AI inference executed
Temp: 53.7°C | CPU: 0.6% | MODE: AUTO → FULL
AI inference executed
Temp: 53.3°C | CPU: 0.9% | MODE: AUTO → FULL
AI inference executed
Temp: 53.4°C | CPU: 0.6% | MODE: AUTO → FULL
AI inference executed
Temp: 53.3°C | CPU: 0.8% | MODE: AUTO → FULL
AI inference executed
Temp: 53.2°C | CPU: 0.9% | MODE: AUTO → FULL
AI inference executed
Temp: 53.5°C | CPU: 1.2% | MODE: AUTO → FULL
AI inference executed
Temp: 53.3°C | CPU: 0.8% | MODE: AUTO → FULL
AI inference executed
Temp: 53.4°C | CPU: 0.9% | MODE: AUTO → FULL
AI inference executed
Temp: 53.7°C | CPU: 0.6% | MODE: AUTO → FULL
AI inference executed
Temp: 53.6°C | CPU: 1.1% | MODE: AUTO → FULL
AI inference executed
Temp: 53.6°C | CPU: 0.8% | MODE: AUTO → FULL
AI inference executed
Temp: 53.4°C | CPU: 0.8% | MODE: AUTO → FULL
AI inference executed
Temp: 53.4°C | CPU: 0.9% | MODE: AUTO → FULL
AI inference executed
Temp: 53.7°C | CPU: 0.7% | MODE: AUTO → FULL
AI inference executed
Temp: 53.7°C | CPU: 0.8% | MODE: AUTO → FULL
AI inference executed
Temp: 53.7°C | CPU: 2.4% | MODE: AUTO → FULL
AI inference executed
Temp: 53.5°C | CPU: 1.4% | MODE: OFF (AI disabled)
Temp: 53.8°C | CPU: 0.8% | MODE: OFF (AI disabled)
Temp: 53.4°C | CPU: 0.8% | MODE: OFF (AI disabled)
Temp: 53.7°C | CPU: 0.8% | MODE: OFF (AI disabled)
Temp: 53.6°C | CPU: 0.9% | MODE: OFF (AI disabled)
Temp: 53.4°C | CPU: 1.1% | MODE: AUTO → FULL
AI inference executed
Temp: 53.9°C | CPU: 1.0% | MODE: AUTO → FULL
AI inference executed
^CSystem AI Governor shutting down
t527-demo-car:/data/local/tmp $ exit
```

---

### Author

**Deepak**
Embedded Systems & On-Device AI
