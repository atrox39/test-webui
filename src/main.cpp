#define WEBVIEW_IMPLEMENTATION
#include <windows.h>
#include <windowsx.h>
#include <string>
#include <algorithm>
#include "api.h"
#include "webview.h"

static const wchar_t *kPrevWndProcProp = L"TestWebUI_PrevWndProc";

static void CenterWindowOnWorkArea(HWND hwnd)
{
  RECT rcWindow{};
  if (!GetWindowRect(hwnd, &rcWindow))
  {
    return;
  }

  MONITORINFO mi{};
  mi.cbSize = sizeof(mi);
  if (!GetMonitorInfoW(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi))
  {
    return;
  }

  auto winW = rcWindow.right - rcWindow.left;
  auto winH = rcWindow.bottom - rcWindow.top;

  auto rcWork = mi.rcWork;
  auto workW = rcWork.right - rcWork.left;
  auto workH = rcWork.bottom - rcWork.top;

  auto x = rcWork.left + (workW - winW) / 2;
  auto y = rcWork.top + (workH - winH) / 2;

  SetWindowPos(hwnd, nullptr, x, y, 0, 0,
               SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

static LRESULT CALLBACK FrameLessWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
  auto prev = reinterpret_cast<WNDPROC>(GetPropW(hwnd, kPrevWndProcProp));

  if (msg == WM_NCCALCSIZE && wp)
  {
    return 0;
  }

  if (msg == WM_GETMINMAXINFO)
  {
    auto mmi = reinterpret_cast<MINMAXINFO *>(lp);
    MONITORINFO mi{};
    mi.cbSize = sizeof(mi);
    if (GetMonitorInfoW(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi))
    {
      auto rcWork = mi.rcWork;
      auto rcMonitor = mi.rcMonitor;
      mmi->ptMaxPosition.x = rcWork.left - rcMonitor.left;
      mmi->ptMaxPosition.y = rcWork.top - rcMonitor.top;
      mmi->ptMaxSize.x = rcWork.right - rcWork.left;
      mmi->ptMaxSize.y = rcWork.bottom - rcWork.top;
    }
    if (prev)
    {
      return CallWindowProcW(prev, hwnd, msg, wp, lp);
    }
    return 0;
  }

  if (msg == WM_NCHITTEST)
  {
    POINT p{GET_X_LPARAM(lp), GET_Y_LPARAM(lp)};
    ScreenToClient(hwnd, &p);

    RECT rc{};
    GetClientRect(hwnd, &rc);

    auto frameX = GetSystemMetrics(SM_CXSIZEFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
    auto frameY = GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);

    auto onLeft = p.x < frameX;
    auto onRight = p.x >= (rc.right - frameX);
    auto onTop = p.y < frameY;
    auto onBottom = p.y >= (rc.bottom - frameY);

    if (onTop && onLeft)
      return HTTOPLEFT;
    if (onTop && onRight)
      return HTTOPRIGHT;
    if (onBottom && onLeft)
      return HTBOTTOMLEFT;
    if (onBottom && onRight)
      return HTBOTTOMRIGHT;
    if (onLeft)
      return HTLEFT;
    if (onRight)
      return HTRIGHT;
    if (onTop)
      return HTTOP;
    if (onBottom)
      return HTBOTTOM;
  }

  if (msg == WM_NCDESTROY)
  {
    if (prev)
    {
      auto res = CallWindowProcW(prev, hwnd, msg, wp, lp);
      RemovePropW(hwnd, kPrevWndProcProp);
      return res;
    }
    RemovePropW(hwnd, kPrevWndProcProp);
  }

  if (prev)
  {
    return CallWindowProcW(prev, hwnd, msg, wp, lp);
  }
  return DefWindowProcW(hwnd, msg, wp, lp);
}

std::string GetExeDirectory()
{
  char buffer[MAX_PATH];
  GetModuleFileNameA(NULL, buffer, MAX_PATH);
  std::string path(buffer);

  std::string::size_type pos = path.find_last_of("\\/");
  return path.substr(0, pos);
}

static std::string MakeUiPath()
{
  auto exeDir = GetExeDirectory();
  auto uiPath = std::string{"file:///"} + exeDir + "/ui/index.html";
  std::replace(uiPath.begin(), uiPath.end(), '\\', '/');
  return uiPath;
}

static HWND MakeFramelessAndGetHwnd(webview::webview &w)
{
  auto win = w.window();
  if (!win.ok())
  {
    return nullptr;
  }

  auto hwnd = static_cast<HWND>(win.value());
  auto style = GetWindowLongPtrW(hwnd, GWL_STYLE);
  style &= ~static_cast<LONG_PTR>(WS_OVERLAPPEDWINDOW);
  style |= static_cast<LONG_PTR>(WS_POPUP | WS_THICKFRAME | WS_MINIMIZEBOX |
                                 WS_MAXIMIZEBOX);
  SetWindowLongPtrW(hwnd, GWL_STYLE, style);
  auto exstyle = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
  exstyle &= ~static_cast<LONG_PTR>(WS_EX_CLIENTEDGE);
  SetWindowLongPtrW(hwnd, GWL_EXSTYLE, exstyle);

  auto prev = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(
      hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(FrameLessWndProc)));
  if (prev)
  {
    SetPropW(hwnd, kPrevWndProcProp, reinterpret_cast<HANDLE>(prev));
  }

  SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
               SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE |
                   SWP_FRAMECHANGED);
  return hwnd;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  auto uiPath = MakeUiPath();

#ifdef DEBUG
  webview::webview w(true, nullptr);
#else
  webview::webview w(false, nullptr);
#endif

  auto hwnd = MakeFramelessAndGetHwnd(w);

  w.set_title("Test WebUI");
  w.set_size(1024, 768, WEBVIEW_HINT_NONE);
  if (hwnd)
  {
    CenterWindowOnWorkArea(hwnd);
  }

  RegisterNativeAPI(w);

#ifdef DEBUG
  w.navigate("http://localhost:5173");
#else
  w.navigate(uiPath);
#endif

  w.run();
  return 0;
}
