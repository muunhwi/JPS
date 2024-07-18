#include <cmath>
#include "JumpPointSearch.h"
#include "Map.h"

void Render();

bool  JumpPointSearch::check(Node* parent, int col, int row, Direction fromParent)
{
    if (g_Tile[row][col] == BLOCK)
        return false;

    if (g_Tile[row][col] == CLOSE)
        return false;

    if (g_Tile[row][col] == OPEN)
    {
        Point newPoint = { col, row };
        double G = parent->_G + getG(parent->_point, newPoint);

        if (G < _openListMap[newPoint]->_G)
        {
            _openListMap[newPoint]->_G = G;
            _openListMap[newPoint]->_F = G + _openListMap[newPoint]->_H;
            _openListMap[newPoint]->_parent = parent;
            _openListMap[newPoint]->_fromParent = fromParent;
        }

        return false;
    }

    return true;
}
Node* JumpPointSearch::createNode(Node* parent, Point newPoint, Direction reason, Direction fromParent )
{
    double G, H;
    G = parent->_G + getG(parent->_point, newPoint);
    H = getH(newPoint, _end);
    return new Node{ newPoint, parent, G, H, G + H , reason, fromParent };
}
void  JumpPointSearch::findEnd(Node* parent, int col, int row)
{
    Point newPoint = { col, row };
    Node* node = createNode(parent, newPoint);

    _openList.push(node);
    _openListMap.insert(std::make_pair(newPoint, node));
}
void  JumpPointSearch::openListPush(Node* parent, int col, int row, Direction reason, Direction fromParent)
{
    Point newPoint = { col, row };
    Node* node = createNode(parent, newPoint, reason, fromParent);

    _openList.push(node);
    _openListMap.insert(std::make_pair(newPoint, node));
    g_Tile[row][col] = OPEN;
}


