# Init Integration – system_ai_governor

## Purpose of this Document

This document explains how the **System AI Governor** is designed to integrate with **Android init** using an `.rc` file, and why this integration is **conceptual on production devices**.

The goal is to demonstrate **platform-level understanding**, not to bypass Android security mechanisms.

---

## What Is Android Init?

Android uses a custom **init system** (not systemd) to start and manage all core services during boot.

* `init` is the **first userspace process (PID 1)**
* It reads `.rc` files to learn:

  * What services exist
  * When they should start
  * Under which user/group they run
  * How their lifecycle is managed

Every major Android component (zygote, surfaceflinger, HALs) is started this way.

---

## Role of `.rc` Files

`.rc` files are **not shell scripts**. They are declarative configuration files parsed by `init`.

They are used to:

* Define long‑running system services (daemons)
* Define one‑shot boot helpers
* React to system events (boot completed, property changes)

They are considered **part of the operating system**, not runtime configuration.

---

## system_ai_governor.rc

The System AI Governor is intended to run as a **vendor‑level native service**.

A correct init definition (OEM / engineering build) would look like:

```
service system_ai_governor /vendor/bin/system_ai_governor
    class late_start
    user system
    group system
    disabled
```

### Explanation

* **service system_ai_governor**
  Registers a managed init service controllable via `ctl.start`

* **/vendor/bin/system_ai_governor**
  Vendor partition location for OEM native daemons

* **class late_start**
  Starts after core system services and framework are ready

* **user / group system**
  Typical privilege level for system services

* **disabled**
  Prevents automatic start; allows explicit control via IPC or properties

---

## Why Init Integration Does Not Activate on This Device

On **production Android devices**, the following protections are enforced:

* `/system`, `/vendor`, `/product`, `/odm` are **read‑only**
* Partitions are protected by **Android Verified Boot (AVB)**
* Init only loads `.rc` files that are **baked into the OS image at build time**

As a result:

* `.rc` files pushed at runtime may appear in the filesystem
* But **Android init ignores them silently**
* `setprop ctl.start <service>` fails because the service is never registered

This behavior is **intentional and correct**.

---

## Correct Integration Path (OEM / Platform Builds)

In real Android platform development, init services are added during OS build:

```
vendor/<oem>/<device>/init/system_ai_governor.rc
↓
Android build system
↓
vendor.img
↓
Flashed to device
```

Only this path guarantees:

* Service registration
* Correct SELinux labeling
* Verified boot trust

---

## Development & Testing Strategy Used Here

Because this project runs on a **locked production device**:

* Init integration is provided as a **design artifact**
* The service is started manually during development
* IPC control (Unix domain socket) manages runtime behavior

This mirrors how system‑level development is commonly done on non‑OEM hardware.

---

## Key Takeaway

> Android init integration is an OS‑level concern. On production devices, it cannot be modified safely at runtime and must be implemented at build time.

Understanding this boundary is a critical part of Android platform engineering.

---

## Why This Matters

This project demonstrates:

* Knowledge of Android boot architecture
* Correct usage of init service definitions
* Awareness of Verified Boot and vendor partition security
* Engineering judgment to respect platform constraints

These are the same considerations faced by OEM and system software teams.

