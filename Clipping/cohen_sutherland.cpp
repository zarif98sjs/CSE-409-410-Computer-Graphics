/* Which of the favors of your Lord will you deny? */

/**
 * 
 * Cohen Sutherland Line Clipping Algorithm
 * ---------------------------------------------
 * 
 * Only works with rectangle like clip region.
 * 
 */

#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;
    int code;

    Point(double x, double y) {
        this->x = x;
        this->y = y;
        code = 0;
    }

    void setCode(double xMin, double xMax, double yMin, double yMax) {
        code = 0;
        if (x < xMin) code |= 1;
        if (x > xMax) code |= 2;
        if (y < yMin) code |= 4;
        if (y > yMax) code |= 8;
    }
};

struct Line {
    Point p1, p2;
};

void cohenSutherland(Line line, double xMin, double xMax, double yMin, double yMax) {
    line.p1.setCode(xMin, xMax, yMin, yMax);
    line.p2.setCode(xMin, xMax, yMin, yMax);

    int code1 = line.p1.code;
    int code2 = line.p2.code;

    while (true) {
        // cout<<"("<<line.p1.x<<", "<<line.p1.y<<") -> ("<<line.p2.x<<", "<<line.p2.y<<")"<<endl;
        // cout<<"code1: "<<code1<<", code2: "<<code2<<endl;
        if ((code1 | code2) == 0) {
            cout << "Line is completely inside the window, Accept ("<< line.p1.x << ", " << line.p1.y << ") to (" << line.p2.x << ", " << line.p2.y << ")" << endl;
            break;
        } else if ((code1 & code2) != 0) {
            cout << "Line is completely outside the window, Reject ("<< line.p1.x << ", " << line.p1.y << ") to (" << line.p2.x << ", " << line.p2.y << ")" << endl;
            break;
        } else {
            int codeOut = code1 ? code1 : code2;
            double x, y;

            // order is from left to right

            if (codeOut & 8) {
                x = line.p1.x + (line.p2.x - line.p1.x) * (yMax - line.p1.y) / (line.p2.y - line.p1.y);
                y = yMax;
            } else if (codeOut & 4) {
                x = line.p1.x + (line.p2.x - line.p1.x) * (yMin - line.p1.y) / (line.p2.y - line.p1.y);
                y = yMin;
            } else if (codeOut & 2) {
                y = line.p1.y + (line.p2.y - line.p1.y) * (xMax - line.p1.x) / (line.p2.x - line.p1.x);
                x = xMax;
            } else if (codeOut & 1) {
                y = line.p1.y + (line.p2.y - line.p1.y) * (xMin - line.p1.x) / (line.p2.x - line.p1.x);
                x = xMin;
            }

            if (codeOut == code1) {
                // if codeOut was code1, then update code1 and p1
                line.p1.x = x;
                line.p1.y = y;
                line.p1.setCode(xMin, xMax, yMin, yMax);
                code1 = line.p1.code;
            } else {
                // if codeOut was code2, then update code2 and p2
                line.p2.x = x;
                line.p2.y = y;
                line.p2.setCode(xMin, xMax, yMin, yMax);
                code2 = line.p2.code;
            }
        }
    }
}

int main()
{
    double xMin, xMax, yMin, yMax;
    xMin = 0, xMax = 10, yMin = 0, yMax = 10;

    // trivial accept
    Line l1 = {Point(1, 1), Point(5, 5)};
    Line l2 = {Point(1, 5), Point(5, 1)};
    Line l3 = {Point(1, 1), Point(5, 1)};
    Line l4 = {Point(1, 1), Point(1, 5)};
    Line l5 = {Point(5, 5), Point(5, 1)};

    // trivial reject
    Line l6 = {Point(11, 5), Point(16, 5)};
    Line l7 = {Point(11, 5), Point(11, 15)};
    Line l8 = {Point(-4, 1), Point(-1, 10)};
    Line l9 = {Point(0, -5), Point(-2, -15)};

    // partially accept
    Line l10 = {Point(1, 1), Point(15, 5)};
    Line l11 = {Point(1, 1), Point(5, 15)};
    Line l12 = {Point(-1, -1), Point(5, 5)};
    Line l13 = {Point(-1, -1), Point(15, 5)};

    vector<Line> lines = {l1, l2, l3, l4, l5, l6, l7, l8, l9, l10, l11, l12, l13};

    for (Line line : lines) {
        cohenSutherland(line, xMin, xMax, yMin, yMax);
    }

    return 0;
}