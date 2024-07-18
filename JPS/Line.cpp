#include "Line.h"

void Line::linePoint(int sxpos, int sypos, int expos, int eypos)
{
    int dx = std::abs(sxpos - expos);
    int dy = std::abs(sypos - eypos);
    int error = 0;

    Quadrant q;
    int xpos;
    int ypos;

    if (sxpos > expos)
    {
        if (sypos > eypos)
            q = Q4;
        else
            q = Q1;
    }
    else
    {
        if (sypos < eypos)
            q = Q2;
        else
            q = Q3;
    }

    if (dx > dy)
    {
        xpos = 1;
        ypos = 0;

        if (dy == 1)
            dy++;

        for (; xpos < dx; ++xpos)
        {
            error += dy;
            if (error >= dx)
            {
                ypos++;
                error -= dx;
            }
            addPoint(q, sxpos, sypos, xpos, ypos);
        }
    }
    else
    {
        xpos = 0;
        ypos = 1;

        if (dx == 1)
            dx++;

        for (; ypos < dy; ++ypos)
        {
            error += dx;

            if (error >= dy)
            {
                xpos++;
                error -= dy;
            }
            addPoint(q, sxpos, sypos, xpos, ypos);
        }
    }
}
void Line::addPoint(Quadrant q, int sxpos, int sypos, int xpos, int ypos)
{
    switch (q)
    {
    case Q1:
    {
        points.push_back({ sxpos - xpos, sypos + ypos });
        break;
    }
    case Q2:
    {
        points.push_back({ sxpos + xpos, sypos + ypos });
        break;
    }
    case Q3:
    {
        points.push_back({ sxpos + xpos, sypos - ypos });
        break;
    }
    case Q4:
    {
        points.push_back({ sxpos - xpos, sypos - ypos });
        break;
    }
    }
}
