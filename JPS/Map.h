#pragma once
#include <windows.h>

#define GRID_WIDTH 100
#define GRID_HEIGHT 50

extern HBRUSH g_startBrush;
extern HBRUSH g_endBrush;
extern HBRUSH g_openBrush;
extern HBRUSH g_closeBrush;
extern HBRUSH g_findBrush;
extern HBRUSH g_hTileBrush;
extern HBRUSH g_searchBrush;
extern HBRUSH g_lineBrush;
extern HPEN   g_hGridPen;
extern HPEN   g_LinePen;

extern int GRID_SIZE;
extern char g_Tile[GRID_HEIGHT][GRID_WIDTH];

enum TileProperty
{
    EMPTY,
    BLOCK,
    START,
    END,
    OPEN,
    CLOSE,
    FIND,
    SEARCH,
    LINE
};




