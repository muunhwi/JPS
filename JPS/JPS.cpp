#include <windowsx.h>
#include <windows.h>
#include "Resource.h"
#include "JumpPointSearch.h"
#include "Map.h"
#include "Line.h"


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HWND hWnd;
HDC g_MemDc;
RECT g_memDc_rect;
HBITMAP g_MemDc_bitMap;
HBITMAP g_MemDc_bitMap_old;

bool g_bErase = false;
bool g_bDrag = false;
TileProperty g_mode = BLOCK;
JumpPointSearch g_jps;


Point g_start{ -1,-1 };
Point g_end{-1,-1};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    MSG msg;
    

    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"JPS";
    wcex.hIconSm = nullptr;
    RegisterClassEx(&wcex);

    hWnd = CreateWindow
    (
        L"JPS",
        L"JPS",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        500,
        500,
        nullptr,

        nullptr,
        hInstance,
        nullptr
    );

    if (!hWnd) return false;
   
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

void RenderGrid(HDC hdc)
{
    int iX = 0;
    int iY = 0;

    HPEN hOldPen = (HPEN)SelectObject(hdc, g_hGridPen);

    for (int iCntW = 0; iCntW <= GRID_WIDTH; iCntW++)
    {
        MoveToEx(hdc, iX, 0, nullptr);
        LineTo(hdc, iX, GRID_HEIGHT * GRID_SIZE);
        iX += GRID_SIZE;
    }

    for (int i = 0; i <= GRID_HEIGHT; ++i)
    {
        MoveToEx(hdc, 0, iY, nullptr);
        LineTo(hdc, GRID_WIDTH * GRID_SIZE, iY);
        iY += GRID_SIZE;
    }
    SelectObject(hdc, hOldPen);
}
void RenderTile(HDC hdc)
{
    int iX = 0;
    int iY = 0;
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, g_startBrush);
    SelectObject(hdc, GetStockObject(NULL_PEN));

    for (int row{ 0 }; row < GRID_HEIGHT; ++row)
    {
        for (int col{ 0 }; col < GRID_WIDTH; ++col)
        {
            if (g_Tile[row][col])
            {
                HBRUSH htempBrush = nullptr;
                switch (g_Tile[row][col])
                {
                case BLOCK:
                    htempBrush = g_hTileBrush;
                    break;
                case START:
                    htempBrush = g_startBrush;
                    break;
                case END:
                    htempBrush = g_endBrush;
                    break;
                case OPEN:
                    htempBrush = g_openBrush;
                    break;
                case CLOSE:
                    htempBrush = g_closeBrush;
                    break;
                case FIND:
                    htempBrush = g_findBrush;
                    break;
                case SEARCH:
                    htempBrush = g_searchBrush;
                    break;
                case LINE:
                    htempBrush = g_lineBrush;
                    break;
                }

                if (htempBrush)
                {
                    SelectObject(hdc, htempBrush);
                    iX = col * GRID_SIZE;
                    iY = row * GRID_SIZE;
                    Rectangle(hdc, iX, iY, iX + GRID_SIZE + 1, iY + GRID_SIZE + 1);
                }
            }
        }
    }
    SelectObject(hdc, hOldBrush);
}
void RenderText(HDC hdc)
{
    const wchar_t* one = L"KEY 1 START POINT MODE";
    const wchar_t* two = L"KEY 2 END POINT MODE";
    const wchar_t* three = L"KEY 3 SEARCH";
    const wchar_t* four = L"KEY 4 DELETE PATH";
    const wchar_t* five = L"KEY 5 CLEAR ALL";
    const wchar_t* six = L"KEY 6 LINE";

    TextOut(hdc, GRID_WIDTH / 2 * GRID_SIZE, (GRID_HEIGHT + 2) * GRID_SIZE, one, wcslen(one));
    TextOut(hdc, GRID_WIDTH / 2 * GRID_SIZE, (GRID_HEIGHT + 3) * GRID_SIZE, two, wcslen(two));
    TextOut(hdc, GRID_WIDTH / 2 * GRID_SIZE, (GRID_HEIGHT + 4) * GRID_SIZE, three, wcslen(three));
    TextOut(hdc, GRID_WIDTH / 2 * GRID_SIZE, (GRID_HEIGHT + 5) * GRID_SIZE, four, wcslen(four));
    TextOut(hdc, GRID_WIDTH / 2 * GRID_SIZE, (GRID_HEIGHT + 6) * GRID_SIZE, five, wcslen(five));
    TextOut(hdc, GRID_WIDTH / 2 * GRID_SIZE, (GRID_HEIGHT + 7) * GRID_SIZE, six, wcslen(six));

}
void RenderLine(HDC hdc)
{
    if (g_jps.isFind())
    {
        HPEN hOldPen = (HPEN)SelectObject(hdc, g_LinePen);
        Node* cur = g_jps.getFind();

        int mid = GRID_SIZE / 2;
        while (cur->_parent)
        {

            Point p1 = cur->_point;
            Point p2 = cur->_parent->_point;

            g_Tile[p1._ypos][p1._xpos] = FIND;
            MoveToEx(hdc, p1._xpos * GRID_SIZE + mid, p1._ypos * GRID_SIZE + mid, nullptr);
            LineTo(hdc, p2._xpos * GRID_SIZE + mid, p2._ypos * GRID_SIZE + mid);
            cur = cur->_parent;
        }

        SelectObject(hdc, hOldPen);
    }

}
   
