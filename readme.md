# Windows Screen Capture DLL

## Project Story
This project is an attempt to boost screen capture performance compared to MSS (Python Screen Capture).
While MSS is excellent for pure Python, this endeavor aims to achieve better results. It's all about optimizing screen capture for enhanced performance while still appreciating MSS as a solid option for Python enthusiasts.

## Description
Windows Screen Capture DLL is a low-level dynamic link library (DLL) designed for high-performance screen capturing on Windows platforms. This library is engineered to provide rapid capture capabilities, making it an ideal solution for applications requiring frequent or real-time screen captures. It is especially useful for developers looking to integrate screen capture features into high-level programming languages.

## Key Features
- **Minimal Dependencies**: The library is designed to be lightweight and portable, only requiring the Windows API and C standard library.
- **Compact**: The library is a single file witch less than 500 lines of code.

## Requirements
- Windows operating system.
- C-compatible compiler (GCC, Clang, MSVC) for compiling the library.

## Compilation
Compile the library to a DLL using the following command (modify the output file name as required):

gcc -shared -o WindowsScreenCapture.dll WindowsScreenCapture.c -luser32 -lgdi32

## Usage
The library serves as a foundational tool that can be integrated into various programming environments through its DLL interface.


## API Overview
The library provides a straightforward API:
- `GetMonitorsInfo`: Retrieve details of connected monitors.
- `CaptureMonitor`: Execute screen captures for specified monitors.
- `FreeAllCaptures`: Efficiently release resources post-capture.

GetMonitorsInfo
```c
int GetMonitorsInfo(MonitorInfo* monitors)
```
CaptureMonitor
```c
CaptureMonitor(int monitorIndex,
               int* width,
               int* height,
               MonitorInfo* monitors,
               int monitorCount)
```

FreeAllCaptures
```c
void FreeAllCaptures()
```

## TODO
- [ ] Add support for custom capture regions.

## Contributions
Contributions are welcome! Please feel free to submit a pull request or open an issue for any bugs or feature requests.
Its my first project in C, so any help is appreciated.

## License
MIT License
