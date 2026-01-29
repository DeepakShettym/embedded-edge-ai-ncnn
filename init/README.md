## Init Service Definition

The `init/system_ai_governor.rc` file defines how this service would be
registered with Android init on OEM or engineering builds.

On production Android devices, vendor partitions are read-only and protected
by Verified Boot, so this file is provided as a design artifact rather than
being loaded at runtime.