void initMemDC(HWND& hWnd)
{
    HDC hDC = GetDC(hWnd);
    GetClientRect(hWnd, &g_memDc_rect);
    g_MemDc_bitMap = CreateCompatibleBitmap(hDC, g_memDc_rect.right, g_memDc_rect.bottom);
    g_MemDc = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    g_MemDc_bitMap_old = (HBITMAP)SelectObject(g_MemDc, g_MemDc_bitMap);
}
void deleteMemDC()
{
    SelectObject(g_MemDc, g_MemDc_bitMap_old);
    DeleteObject(g_MemDc_bitMap);
    DeleteObject(g_MemDc);
}
void Render()
{

    PatBlt(g_MemDc, 0, 0, g_memDc_rect.right, g_memDc_rect.bottom, WHITENESS);
    RenderTile(g_MemDc);
    RenderText(g_MemDc);
    RenderGrid(g_MemDc);
    RenderLine(g_MemDc);

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    BitBlt(hdc, 0, 0, g_memDc_rect.right, g_memDc_rect.bottom, g_MemDc, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    InvalidateRect(hWnd, nullptr, false);
        
 
    //Sleep(10);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{

    switch (message)
    {
        case WM_LBUTTONDOWN:
        {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            int iTileX = xPos / GRID_SIZE;
            int iTileY = yPos / GRID_SIZE;

            g_bDrag = true;
            if (g_Tile[iTileY][iTileX] == BLOCK)
                g_bErase = true;
            else
                g_bErase = false;

            break;
        }
        case WM_LBUTTONUP:
        {
            g_bDrag = false;
            break;
        }
        case WM_MOUSEMOVE:
        {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);

            int iTileX = xPos / GRID_SIZE;
            int iTileY = yPos / GRID_SIZE;

            if (iTileX > GRID_WIDTH - 1 || iTileY > GRID_HEIGHT - 1)
                break;

            if (g_bDrag)
            {
                switch (g_mode)
                {
                case BLOCK:
                {
                    if (g_Tile[iTileY][iTileX] < 2)
                    {
                        g_Tile[iTileY][iTileX] = !g_bErase;
                        InvalidateRect(hWnd, nullptr, false);
                    }
                    break;
                }
                case START:
                {
                    if (g_start._xpos > -1 && g_start._ypos > -1)
                        g_Tile[g_start._ypos][g_start._xpos] = EMPTY;

                    g_start._xpos = iTileX;
                    g_start._ypos = iTileY;
                    g_Tile[g_start._ypos][g_start._xpos] = START;
                    g_mode = BLOCK;
                    InvalidateRect(hWnd, nullptr, false);
                    break;
                }
                case END:
                {
                    if (g_end._xpos > -1 && g_end._ypos > -1)
                        g_Tile[g_end._ypos][g_end._xpos] = EMPTY;

                    g_end._xpos = iTileX;
                    g_end._ypos = iTileY;
                    g_Tile[g_end._ypos][g_end._xpos] = END;
                    g_mode = BLOCK;
                    InvalidateRect(hWnd, nullptr, false);
                    break;
                }
                }
            }
            break;
        }
        case WM_SIZE:
        {
            deleteMemDC();
            initMemDC(hWnd);
            break;
        }
        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case L'1':
                {
                    g_mode = START;
                    break;
                }
                case L'2':
                {
                    g_mode = END;
                    break;
                }
                case L'3': // search
                {

                    if (g_start._xpos == -1 && g_start._ypos == -1)
                        break;

                    if (g_end._xpos == -1 && g_end._ypos == -1)
                        break;

                    g_jps.init(g_start, g_end);
                    g_jps.search();

                    InvalidateRect(hWnd, nullptr, false);
                    break;
                }
                case L'4': // path delete
                {
                    g_jps.clearPath(false);
                    InvalidateRect(hWnd, nullptr, false);
                    break;
                }
                case L'5': // clear all
                {
                    g_start._xpos = -1;
                    g_start._ypos = -1;

                    g_end._xpos = -1;
                    g_end._ypos = -1;
                    g_jps.clearPath(true);
                    
                    InvalidateRect(hWnd, nullptr, false);
                    break;
                }
                case L'6':
                {
                    if (g_start._xpos == -1 && g_start._ypos == -1)
                        break;

                    if (g_end._xpos == -1 && g_end._ypos == -1)
                        break;

                    Line line;
                    line.linePoint(g_start._xpos, g_start._ypos, g_end._xpos, g_end._ypos);
                    auto points = line.getPoints();

                    for (auto pair : points)
                    {
                        g_Tile[pair.second][pair.first] = LINE;
                    }
                    points.clear();
                    InvalidateRect(hWnd, nullptr, false);

                }
            }
            break;
        }
        case WM_PAINT:
        {
            Render();
            break;
        }
        case WM_CREATE:
        {
            g_hGridPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
            g_LinePen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));

            g_hTileBrush = CreateSolidBrush(RGB(100, 100, 100));
            g_lineBrush = CreateSolidBrush(RGB(203, 100, 230));
            g_startBrush = CreateSolidBrush(RGB(215, 55, 15));
            g_endBrush = CreateSolidBrush(RGB(15, 215, 165));
            g_openBrush = CreateSolidBrush(RGB(215, 208, 15));
            g_closeBrush = CreateSolidBrush(RGB(215, 150, 15));
            g_findBrush = CreateSolidBrush(RGB(102, 178, 255));
            g_searchBrush = CreateSolidBrush(RGB(207, 123, 221));
            initMemDC(hWnd);
            break;
        }
        case WM_DESTROY:
        {

            DeleteObject(g_hTileBrush);
            DeleteObject(g_searchBrush);
            DeleteObject(g_findBrush);
            DeleteObject(g_hGridPen);
            DeleteObject(g_startBrush);
            DeleteObject(g_endBrush);
            DeleteObject(g_openBrush);
            DeleteObject(g_closeBrush);
            DeleteObject(g_lineBrush);
            DeleteObject(g_LinePen);
            deleteMemDC();
            PostQuitMessage(0);
            break;
        }
        default:
        {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    
    }
    return 0;
}

