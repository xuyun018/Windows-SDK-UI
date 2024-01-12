#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <windowsx.h>
#include <time.h>
#include <string.h>
#include "dialog02.h"

#define ID_MYTIME 32767

#define GET_WM_COMMAND_ID(wp, lp)               LOWORD(wp)

int SetTimeStr(HWND hWnd)
{
    time_t long_time;
    struct tm* now_time;
    WCHAR dlg_hour[6], dlg_min[6], dlg_sec[6], dlg_ampm[6];

    time(&long_time);
    now_time = localtime(&long_time);
    if (now_time->tm_hour > 12) {
        now_time->tm_hour -= 12;
        wcscpy(dlg_ampm, L"ŒÁ··");
    }
    else {
        wcscpy(dlg_ampm, L"ŒÁ«∞");
    }
    wsprintf(dlg_hour, L"%2d", now_time->tm_hour);
    wsprintf(dlg_min, L"%2d", now_time->tm_min);
    wsprintf(dlg_sec, L"%2d", now_time->tm_sec);

    Static_SetText(GetDlgItem(hWnd, IDC_HOUR), dlg_hour);
    Static_SetText(GetDlgItem(hWnd, IDC_MIN), dlg_min);
    Static_SetText(GetDlgItem(hWnd, IDC_SEC), dlg_sec);
    Static_SetText(GetDlgItem(hWnd, IDC_AMPM), dlg_ampm);
    return 0;
}

LRESULT CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_INITDIALOG:
        SetTimer(hWnd, ID_MYTIME, 1000, NULL);
        return TRUE;
    case WM_TIMER:
        SetTimeStr(hWnd);
        return TRUE;
    case WM_COMMAND:
        switch (GET_WM_COMMAND_ID(wp, lp)) {
        case IDOK:
            if (MessageBox(
                hWnd,
                (LPCWSTR)L"ΩK¡À§∑§∆§‚¡º§§§«§π§´",
                (LPCWSTR)L"ΩK¡À¥_’J",
                MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL) {
                return FALSE;
            }
            KillTimer(hWnd, ID_MYTIME);
            EndDialog(hWnd, IDOK);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

int WINAPI wWinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst,
    LPWSTR lpsCmdLine, int nCmdShow)
{
    DialogBox(hCurInst, L"MYDIALOG", NULL, (DLGPROC)DlgProc);
    return 0;
}