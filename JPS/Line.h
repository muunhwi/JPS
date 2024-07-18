#pragma once
#include <cmath>
#include <vector>

class Line
{
public:
    void linePoint(int sxpos, int sypos, int expos, int eypos);
    const std::vector<std::pair<int, int>>& getPoints() const { return points; }
private:
    enum Quadrant
    {
        Q1,
        Q2,
        Q3,
        Q4
    };
    std::vector<std::pair<int, int>> points;
private:
    void addPoint(Quadrant q, int sxpos, int sypos, int xpos, int ypos);
};




