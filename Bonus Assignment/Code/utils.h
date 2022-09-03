#include <bits/stdc++.h>
#include "bitmap_image.hpp"
using namespace std;

#define PI 2.0 * acos(0.0)

/**
 * @brief Point
 *
 */

struct PT
{

    double x, y;

    PT()
    {
        x = y = 0.0;
    }

    PT(double x, double y) : x(x), y(y) {}
    PT(const PT &p) : x(p.x), y(p.y) {}

    // arithemtic operations

    PT operator+(PT b) { return PT(x + b.x, y + b.y); }
    PT operator-(PT b) { return PT(x - b.x, y - b.y); }
    PT operator*(double b) { return PT(x * b, y * b); }
    PT operator/(double b) { return PT(x / b, y / b); }

    // functions

    double length() { return sqrt(x * x + y * y); }

    // streams
    friend ostream &operator<<(ostream &out, PT p)
    {
        out << "(" << p.x << "," << p.y << ")";
        return out;
    }

    friend istream &operator>>(istream &in, PT &p)
    {
        in >> p.x >> p.y;
        return in;
    }

    friend ofstream &operator<<(ofstream &output, PT &p)
    {
        output << fixed << setprecision(7) << p.x << " " << p.y;
        return output;
    }
};

struct Color
{
    double r, g, b;
    Color()
    {
        r = g = b = 0.0;
    }
    Color(double r, double g, double b) : r(r), g(g), b(b) {}
    Color(const Color &c) : r(c.r), g(c.g), b(c.b) {}
};

extern bitmap_image image;
extern int screenHeight, screenWidth;

/**
 * @brief Line
 *
 */

struct Line
{
    PT p, q;
    Color c;

    Line() {}
    Line(PT p, PT q) : p(p), q(q) {}
    Line(PT p, PT q, Color c) : p(p), q(q), c(c) {}

    void drawPixel(int x, int y)
    {
        // cout<<c.r<<" "<<c.g<<" "<<c.b<<endl;
        image.set_pixel(x, screenHeight - y, c.r, c.g, c.b);
    }

    void IntensifyPixel(int x, int y, double distance)
    {
        // double intensity = Filter(Math.round(Math.abs(distance)));
        // cout<<"distance: "<<distance<<endl;
        distance = round(abs(distance));
        image.set_pixel(x, screenHeight - y, c.r * distance, c.g * distance, c.b * distance);
    }

    void drawLineCase0(int dx, int dy, int type)
    {
        int y = p.y;
        int d = 2 * dy - dx;
        int incrDown = 2 * dy;
        int incrUp = 2 * (dy - dx);

        // AA
        int two_v_dx = 0;
        double invDenom = 1.0 / (2 * sqrt(dx * dx + dy * dy));
        double two_v_dx_invDenom = 2.0 * dx * invDenom;

        for (int x = p.x; x <= q.x; x++)
        {
            drawPixel(x, y);

            if (d < 0)
            {
                two_v_dx = d + dx;
                d += incrDown;
            }
            else
            {
                two_v_dx = d - dx;
                d += incrUp;
                y++;
            }

            // Anti Aliasing

            if (type == 2) // weighted
            {
                IntensifyPixel(x, y, two_v_dx * invDenom);
                IntensifyPixel(x, y + 1, two_v_dx_invDenom - two_v_dx * invDenom);
                IntensifyPixel(x, y - 1, two_v_dx_invDenom + two_v_dx * invDenom);
            }
            else if (type == 1) // unweighted
            {
                IntensifyPixel(x, y, 1);
                IntensifyPixel(x, y + 1, 1);
                IntensifyPixel(x, y - 1, 1);
            }
        }
    }