void  JumpPointSearch::checkUP(Node* parent, int col, int row)
{
    bool RU = false;
    bool LU = false;
    for (; row > -1; --row)
    {
        if (g_Tile[row][col] == END)
        {
            findEnd(parent, col, row);
            return;
        }

        if (!check(parent, col, row, UP))
            return;

        if (row == 0 || g_Tile[row - 1][col] == BLOCK)
        {
            g_Tile[row][col] = SEARCH;
            return;
        }

        if (col == 0)
        {
            if (g_Tile[row][col + 1] == BLOCK &&
                g_Tile[row - 1][col + 1] != BLOCK)
            {
                openListPush(parent, col, row, D_UP_RIGHT, UP);
                return;
            }

        }
        else if (col == GRID_WIDTH - 1)
        {
            if (g_Tile[row][col - 1] == BLOCK &&
                g_Tile[row - 1][col - 1] != BLOCK)
            {
                openListPush(parent, col, row, D_UP_LEFT, UP);
                return;
            }
        }
        else
        {

            if (g_Tile[row][col - 1] == BLOCK &&
                g_Tile[row - 1][col - 1] != BLOCK)
                LU = true;

            if (g_Tile[row][col + 1] == BLOCK &&
                g_Tile[row - 1][col + 1] != BLOCK)
                RU = true;

            if (RU || LU)
            {
                Direction d;

                if (RU && LU)
                    d = UP;
                else if (RU)
                    d = D_UP_RIGHT;
                else
                    d = D_UP_LEFT;

                openListPush(parent, col, row, d, UP);
                return;
            }
        }

        g_Tile[row][col] = SEARCH;
       // Render();
    }
}
void  JumpPointSearch::checkDOWN(Node* parent, int col, int row)
{
    bool LD = false;
    bool RD = false;

    for (; row < GRID_HEIGHT; ++row)
    {

        if (g_Tile[row][col] == END)
        {
            findEnd(parent, col, row);
            return;
        }

        if (!check(parent, col, row, DOWN))
            return;

        if (row == GRID_HEIGHT - 1 || g_Tile[row + 1][col] == BLOCK)
        {
            g_Tile[row][col] = SEARCH;
            return;
        }

        if (col == 0)
        {
            if (g_Tile[row][col + 1] == BLOCK &&
                g_Tile[row + 1][col + 1] != BLOCK)
            {

                openListPush(parent, col, row, D_DOWN_RIGHT, DOWN);
                return;
            }

        }
        else if (col == GRID_WIDTH - 1)
        {
            if (g_Tile[row][col - 1] == BLOCK &&
                g_Tile[row + 1][col - 1] != BLOCK)
            {
                openListPush(parent, col, row, D_DOWN_LEFT, DOWN);
                return;
            }
        }
        else
        {

            if (g_Tile[row][col - 1] == BLOCK &&
                g_Tile[row + 1][col - 1] != BLOCK)
                LD = true;


            if (g_Tile[row][col + 1] == BLOCK &&
                g_Tile[row + 1][col + 1] != BLOCK)
                RD = true;


            if (LD || RD)
            {
                Direction d;

                if (LD && RD)
                    d = DOWN;
                else if (LD)
                    d = D_DOWN_LEFT;
                else
                    d = D_DOWN_RIGHT;

                openListPush(parent, col, row, d, DOWN);
                return;
            }
        }

        g_Tile[row][col] = SEARCH;
      //  Render();
    }
}
void  JumpPointSearch::checkRIGHT(Node* parent, int col, int row)
{
    bool RU = false;
    bool RD = false;
    for (; col < GRID_WIDTH; ++col)
    {

        if (g_Tile[row][col] == END)
        {
            findEnd(parent, col, row);
            return;
        }

        if (!check(parent, col, row, RIGHT))
            return;

        if (col == GRID_WIDTH - 1 || g_Tile[row][col + 1] == BLOCK)
        {
            g_Tile[row][col] = SEARCH;
            return;
        }

        if (row == 0)
        {
            if (g_Tile[row + 1][col] == BLOCK &&
                g_Tile[row + 1][col + 1] != BLOCK)
            {
                openListPush(parent, col, row, D_DOWN_RIGHT, RIGHT);
                return;
            }
        }
        else if (row == GRID_HEIGHT - 1)
        {
            if (g_Tile[row - 1][col] == BLOCK &&
                g_Tile[row - 1][col + 1] != BLOCK)
            {
                openListPush(parent, col, row, D_UP_RIGHT, RIGHT);
                return;
            }
        }
        else
        {

            if (g_Tile[row + 1][col] == BLOCK &&
                g_Tile[row + 1][col + 1] != BLOCK)
                RD = true;

            if (g_Tile[row - 1][col] == BLOCK &&
                g_Tile[row - 1][col + 1] != BLOCK)
                RU = true;

            if (RU || RD)
            {
                Direction d;

                if (RU && RD)
                    d = RIGHT;
                else if (RU)
                    d = D_UP_RIGHT;
                else
                    d = D_DOWN_RIGHT;

                openListPush(parent, col, row, d, RIGHT);
                return;
            }
        }

        g_Tile[row][col] = SEARCH;
       // Render();
    }

}
void  JumpPointSearch::checkLEFT(Node* parent, int col, int row)
{

    bool LU = false;
    bool LD = false;

    for (; col > -1; --col)
    {

        if (g_Tile[row][col] == END)
        {
            findEnd(parent, col, row);
            return;
        }

        if (!check(parent, col, row, LEFT))
            return;

        if (col == 0 || g_Tile[row][col - 1] == BLOCK)
        {
            g_Tile[row][col] = SEARCH;
            return;
        }

        if (row == 0)
        {
            if (g_Tile[row + 1][col] == BLOCK &&
                g_Tile[row + 1][col - 1] != BLOCK)
            {
                openListPush(parent, col, row, D_DOWN_LEFT, LEFT);
                return;
            }

        }
        else if (row == GRID_HEIGHT - 1)
        {
            if (g_Tile[row - 1][col] == BLOCK &&
                g_Tile[row - 1][col - 1] != BLOCK)
            {
                openListPush(parent, col, row, D_UP_LEFT, LEFT);
                return;
            }
        }
        else
        {
            if (g_Tile[row + 1][col] == BLOCK &&
                g_Tile[row + 1][col - 1] != BLOCK)
                LD = true;

            if (g_Tile[row - 1][col] == BLOCK &&
                g_Tile[row - 1][col - 1] != BLOCK)
                LU = true;

            if (LD || LU)
            {
                Direction d;

                if (LD && LU)
                    d = LEFT;
                else if (LD)
                    d = D_DOWN_LEFT;
                else
                    d = D_UP_LEFT;

                openListPush(parent, col, row, d, LEFT);
                return;
            }

        }

        g_Tile[row][col] = SEARCH;
       //Render();
    }

}

