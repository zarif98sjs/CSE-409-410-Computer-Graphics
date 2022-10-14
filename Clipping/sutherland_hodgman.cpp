/* Which of the favors of your Lord will you deny? */

/**
 * 
 * Sutherland Hodgman Polygon Clipping Algorithm
 * ---------------------------------------------
 * 
 * Works for Convex polygon clip region.
 * 
 */

#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;

    Point() {
        x = 0;
        y = 0;
    }

    Point(double x, double y) {
        this->x = x;
        this->y = y;
    }
};

struct Polygon {
    vector<Point> points;
};

struct Line {
    Point p1, p2;
};


// Sutherland-Hodgman algorithm

Polygon sutherlandHodgman(Polygon subjectPolygon, Polygon clipPolygon) {
    Polygon outputPolygon = subjectPolygon;

    for (int i = 0; i < clipPolygon.points.size(); i++) {
        int k = (i + 1) % clipPolygon.points.size();
        Polygon inputPolygon = outputPolygon;
        outputPolygon.points.clear();

        Point A = clipPolygon.points[i];
        Point B = clipPolygon.points[k];

        // do clipping for the infinite line joining A and B

        for (int j = 0; j < inputPolygon.points.size(); j++) {
            int l = (j + 1) % inputPolygon.points.size();
            Point P = inputPolygon.points[j];
            Point Q = inputPolygon.points[l];

            // check which side P is on with respect to line AB
            double sideP = (B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x);
            // check which side Q is on with respect to line AB
            double sideQ = (B.x - A.x) * (Q.y - A.y) - (B.y - A.y) * (Q.x - A.x);

            if (sideP < 0 && sideQ < 0) {
                // both P and Q are inside, so only Q is added to the output polygon
                outputPolygon.points.push_back(Q);
            } else if (sideP < 0 && sideQ >= 0) {
                // P is inside and Q is outside, so intersection point is added to the output polygon
                double t = ((P.y - A.y) * (B.x - A.x) - (P.x - A.x) * (B.y - A.y)) / ((Q.x - P.x) * (B.y - A.y) - (Q.y - P.y) * (B.x - A.x));
                Point intersectionPoint;
                intersectionPoint.x = P.x + t * (Q.x - P.x);
                intersectionPoint.y = P.y + t * (Q.y - P.y);
                outputPolygon.points.push_back(intersectionPoint);
            } else if (sideP >= 0 && sideQ <= 0) {
                // P is outside and Q is inside, so intersection point and Q are added to the output polygon
                double t = ((P.y - A.y) * (B.x - A.x) - (P.x - A.x) * (B.y - A.y)) / ((Q.x - P.x) * (B.y - A.y) - (Q.y - P.y) * (B.x - A.x));
                Point intersectionPoint;
                intersectionPoint.x = P.x + t * (Q.x - P.x);
                intersectionPoint.y = P.y + t * (Q.y - P.y);
                outputPolygon.points.push_back(intersectionPoint);
                outputPolygon.points.push_back(Q);
            }
        }
    }

    return outputPolygon;
}

int main()
{
    // Case 1

    // Polygon : (100,150), (200,250), (300,200)
    // Clipping Area : (150,150), (150,200), (200,200), (200,150)

    Polygon subjectPolygon;
    subjectPolygon.points = {
        Point(100,150),
        Point(200,250),
        Point(300,200)
    };

    Polygon clipPolygon;
    clipPolygon.points = {
        Point(150,150),
        Point(150,200),
        Point(200,200),
        Point(200,150)
    };

    Polygon outputPolygon = sutherlandHodgman(subjectPolygon, clipPolygon);

    cout << "Output Polygon: " << endl;
    for (int i = 0; i < outputPolygon.points.size(); i++) {
        cout << "(" << outputPolygon.points[i].x << ", " << outputPolygon.points[i].y << ")" << endl;
    }

    // Case 2
    // Polygon : (100,150), (200,250), (300,200)
    // Clipping Area : (100,300), (300,300), (200,100)

    subjectPolygon.points = {
        Point(100,150),
        Point(200,250),
        Point(300,200)
    };

    clipPolygon.points = {
        Point(100,300),
        Point(300,300),
        Point(200,100)
    };

    outputPolygon = sutherlandHodgman(subjectPolygon, clipPolygon);

    cout << "Output Polygon: " << endl;
    for (int i = 0; i < outputPolygon.points.size(); i++) {
        cout << "(" << outputPolygon.points[i].x << ", " << outputPolygon.points[i].y << ")" << endl;
    }


    return 0;
}