# ToastAPI-Example

A minimal example demonstrating how to use **ToastAPI** from a native Minecraft Bedrock mod.

This project shows how a native shared library can call `ToastAPI` to display an Android Toast message from inside Minecraft Bedrock. The example also demonstrates integrating ToastAPI with the **Preloader** hook system.

## Features

- Native C++20 Minecraft Bedrock mod
- Android Toast notifications through ToastAPI
- Preloader-based native hooking
- ARM64 (`arm64-v8a`) support
- CMake-based build system
- Lightweight release configuration
- Automatic hook installation when the library is loaded
- Automatic hook cleanup when the library is unloaded

## How It Works

The example hooks `StartMenuScreenController::open` from `libminecraftpe.so`.

Once the original function executes, the mod calls:

```cpp
nexcaise::toastapi::sendToastMessage("ToastAPI !!!!!!");
```

The execution flow is:

```text
Minecraft Bedrock
        │
        ▼
StartMenuScreenController::open
        │
        ▼
Native Hook
        │
        ▼
ToastAPI
        │
        ▼
Android Toast
```

The hook is installed through a constructor function when the shared library is loaded and removed through a destructor function when the library is unloaded.

## Project Structure

```text
ToastAPI-Example/
├── include/
├── libs/
│   └── arm64-v8a/
│       ├── libToastAPI.so
│       └── libpreloader.so
├── src/
│   └── main.cpp
├── third_party/
│   └── preloader/
├── CMakeLists.txt
├── buildmod
└── LICENSE
```

## Requirements

- Android NDK
- CMake 3.22 or newer
- ARM64 Android target
- Minecraft Bedrock native library environment
- `libToastAPI.so`
- `libpreloader.so`

The project requires the Android NDK toolchain and uses C++20.

## Building

The repository includes a simple build script:

```bash
chmod +x buildmod
./buildmod
```

The script removes the previous `build` directory, configures a Release CMake build, and builds the shared library using all available CPU cores.

You can also build manually:

```bash
rm -rf build
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

The resulting library is:

```text
build/libToastAPI-Example.so
```

## Usage

Include the ToastAPI header:

```cpp
#include "ToastAPI.hpp"
```

Then send a Toast message:

```cpp
nexcaise::toastapi::sendToastMessage("Hello from ToastAPI!");
```

For example:

```cpp
void showNotification() {
    nexcaise::toastapi::sendToastMessage("Module enabled");
}
```

## Hook Example

The repository demonstrates hooking a Minecraft Bedrock function and displaying a Toast after the original function executes:

```cpp
LL_TYPED_HOOK(
    StartMenuScreenController_open,
    memory::HookPriority::Normal,
    StartMenuScreenController,
    pl::memory::resolveVtableFunction("25StartMenuScreenController", 7, "libminecraftpe.so"),
    "libminecraftpe.so",
    void
) {
    origin();
    nexcaise::toastapi::sendToastMessage("ToastAPI !!!!!!");
}
```

The hook is registered automatically when the library is loaded:

```cpp
__attribute__((constructor))
void onLoad() {
    StartMenuScreenController_open::hook();
}
```

and removed when the library is unloaded:

```cpp
__attribute__((destructor))
void onUnload() {
    StartMenuScreenController_open::unhook();
}
```

The implementation is provided in `src/main.cpp`.

## Linking

`ToastAPI-Example` links against:

- `libToastAPI.so`
- `libpreloader.so`
- `libdl`

The project intentionally does not link directly against `libandroid.so`. ToastAPI accesses Android's Asset Manager through dynamic symbol lookup and Preloader detours.

## Build Configuration

The release build is optimized for a small native library. It uses options such as:

- ThinLTO
- Function and data sectioning
- Hidden symbol visibility
- Section garbage collection
- Identical code folding
- Symbol stripping
- RELRO
- Immediate symbol binding

The project also targets Android's 16 KiB maximum page size through the linker configuration.

## Custom Toast Messages

Once ToastAPI is available, a mod can use it to provide user-facing runtime notifications such as:

```cpp
nexcaise::toastapi::sendToastMessage("Module loaded");
nexcaise::toastapi::sendToastMessage("Configuration loaded");
nexcaise::toastapi::sendToastMessage("Feature enabled");
nexcaise::toastapi::sendToastMessage("Feature disabled");
```

This can be useful for mod status messages, feature feedback, configuration events, and other notifications that should be visible directly to the player.

## Dependencies

This example depends on the following native components:

```text
ToastAPI
Preloader
Minecraft Bedrock native libraries
Android NDK
```

Make sure the required `.so` files and Preloader headers are available before building.

## License

This project is licensed under the **Apache License 2.0**.

See [`LICENSE`](LICENSE) for the complete license text.

## Related Project

**ToastAPI**

ToastAPI provides the native interface used by this example to display Android Toast messages from Minecraft Bedrock native code.

---

Made for Minecraft Bedrock native mod development.
