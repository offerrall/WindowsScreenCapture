#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_CAPTURES 60

typedef struct {
    HBITMAP hBitmap;
    unsigned char* pBits;
    int width;
    int height;
} BitmapCapture;

typedef struct {
    int monitorIndex;
    RECT monitorRect;
    BOOL isPrimary;
} MonitorInfo;

typedef struct {
    MonitorInfo* monitors;
    int count;
} MonitorEnumData;

BitmapCapture captures[MAX_CAPTURES];
int captureCount = 0;

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,
                              HDC hdcMonitor,
                              LPRECT lprcMonitor,
                              LPARAM pData) {
    MonitorEnumData* data = (MonitorEnumData*) pData;

    MONITORINFOEX mi;
    mi.cbSize = sizeof(MONITORINFOEX);

    GetMonitorInfo(hMonitor, (MONITORINFO*)&mi);

    data->monitors[data->count].monitorIndex = data->count;
    data->monitors[data->count].monitorRect = mi.rcMonitor;
    data->monitors[data->count].isPrimary = mi.dwFlags & MONITORINFOF_PRIMARY;

    data->count++;
    return TRUE;
}

__declspec(dllexport) int GetMonitorsInfo(MonitorInfo* monitors) {
    MonitorEnumData data;
    data.monitors = monitors;
    data.count = 0;

    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM) &data);
    return data.count;
}

__declspec(dllexport) unsigned char* CaptureMonitor(int monitorIndex,
                                                    int* width,
                                                    int* height,
                                                    MonitorInfo* monitors,
                                                    int monitorCount) {
    if (monitorIndex < 0 || monitorIndex >= monitorCount) {
        fprintf(stderr, "Invalid monitor index\n");
        return NULL;
    }

    if (captureCount >= MAX_CAPTURES) {
        fprintf(stderr, "Maximum number of captures reached\n");
        return NULL;
    }

    MonitorInfo monitor = monitors[monitorIndex];

    HDC hScreen = GetDC(NULL);
    HDC hDC = CreateCompatibleDC(hScreen);

    *width = monitor.monitorRect.right - monitor.monitorRect.left;
    *height = monitor.monitorRect.bottom - monitor.monitorRect.top;

    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = *width;
    bmi.bmiHeader.biHeight = -(*height);  // Negativo para orientación de arriba hacia abajo
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;

    void* pBits;
    HBITMAP hBitmap = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);

    if (!hBitmap) {
        fprintf(stderr, "CreateDIBSection failed\n");
        ReleaseDC(NULL, hScreen);
        DeleteDC(hDC);
        return NULL;
    }

    captures[captureCount].hBitmap = hBitmap;
    captures[captureCount].pBits = (unsigned char*)pBits;
    captures[captureCount].width = *width;
    captures[captureCount].height = *height;
    captureCount++;

    SelectObject(hDC, hBitmap);

    if (!BitBlt(hDC,
                0,
                0,
                *width,
                *height,
                hScreen,
                monitor.monitorRect.left,
                monitor.monitorRect.top,
                SRCCOPY)) {
        fprintf(stderr, "BitBlt failed\n");
    }

    DeleteDC(hDC);
    ReleaseDC(NULL, hScreen);

    return (unsigned char*)pBits;
}

__declspec(dllexport) void FreeAllCaptures() {
    for (int i = 0; i < captureCount; i++) {
        if (captures[i].hBitmap) {
            DeleteObject(captures[i].hBitmap);
            captures[i].hBitmap = NULL;
            captures[i].pBits = NULL;
        }
    }
    captureCount = 0;
}
