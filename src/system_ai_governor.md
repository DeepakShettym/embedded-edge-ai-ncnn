---

## 4️⃣ System-Aware AI Governor (Embedded Control AI) ⭐⭐
**File:** `src/system_ai_governor.cpp`

A system-level AI controller that continuously monitors **hardware and OS state**
and dynamically controls how aggressively AI is allowed to run.

This project treats AI as a **resource-aware system component**, not a standalone task.

---

### 🔍 What it Monitors (Kernel-Level)

- CPU temperature via Linux sysfs  
---
/sys/class/thermal/thermal_zone0/temp

- CPU utilization via procfs  

These values are read directly from the Linux kernel, without any framework or middleware.

---

### 🧠 AI Control Logic

The governor evaluates system health in real time and switches AI behavior modes:

| Mode | Condition | Behavior |
|----|----|----|
| 🟢 FULL AI | Cool + low CPU load | Max threads, fast execution |
| 🟨 LIMITED AI | Warming system | Reduced threads, slower loop |
| 🟥 SURVIVAL | High temp / load | AI paused, monitoring only |

This ensures:
- Thermal safety
- Predictable performance
- System stability on embedded hardware

---

### 📟 Sample Runtime Output (Real Device)


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