void  JumpPointSearch::checkD_UP_RIGHT(Node* parent, int col, int row)
{
    for (; row > -1 && col < GRID_WIDTH; --row, ++col)
    {
        if (g_Tile[row][col] == END)
        {
            findEnd(parent, col, row);
            return;
        }

        if (!check(parent, col, row, D_UP_RIGHT))
            return;


        if (row == 0 || col == GRID_WIDTH - 1)
        {
            if (row == 0 && col == GRID_WIDTH - 1)
            {
                g_Tile[row][col] = SEARCH;
                return;
            }
            else if (row == 0)
            {
                if (g_Tile[row + 1][col] == BLOCK &&
                    g_Tile[row + 1][col + 1] != BLOCK)
                {
                    openListPush(parent, col, row, DOWN, D_UP_RIGHT);
                    return;
                }
            }
            else
            {
                if (g_Tile[row][col - 1] == BLOCK &&
                    g_Tile[row + 1][col - 1] != BLOCK)
                {
                    openListPush(parent, col, row, LEFT, D_UP_RIGHT);
                    return;
                }

            }
        }
        else
        {
            if (g_Tile[row - 1][col] == BLOCK &&
                g_Tile[row][col + 1] == BLOCK)
            {
                g_Tile[row][col] = SEARCH;
                return;
            }

            if (g_Tile[row + 1][col] == BLOCK &&
                g_Tile[row + 1][col + 1] != BLOCK)
            {
                openListPush(parent, col, row, DOWN, D_UP_RIGHT);
                return;
            }


            if (g_Tile[row][col - 1] == BLOCK &&
                g_Tile[row - 1][col - 1] != BLOCK)
            {
                openListPush(parent, col, row, LEFT, D_UP_RIGHT);
                return;
            }
        }

        g_Tile[row][col] = SEARCH;
       // Render();

        int searchRow{ row - 1 };
        int searchCol{ col + 1 };


        for (; searchRow > -1; --searchRow)
        {

            if (g_Tile[searchRow][col] == END)
            {
                Node* node = createNode(parent, { col, row });
                _closeList.push_back(node);
                findEnd(node, col, searchRow);
                return;
            }

            if (!check(parent, col, searchRow, D_UP_RIGHT))
                break;

            if (searchRow == 0 || g_Tile[searchRow - 1][col] == BLOCK)
            {
                g_Tile[searchRow][col] = SEARCH;
                break;
            }

            if (col == GRID_WIDTH - 1)
            {
                if (g_Tile[searchRow][col - 1] == BLOCK &&
                    g_Tile[searchRow - 1][col - 1] != BLOCK)
                {
                    openListPush(parent, col, row, UP, D_UP_RIGHT);
                    return;
                }

            }
            else
            {
                if (g_Tile[searchRow][col - 1] == BLOCK &&
                    g_Tile[searchRow - 1][col - 1] != BLOCK)
                {
                    openListPush(parent, col, row, UP, D_UP_RIGHT);
                    return;
                }


                if (g_Tile[searchRow][col + 1] == BLOCK &&
                    g_Tile[searchRow - 1][col + 1] != BLOCK)
                {
                    openListPush(parent, col, row, UP, D_UP_RIGHT);
                    return;
                }
            }

            g_Tile[searchRow][col] = SEARCH;
            //Render();
        }


        for (; searchCol < GRID_WIDTH; ++searchCol)
        {

            if (g_Tile[row][searchCol] == END)
            {
                Node* node = createNode(parent, { col, row });
                _closeList.push_back(node);
                findEnd(node, searchCol, row);
                return;
            }

            if (!check(parent, searchCol, row, D_UP_RIGHT))
                break;


            if (searchCol == GRID_WIDTH - 1 || g_Tile[row][searchCol + 1] == BLOCK)
            {
                g_Tile[row][searchCol] = SEARCH;
                break;
            }

            if (row == 0)
            {
                if (g_Tile[row + 1][searchCol] == BLOCK &&
                    g_Tile[row + 1][searchCol + 1] != BLOCK)
                {
                    openListPush(parent, col, row, RIGHT, D_UP_RIGHT);
                    return;
                }
            }
            else if (row == GRID_HEIGHT - 1)
            {
                if (g_Tile[row - 1][searchCol] == BLOCK &&
                    g_Tile[row - 1][searchCol + 1] != BLOCK)
                {
                    openListPush(parent, col, row, RIGHT, D_UP_RIGHT);
                    return;
                }
            }
            else
            {

                if (g_Tile[row + 1][searchCol] == BLOCK &&
                    g_Tile[row + 1][searchCol + 1] != BLOCK)
                {
                    openListPush(parent, col, row, RIGHT, D_UP_RIGHT);
                    return;
                }

                if (g_Tile[row - 1][searchCol] == BLOCK &&
                    g_Tile[row - 1][searchCol + 1] != BLOCK)
                {
                    openListPush(parent, col, row, RIGHT, D_UP_RIGHT);
                    return;
                }

            }

            g_Tile[row][searchCol] = SEARCH;
            //Render();
        }

    }

}
void  JumpPointSearch::checkD_DOWN_RIGHT(Node* parent, int col, int row)
{
    for (; row < GRID_HEIGHT && col < GRID_WIDTH; ++row, ++col)
    {

        if (g_Tile[row][col] == END)
        {
            findEnd(parent, col, row);
            return;
        }

        if (!check(parent, col, row, D_DOWN_RIGHT))
            return;

        if (row == GRID_HEIGHT - 1 || col == GRID_WIDTH - 1)
        {
            if (row == GRID_HEIGHT - 1 && col == GRID_WIDTH - 1)
            {
                g_Tile[row][col] = SEARCH;
                return;
            }
            else if (row == GRID_HEIGHT - 1)
            {
                if (g_Tile[row - 1][col] == BLOCK &&
                    g_Tile[row - 1][col + 1] != BLOCK)
                {
                    openListPush(parent, col, row, UP, D_DOWN_RIGHT);
                    return;
                }
            }
            else
            {
                if (g_Tile[row][col - 1] == BLOCK &&
                    g_Tile[row + 1][col - 1] != BLOCK)
                {
                    openListPush(parent, col, row, LEFT, D_DOWN_RIGHT);
                    return;
                }
            }
        }
        else
        {
            if (g_Tile[row + 1][col] == BLOCK && g_Tile[row][col + 1] == BLOCK)
            {
                g_Tile[row][col] = SEARCH;
                return;
            }

            if (g_Tile[row - 1][col] == BLOCK &&
                g_Tile[row - 1][col + 1] != BLOCK)
            {
                openListPush(parent, col, row, UP, D_DOWN_RIGHT);
                return;
            }

            if (g_Tile[row][col - 1] == BLOCK &&
                g_Tile[row + 1][col - 1] != BLOCK)
            {
                openListPush(parent, col, row, LEFT, D_DOWN_RIGHT);
                return;
            }
        }

        g_Tile[row][col] = SEARCH;
       // Render();

        int searchRow{ row + 1 };
        int searchCol{ col + 1 };
        for (; searchRow < GRID_HEIGHT; ++searchRow)
        {

            if (g_Tile[searchRow][col] == END)
            {
                Node* node = createNode(parent, { col, row });
                _closeList.push_back(node);
                findEnd(node, col, searchRow);
                return;
            }

            if (!check(parent, col, searchRow, D_DOWN_RIGHT))
                break;

            if (searchRow == GRID_HEIGHT - 1 || g_Tile[searchRow + 1][col] == BLOCK)
            {
                g_Tile[searchRow][col] = SEARCH;
                break;
            }

            if (col == 0)
            {
                if (g_Tile[searchRow][col + 1] == BLOCK &&
                    g_Tile[searchRow + 1][col + 1] != BLOCK)
                {

                    openListPush(parent, col, row, DOWN, D_DOWN_RIGHT);
                    return;
                }

            }
            else if (col == GRID_WIDTH - 1)
            {
                if (g_Tile[searchRow][col - 1] == BLOCK &&
                    g_Tile[searchRow + 1][col - 1] != BLOCK)
                {
                    openListPush(parent, col, row, DOWN, D_DOWN_RIGHT);
                    return;
                }
            }
            else
            {

                if (g_Tile[searchRow][col - 1] == BLOCK &&
                    g_Tile[searchRow + 1][col - 1] != BLOCK)
                {
                    openListPush(parent, col, row, DOWN, D_DOWN_RIGHT);
                    return;
                }


                if (g_Tile[searchRow][col + 1] == BLOCK &&
                    g_Tile[searchRow + 1][col + 1] != BLOCK)
                {
                    openListPush(parent, col, row, DOWN, D_DOWN_RIGHT);
                    return;
                }

            }

            g_Tile[searchRow][col] = SEARCH;
           // Render();
        }

        for (; searchCol < GRID_WIDTH; ++searchCol)
        {

            if (g_Tile[row][searchCol] == END)
            {
                Node* node = createNode(parent, { col, row });
                _closeList.push_back(node);
                findEnd(node, searchCol, row);
                return;
            }

            if (!check(parent, searchCol, row, D_DOWN_RIGHT))
                break;

            if (searchCol == GRID_WIDTH - 1 || g_Tile[row][searchCol + 1] == BLOCK)
            {
                g_Tile[row][searchCol] = SEARCH;
                break;
            }

            if (row == 0)
            {
                if (g_Tile[row + 1][searchCol] == BLOCK &&
                    g_Tile[row + 1][searchCol + 1] != BLOCK)
                {
                    openListPush(parent, col, row, RIGHT, D_DOWN_RIGHT);
                    return;
                }
            }
            else if (row == GRID_HEIGHT - 1)
            {
                if (g_Tile[row - 1][searchCol] == BLOCK &&
                    g_Tile[row - 1][searchCol + 1] != BLOCK)
                {
                    openListPush(parent, col, row, RIGHT, D_DOWN_RIGHT);
                    return;
                }
            }
            else
            {

                if (g_Tile[row + 1][searchCol] == BLOCK &&
                    g_Tile[row + 1][searchCol + 1] != BLOCK)
                {
                    openListPush(parent, col, row, RIGHT, D_DOWN_RIGHT);
                    return;
                }

                if (g_Tile[row - 1][searchCol] == BLOCK &&
                    g_Tile[row - 1][searchCol + 1] != BLOCK)
                {
                    openListPush(parent, col, row, RIGHT, D_DOWN_RIGHT);
                    return;
                }

            }

            g_Tile[row][searchCol] = SEARCH;
           // Render();
        }
    }

}
void  JumpPointSearch::checkD_UP_LEFT(Node* parent, int col, int row)
{

    for (; row > -1 && col > -1; --row, --col)
    {

        if (g_Tile[row][col] == END)
        {
            findEnd(parent, col, row);
            return;
        }

        if (!check(parent, col, row, D_UP_LEFT))
            return;

        if (row == 0 || col == 0)
        {
            if (row == 0 && col == 0)
            {
                g_Tile[row][col] = SEARCH;
                return;
            }
            else if (row == 0)
            {
                if (g_Tile[row + 1][col] == BLOCK &&
                    g_Tile[row + 1][col - 1] != BLOCK)
                {
                    openListPush(parent, col, row, DOWN, D_UP_LEFT);
                    return;
                }
            }
            else
            {
                if (g_Tile[row][col + 1] == BLOCK &&
                    g_Tile[row - 1][col + 1] != BLOCK)
                {
                    openListPush(parent, col, row, RIGHT, D_UP_LEFT);
                    return;
                }
            }

        }
        else
        {

            if (g_Tile[row - 1][col] == BLOCK && g_Tile[row][col - 1] == BLOCK)
            {
                g_Tile[row][col] = SEARCH;
                return;
            }

            if (g_Tile[row][col + 1] == BLOCK &&
                g_Tile[row - 1][col + 1] != BLOCK)
            {
                openListPush(parent, col, row, RIGHT, D_UP_LEFT);
                return;
            }

            if (g_Tile[row + 1][col] == BLOCK &&
                g_Tile[row + 1][col - 1] != BLOCK)
            {
                openListPush(parent, col, row, DOWN, D_UP_LEFT);
                return;
            }
        }

        g_Tile[row][col] = SEARCH;
        //Render();

        int searchRow{ row - 1 };
        int searchCol{ col - 1 };

        for (; searchRow > -1; --searchRow)
        {

            if (g_Tile[searchRow][col] == END)
            {
                Node* node = createNode(parent, { col, row });
                _closeList.push_back(node);
                findEnd(node, col, searchRow);
                return;
            }

            if (!check(parent, col, searchRow, D_UP_LEFT))
                break;


            if (searchRow == 0 || g_Tile[searchRow - 1][col] == BLOCK)
            {
                g_Tile[searchRow][col] = SEARCH;
                break;
            }

            if (col == GRID_WIDTH - 1)
            {
                if (g_Tile[searchRow][col - 1] == BLOCK &&
                    g_Tile[searchRow - 1][col - 1] != BLOCK)
                {
                    openListPush(parent, col, row, UP, D_UP_LEFT);
                    return;
                }
            }
            else
            {
                if (g_Tile[searchRow][col - 1] == BLOCK &&
                    g_Tile[searchRow - 1][col - 1] != BLOCK)
                {
                    openListPush(parent, col, row, UP, D_UP_LEFT);
                    return;
                }


                if (g_Tile[searchRow][col + 1] == BLOCK &&
                    g_Tile[searchRow - 1][col + 1] != BLOCK)
                {
                    openListPush(parent, col, row, UP, D_UP_LEFT);
                    return;
                }
            }

            g_Tile[searchRow][col] = SEARCH;
            //Render();
        }

        for (; searchCol > -1; --searchCol)
        {

            if (g_Tile[row][searchCol] == END)
            {
                Node* node = createNode(parent, { col, row });
                _closeList.push_back(node);
                findEnd(node, searchCol, row);
                return;
            }


            if (!check(parent, searchCol, row, D_UP_LEFT))
                break;

            if (searchCol == 0 || g_Tile[row][searchCol - 1] == BLOCK)
            {
                g_Tile[row][searchCol] = SEARCH;
                break;
            }

            if (row == 0)
            {
                if (g_Tile[row + 1][searchCol] == BLOCK &&
                    g_Tile[row + 1][searchCol - 1] != BLOCK)
                {
                    openListPush(parent, col, row, LEFT, D_UP_LEFT);
                    return;
                }
            }
            else if (row == GRID_HEIGHT - 1)
            {
                if (g_Tile[row - 1][searchCol] == BLOCK &&
                    g_Tile[row - 1][searchCol - 1] != BLOCK)
                {
                    openListPush(parent, col, row, LEFT, D_UP_LEFT);
                    return;
                }
            }
            else
            {

                if (g_Tile[row + 1][searchCol] == BLOCK &&
                    g_Tile[row + 1][searchCol - 1] != BLOCK)
                {
                    openListPush(parent, col, row, LEFT, D_UP_LEFT);
                    return;
                }


                if (g_Tile[row - 1][searchCol] == BLOCK &&
                    g_Tile[row - 1][searchCol - 1] != BLOCK)
                {
                    openListPush(parent, col, row, LEFT, D_UP_LEFT);
                    return;
                }

                g_Tile[row][searchCol] = SEARCH;
                //Render();
            }

        }

    }

}
void  JumpPointSearch::checkD_DOWN_LEFT(Node* parent, int col, int row)
{

    for (; row < GRID_HEIGHT && col > -1; ++row, --col)
    {
        if (g_Tile[row][col] == END)
        {
            findEnd(parent, col, row);
            return;
        }

        if (!check(parent, col, row, D_DOWN_LEFT))
            return;

        if (row == GRID_HEIGHT - 1 || col == 0)
        {
            if (row == GRID_HEIGHT - 1 && col == 0)
            {
                g_Tile[row][col] = SEARCH;
                return;
            }
            else if (row == GRID_HEIGHT - 1)
            {
                if (g_Tile[row - 1][col] == BLOCK &&
                    g_Tile[row - 1][col - 1] != BLOCK)
                {
                    openListPush(parent, col, row, UP, D_DOWN_LEFT);
                    return;
                }
            }
            else
            {
                if (g_Tile[row][col + 1] == BLOCK &&
                    g_Tile[row + 1][col + 1] != BLOCK)
                {
                    openListPush(parent, col, row, RIGHT, D_DOWN_LEFT);
                    return;
                }
            }

        }
        else
        {
            if (g_Tile[row + 1][col] == BLOCK &&
                g_Tile[row][col - 1] == BLOCK)
            {
                g_Tile[row][col] = SEARCH;
                return;
            }

            if (g_Tile[row - 1][col] == BLOCK &&
                g_Tile[row - 1][col - 1] != BLOCK)
            {
                openListPush(parent, col, row, UP, D_DOWN_LEFT);
                return;
            }

            if (g_Tile[row][col + 1] == BLOCK &&
                g_Tile[row + 1][col + 1] != BLOCK)
            {
                openListPush(parent, col, row, RIGHT, D_DOWN_LEFT);
                return;
            }
        }

        g_Tile[row][col] = SEARCH;
        //Render();

        int searchRow{ row + 1 };
        int searchCol{ col - 1 };
        for (; searchRow < GRID_HEIGHT; ++searchRow)
        {

            if (g_Tile[searchRow][col] == END)
            {
                Node* node = createNode(parent, { col, row });
                _closeList.push_back(node);
                findEnd(node, col, searchRow);
                return;
            }

            if (!check(parent, col, searchRow, D_DOWN_LEFT))
                break;


            if (searchRow == GRID_HEIGHT - 1 || g_Tile[searchRow + 1][col] == BLOCK)
            {
                g_Tile[searchRow][col] = SEARCH;
                break;
            }

            if (col == 0)
            {
                if (g_Tile[searchRow][col + 1] == BLOCK &&
                    g_Tile[searchRow + 1][col + 1] != BLOCK)
                {
                    openListPush(parent, col, row, DOWN, D_DOWN_LEFT);
                    return;
                }

            }
            else if (col == GRID_WIDTH - 1)
            {
                if (g_Tile[searchRow][col - 1] == BLOCK &&
                    g_Tile[searchRow + 1][col - 1] != BLOCK)
                {
                    openListPush(parent, col, row, DOWN, D_DOWN_LEFT);
                    return;
                }
            }
            else
            {

                if (g_Tile[searchRow][col - 1] == BLOCK &&
                    g_Tile[searchRow + 1][col - 1] != BLOCK)
                {
                    openListPush(parent, col, row, DOWN, D_DOWN_LEFT);
                    return;
                }


                if (g_Tile[searchRow][col + 1] == BLOCK &&
                    g_Tile[searchRow + 1][col + 1] != BLOCK)
                {
                    openListPush(parent, col, row, DOWN, D_DOWN_LEFT);
                    return;
                }

            }

            g_Tile[searchRow][col] = SEARCH;
            //Render();
        }
        for (; searchCol > -1; --searchCol)
        {
            if (g_Tile[row][searchCol] == END)
            {
                Node* node = createNode(parent, { col, row });
                _closeList.push_back(node);
                findEnd(node, searchCol, row);
                return;
            }

            if (!check(parent, searchCol, row, D_DOWN_LEFT))
                break;


            if (searchCol == 0 || g_Tile[row][searchCol - 1] == BLOCK)
            {
                g_Tile[row][searchCol] = SEARCH;
                break;
            }

            if (row == 0)
            {
                if (g_Tile[row + 1][searchCol] == BLOCK &&
                    g_Tile[row + 1][searchCol - 1] != BLOCK)
                {
                    openListPush(parent, col, row, LEFT, D_DOWN_LEFT);
                    return;
                }
            }
            else if (row == GRID_HEIGHT - 1)
            {
                if (g_Tile[row - 1][searchCol] == BLOCK &&
                    g_Tile[row - 1][searchCol - 1] != BLOCK)
                {
                    openListPush(parent, col, row, LEFT, D_DOWN_LEFT);
                    return;
                }
            }
            else
            {

                if (g_Tile[row + 1][searchCol] == BLOCK &&
                    g_Tile[row + 1][searchCol - 1] != BLOCK)
                {
                    openListPush(parent, col, row, LEFT, D_DOWN_LEFT);
                    return;
                }


                if (g_Tile[row - 1][searchCol] == BLOCK &&
                    g_Tile[row - 1][searchCol - 1] != BLOCK)
                {
                    openListPush(parent, col, row, LEFT, D_DOWN_LEFT);
                    return;
                }

                g_Tile[row][searchCol] = SEARCH;
                //Render();
            }

        }

    }

}