    void drawLineCase1(int dx, int dy, int type)
    {
        int x = p.x;
        int d = -2 * dy - dx;
        int incrRight = 2 * (dy - dx);
        int incrUp = -2 * dx;

        // AA
        int two_v_dx = 0;
        double invDenom = 1.0 / (2 * sqrt(dx * dx + dy * dy));
        double two_v_dx_invDenom = 2.0 * dy * invDenom;

        for (int y = p.y; y <= q.y; y++)
        {
            drawPixel(x, y);
            if (d <= 0)
            {
                two_v_dx = d + dy;
                d += incrRight;
                x++;
            }
            else
            {
                two_v_dx = d - dy;
                d += incrUp;
            }

            // ANTI ALIASING

            if (type == 2)
            {
                IntensifyPixel(x, y, two_v_dx * invDenom);
                IntensifyPixel(x - 1, y, two_v_dx_invDenom - two_v_dx * invDenom);
                IntensifyPixel(x + 1, y, two_v_dx_invDenom + two_v_dx * invDenom);
            }
            else if (type == 1)
            {
                IntensifyPixel(x, y, 1);
                IntensifyPixel(x - 1, y, 1);
                IntensifyPixel(x + 1, y, 1);
            }
        }
    }

    void drawLineCase2(int dx, int dy, int type)
    {
        int y = p.y;
        int d = 2 * dy - dx;
        int incrRight = 2 * dy;
        int incrDown = 2 * (dy + dx);

        // AA
        int two_v_dx = 0;
        double invDenom = 1.0 / (2 * sqrt(dx * dx + dy * dy));
        double two_v_dx_invDenom = 2.0 * dx * invDenom;

        for (int x = p.x; x <= q.x; x++)
        {
            drawPixel(x, y);

            if (d < 0)
            {
                two_v_dx = d + dx;
                d += incrDown;
                y--;
            }
            else
            {
                two_v_dx = d - dx;
                d += incrRight;
            }

            if (type == 2)
            {
                IntensifyPixel(x, y, two_v_dx * invDenom);
                IntensifyPixel(x, y + 1, two_v_dx_invDenom - two_v_dx * invDenom);
                IntensifyPixel(x, y - 1, two_v_dx_invDenom + two_v_dx * invDenom);
            }
            else if (type == 1)
            {
                IntensifyPixel(x, y, 1);
                IntensifyPixel(x, y + 1, 1);
                IntensifyPixel(x, y - 1, 1);
            }
        }
    }

    void drawLineCase3(int dx, int dy, int type)
    {
        int x = p.x;
        int d = dy + 2 * dx;
        int incrDown = 2 * dx;
        int incrRight = 2 * (dy + dx);

        // AA
        int two_v_dx = 0;
        double invDenom = 1.0 / (2 * sqrt(dx * dx + dy * dy));
        double two_v_dx_invDenom = 2.0 * dy * invDenom;

        for (int y = p.y; y >= q.y; y--)
        {
            drawPixel(x, y);

            if (d < 0)
            {
                two_v_dx = d + dy;
                d += incrDown;
            }
            else
            {
                two_v_dx = d - dy;
                d += incrRight;
                x++;
            }

            // ANTI ALIASING
            if (type == 2)
            {
                IntensifyPixel(x, y, two_v_dx * invDenom);
                IntensifyPixel(x - 1, y, two_v_dx_invDenom - two_v_dx * invDenom);
                IntensifyPixel(x + 1, y, two_v_dx_invDenom + two_v_dx * invDenom);
            }
            else if (type == 1)
            {
                IntensifyPixel(x, y, 1);
                IntensifyPixel(x - 1, y, 1);
                IntensifyPixel(x + 1, y, 1);
            }
        }
    }

    void MidpointAlgorithm(int type = 0)
    {
        if (p.x > q.x)
        {
            swap(p.x, q.x);
            swap(p.y, q.y);
        }

        int dx = q.x - p.x;
        int dy = q.y - p.y;

        // cout << "dx: " << dx << " dy: " << dy << endl;

        if (dy <= dx && dy >= 0) // 0 < slope <= 1
        {
            cout << "Case 0" << endl;
            drawLineCase0(dx, dy, type);
            return;
        }
        else if (dy > dx && dy >= 0) // 1 < slope < INF
        {
            cout << "Case 1" << endl;
            drawLineCase1(dx, dy, type);
            return;
        }
        else if (-dy <= dx)
        {
            cout << "Case 2" << endl;
            drawLineCase2(dx, dy, type);
            return;
        }
        else
        {
            cout << "Case 3" << endl;
            drawLineCase3(dx, dy, type);
            return;
        }
    }
};
