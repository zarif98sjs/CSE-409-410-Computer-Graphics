/* Which of the favors of your Lord will you deny? */

/**
 * 
 * Cyrus-Beck Line Clipping Algorithm
 * ---------------------------------------------
 * 
 * This can be modified to work with polygon clip region
 * (not only rectangle like in Cohern Sutherland) as well.
 * 
 *  ** Does not work with CONCAVE polygon clip region **
 * 
 */


#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;

    Point(double x, double y) {
        this->x = x;
        this->y = y;
    }
};

struct Line {
    Point p1, p2;
};


void cyrusBeck(Line line, double xMin, double xMax, double yMin, double yMax) {
    double dx = line.p2.x - line.p1.x;
    double dy = line.p2.y - line.p1.y;

    vector<Point> p;
    p.push_back(Point(1, 0));
    p.push_back(Point(0, 1));
    p.push_back(Point(-1, 0));
    p.push_back(Point(0, -1));

    vector<double> q;
    q.push_back(xMax - line.p1.x);
    q.push_back(yMax - line.p1.y);
    q.push_back(line.p1.x - xMin);
    q.push_back(line.p1.y - yMin);

    double tMin = 0;
    double tMax = 1;

    for (int i = 0; i < 4; i++) {
        double r = p[i].x * dx + p[i].y * dy;
        double s = q[i];

        if (r == 0) {
            if (s < 0) {
                cout << "Line is completely outside the window, Reject ("<< line.p1.x << ", " << line.p1.y << ") to (" << line.p2.x << ", " << line.p2.y << ")" << endl;
                return;
            }
        } else {
            double t = (double) s / r;
            if (r < 0) {
                tMin = max(tMin, t);
            } else {
                tMax = min(tMax, t);
            }
        }
    }

    if (tMin > tMax) {
        cout << "Line is completely outside the window, Reject ("<< line.p1.x << ", " << line.p1.y << ") to (" << line.p2.x << ", " << line.p2.y << ")" << endl;
    } else {
        cout << "Line is partially inside the window, Accept ("<< line.p1.x + tMin * dx << ", " << line.p1.y + tMin * dy << ") to (" << line.p1.x + tMax * dx << ", " << line.p1.y + tMax * dy << ")" << endl;
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
        cyrusBeck(line, xMin, xMax, yMin, yMax);
    }

    return 0;
}