void  JumpPointSearch::search()
{
    while (!_openList.empty())
    {
        Node* node = _openList.top();
        _openList.pop();

        if (node->_point == _end)
        {
            _find = node;
            _closeList.push_back(node);
            return;
        }

        _closeList.push_back(node);
        if (g_Tile[node->_point._ypos][node->_point._xpos] == OPEN)
            g_Tile[node->_point._ypos][node->_point._xpos] = CLOSE;

        if (!node->_parent)
        {

            for (size_t i{ 0 }; i < directions.size(); ++i)
            {

                int xpos = node->_point._xpos;
                int ypos = node->_point._ypos;
                int newX = xpos + directions[i]._xpos;
                int newY = ypos + directions[i]._ypos;

                if (newX > GRID_WIDTH - 1 || newX < 0)
                    continue;

                if (newY > GRID_HEIGHT - 1 || newY < 0)
                    continue;

                /*if (g_Tile[newY][newX] == TileProp::BLOCK)
                    continue;

                if (g_Tile[newY][newX] == TileProp::START)
                    continue;

                if (g_Tile[newY][newX] > TileProp::END)
                    continue;*/

                switch (directions[i].direction)
                {
                case UP:
                {
                    checkUP(node, newX, newY);
                    break;
                }
                case DOWN:
                {
                    checkDOWN(node, newX, newY);
                    break;
                }
                case RIGHT:
                {
                    checkRIGHT(node, newX, newY);
                    break;
                }
                case LEFT:
                {
                    checkLEFT(node, newX, newY);
                    break;
                }
                case D_UP_RIGHT:
                {
                    if (!(g_Tile[ypos - 1][xpos] == BLOCK && g_Tile[ypos][xpos + 1] == BLOCK))
                           checkD_UP_RIGHT(node, newX, newY);

                    break;
                }
                case D_UP_LEFT:
                {
                    if (!(g_Tile[ypos - 1][xpos] == BLOCK && g_Tile[ypos][xpos -1] == BLOCK))
                           checkD_UP_LEFT(node, newX, newY);

                    break;
                }
                case D_DOWN_RIGHT:
                {
                    if (!(g_Tile[ypos][xpos +1] == BLOCK && g_Tile[ypos +1][xpos] == BLOCK))
                        checkD_DOWN_RIGHT(node, newX, newY);

                    break;
                }
                case D_DOWN_LEFT:
                {
                    if (!(g_Tile[ypos][xpos - 1] == BLOCK && g_Tile[ypos + 1][xpos] == BLOCK))
                        checkD_DOWN_LEFT(node, newX, newY);

                    break;
                }
                }
            }
        }
        else
        {
            Direction reason = node->_reason;
            int newX = node->_point._xpos;
            int newY = node->_point._ypos;

            switch (node->_fromParent)
            {
                case UP:
                {
                    if (reason == UP)
                    {
                        checkD_UP_RIGHT(node, newX + 1, newY - 1);
                        checkD_UP_LEFT(node, newX - 1, newY - 1);
                    }
                    else if (reason == D_UP_RIGHT)
                        checkD_UP_RIGHT(node, newX + 1, newY - 1);
                    else
                        checkD_UP_LEFT(node, newX - 1, newY - 1);

                    checkUP(node, newX, newY - 1);
                    break;
                }
                case DOWN:
                {
                    if (reason == DOWN)
                    {
                        checkD_DOWN_LEFT(node, newX - 1, newY + 1);
                        checkD_DOWN_RIGHT(node, newX + 1, newY + 1);
                    }
                    else if (reason == D_DOWN_LEFT)
                        checkD_DOWN_LEFT(node, newX - 1, newY + 1);
                    else
                        checkD_DOWN_RIGHT(node, newX + 1, newY + 1);

                    checkDOWN(node, newX, newY + 1);
                    break;
                }
                case RIGHT:
                {
                    if (reason == RIGHT)
                    {
                        checkD_UP_RIGHT(node, newX + 1, newY - 1);
                        checkD_DOWN_RIGHT(node, newX + 1, newY + 1);
                    }
                    else if (reason == D_UP_RIGHT)
                        checkD_UP_RIGHT(node, newX + 1, newY - 1);
                    else
                        checkD_DOWN_RIGHT(node, newX + 1, newY + 1);

                    checkRIGHT(node, newX + 1, newY);
                    break;
                }
                case LEFT:
                {
                    if (reason == LEFT)
                    {
                        checkD_UP_LEFT(node, newX - 1, newY - 1);
                        checkD_DOWN_LEFT(node, newX - 1, newY + 1);
                    }
                    else if (reason == D_UP_LEFT)
                        checkD_UP_LEFT(node, newX - 1, newY - 1);
                    else
                        checkD_DOWN_LEFT(node, newX - 1, newY + 1);

                    checkLEFT(node, newX - 1, newY);
                    break;
                }
                case D_UP_RIGHT:
                {
                    if (reason == LEFT)
                    {
                        if (!(g_Tile[newY - 1][newX] == BLOCK && g_Tile[newY][newX - 1] == BLOCK))
                            checkD_UP_LEFT(node, newX - 1, newY - 1);
                    }
                    else if (reason == DOWN)
                    {
                        if (!(g_Tile[newY][newX + 1] == BLOCK && g_Tile[newY + 1][newX] == BLOCK))
                            checkD_DOWN_RIGHT(node, newX + 1, newY + 1);
                    }

                    checkUP(node, newX, newY - 1);
                    checkRIGHT(node, newX + 1, newY);
                    checkD_UP_RIGHT(node, newX + 1, newY - 1);
                    break;
                }
                case D_UP_LEFT:
                {
                    if (reason == RIGHT)
                    {
                        if (!(g_Tile[newY - 1][newX] == BLOCK && g_Tile[newY][newX + 1] == BLOCK))
                            checkD_UP_RIGHT(node, newX + 1, newY - 1);
                    }
                    else if (reason == DOWN)
                    {
                        if (!(g_Tile[newY][newX - 1] == BLOCK && g_Tile[newY + 1][newX] == BLOCK))
                            checkD_DOWN_LEFT(node, newX - 1, newY + 1);
                    }


                    checkUP(node, newX, newY - 1);
                    checkLEFT(node, newX - 1, newY);
                    checkD_UP_LEFT(node, newX - 1, newY - 1);

                    break;
                }
                case D_DOWN_RIGHT:
                {
                    if (reason == UP)
                    {
                        if (!(g_Tile[newY - 1][newX] == BLOCK && g_Tile[newY][newX + 1] == BLOCK))
                            checkD_UP_RIGHT(node, newX + 1, newY - 1);
                    }
                    else if (reason == LEFT)
                    {
                        if (!(g_Tile[newY][newX - 1] == BLOCK && g_Tile[newY + 1][newX] == BLOCK))
                            checkD_DOWN_LEFT(node, newX - 1, newY + 1);
                    }

                    checkDOWN(node, newX, newY + 1);
                    checkRIGHT(node, newX + 1, newY);
                    checkD_DOWN_RIGHT(node, newX + 1, newY + 1);
                    break;
                }
                case D_DOWN_LEFT:
                {
                    if (reason == UP)
                    {
                        if (!(g_Tile[newY - 1][newX] == BLOCK && g_Tile[newY][newX - 1] == BLOCK))
                            checkD_UP_LEFT(node, newX - 1, newY - 1);
                    }
                    else if (reason == RIGHT)
                    {
                        if (!(g_Tile[newY][newX + 1] == BLOCK && g_Tile[newY + 1][newX] == BLOCK))
                            checkD_DOWN_RIGHT(node, newX + 1, newY + 1);
                    }

                    checkDOWN(node, newX, newY + 1);
                    checkLEFT(node, newX - 1, newY);
                    checkD_DOWN_LEFT(node, newX - 1, newY + 1);
                    break;
                }
            }
            Render();
        }
    }
}
void  JumpPointSearch::clearPath(bool isClearAll)
{
    for (Node* n : _closeList)
        delete n;
    
    while (!_openList.empty())
    {
        delete _openList.top();
        _openList.pop();
    }

    if (!isClearAll)
    {
        for (int row = 0; row < GRID_HEIGHT; ++row)
        {
            for (int col = 0; col < GRID_WIDTH; ++col)
            {
                if (g_Tile[row][col] > BLOCK)
                    g_Tile[row][col] = EMPTY;
            }
        }

        g_Tile[_start._ypos][_start._xpos] = START;
        g_Tile[_end._ypos][_end._xpos] = END;
    }
    else 
        memset(g_Tile, EMPTY, GRID_HEIGHT * GRID_WIDTH);

    _find = nullptr;
    _openListMap.clear();
    _closeList.clear();

}

JumpPointSearch::JumpPointSearch(Point start, Point end)
    :_start { start }, _end { end }, _find { nullptr }
{
    _openListMap.reserve(GRID_HEIGHT * GRID_WIDTH);
    _closeList.reserve(GRID_HEIGHT * GRID_WIDTH);
}
JumpPointSearch::~JumpPointSearch()
{
    for (Node* n : _closeList)
    {
        delete n;
    }

    while (!_openList.empty())
    {
        Node* n = _openList.top();
        _openList.pop();
        delete n;
    }
}

void JumpPointSearch::init(Point start, Point end)
{
    if (!_openList.empty() || !_closeList.empty())
        clearPath(false);

    _start = start;
    _end = end;

    double H = getH(start, end);
    _openList.push(new Node{start, nullptr, 0, H,H ,NONE, NONE });
}

double JumpPointSearch::getH(Point start, Point end) const { return (std::abs(end._xpos - start._xpos) + std::abs(end._ypos - start._ypos)); }
double JumpPointSearch::getG(Point start, Point end) const
{
    int dx = end._xpos - start._xpos;
    int dy = end._ypos - start._ypos;
    return std::sqrt(dx * dx + dy * dy);
}
