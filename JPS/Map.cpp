#include "Map.h"


HBRUSH g_startBrush;
HBRUSH g_endBrush;
HBRUSH g_openBrush;
HBRUSH g_closeBrush;
HBRUSH g_findBrush;
HBRUSH g_hTileBrush;
HBRUSH g_searchBrush;
HBRUSH g_lineBrush;
HPEN   g_hGridPen;
HPEN   g_LinePen;

int GRID_SIZE = 16;
char g_Tile[GRID_HEIGHT][GRID_WIDTH];