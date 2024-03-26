//        dentak01.cpp
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <string.h> //memset, strcat, strcpy
#include <stdio.h> //sprintf
#include <math.h> //atof
#include <stdlib.h> //atof
#include "resource.h"

LRESULT CALLBACK MyDlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst,
    LPWSTR lpsCmdLine, int nCmdShow)
{
    DialogBox(hCurInst, L"MYDLG", NULL, (DLGPROC)MyDlgProc);
    return 0;
}

LRESULT CALLBACK MyDlgProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    static WCHAR str[2][32], ans_str[32], tmp[32];
    static HWND hEWnd; //•®•∏•√•»•‹•√•Ø•π§Œ•¶•£•Û•…•¶•œ•Û•…•Î
    static double ans;
    enum { plus, minus, kake, wari };
    static int op, mode; //op:—›À„§Œ∑NÓê°°mode:—›À„◊”§Œ«∞§´··§Ì§´

    switch (msg) {
    case WM_INITDIALOG:
        hEWnd = GetDlgItem(hWnd, IDC_EDIT1);
        Edit_SetText(hEWnd, L"0");
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDOK:
            EndDialog(hWnd, 0);
            return TRUE;
        case IDCANCEL:
            memset(str[mode], '\0', sizeof(str[mode]));
            Edit_SetText(hEWnd, str[mode]);
            return TRUE;
        case IDC_ALLCLEAR:
            memset(str, '\0', sizeof(str));
            memset(ans_str, '\0', sizeof(ans_str));
            mode = 0;
            Edit_SetText(hEWnd, ans_str);
            return TRUE;
        case IDC_POINT:
            wcscat(str[mode], L".");
            Edit_SetText(hEWnd, str[mode]);
            return TRUE;
        case IDC_1:
            wcscat(str[mode], L"1");
            Edit_SetText(hEWnd, str[mode]);
            return TRUE;
        case IDC_2:
            wcscat(str[mode], L"2");
            Edit_SetText(hEWnd, str[mode]);
            return TRUE;
        case IDC_3:
            wcscat(str[mode], L"3");
            Edit_SetText(hEWnd, str[mode]);
            return TRUE;
        case IDC_4:
            wcscat(str[mode], L"4");
            Edit_SetText(hEWnd, str[mode]);
            return TRUE;
        case IDC_5:
            wcscat(str[mode], L"5");
            Edit_SetText(hEWnd, str[mode]);
            return TRUE;
        case IDC_6:
            wcscat(str[mode], L"6");
            Edit_SetText(hEWnd, str[mode]);
            return TRUE;
        case IDC_7:
            wcscat(str[mode], L"7");
            Edit_SetText(hEWnd, str[mode]);
            return TRUE;
        case IDC_8:
            wcscat(str[mode], L"8");
            Edit_SetText(hEWnd, str[mode]);
            return TRUE;
        case IDC_9:
            wcscat(str[mode], L"9");
            Edit_SetText(hEWnd, str[mode]);
            return TRUE;
        case IDC_0:
            wcscat(str[mode], L"0");
            Edit_SetText(hEWnd, str[mode]);
            return TRUE;
        case IDC_PLUS:
            mode = 1;
            op = plus;
            return TRUE;
        case IDC_MINUS:
            mode = 1;
            op = minus;
            return TRUE;
        case IDC_KAKE:
            mode = 1;
            op = kake;
            return TRUE;
        case IDC_WARI:
            mode = 1;
            op = wari;
            return TRUE;
        case IDC_PLUSMINUS:
            wcscpy(tmp, L"-");
            wcscat(tmp, str[mode]);
            wcscpy(str[mode], tmp);
            Edit_SetText(hEWnd, str[mode]);
            return TRUE;
        case IDC_EQUAL:
            switch (op) {
            case plus:
                ans = _wtof(str[0]) + _wtof(str[1]);
                break;
            case minus:
                ans = _wtof(str[0]) - _wtof(str[1]);
                break;
            case kake:
                ans = _wtof(str[0]) * _wtof(str[1]);
                break;
            case wari:
                ans = _wtof(str[0]) / _wtof(str[1]);
                break;
            }
            swprintf(ans_str, L"%.10f", ans);
            Edit_SetText(hEWnd, ans_str);
            memset(str, '\0', sizeof(str));
            mode = 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}
