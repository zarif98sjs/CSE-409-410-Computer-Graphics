// include headers

#include "utils.h"


int screenWidth, screenHeight;

bitmap_image image;

void initImage()
{
    image.setwidth_height(screenWidth, screenHeight);
    // init image as all black cells
    for (int i = 0; i < screenWidth; i++)
    {
        for (int j = 0; j < screenHeight; j++)
        {
            image.set_pixel(i, j, 255, 255, 255);
        }
    }
}

int main()
{    
    ifstream fin("scene.txt");

    fin>>screenWidth>>screenHeight;

    initImage();

    int numLines;
    fin >> numLines;

    vector<Line> lines;

    for (int i = 0; i < numLines; i++)
    {
        Line l;
        fin >> l.p >> l.q;
        fin >> l.c.r >> l.c.g >> l.c.b;

        // cout<< "Line " << i << " : " << l.p << " " << l.q << " " << l.c.r << " " << l.c.g << " " << l.c.b << endl;

        lines.push_back(l);
        l.MidpointAlgorithm();
    }

    image.save_image("1_R.bmp");

    initImage();

    for (auto l : lines)
        l.MidpointAlgorithm(1);

    image.save_image("2_RUA.bmp");

    initImage();

    for (auto l : lines)
        l.MidpointAlgorithm(2);

    image.save_image("3_RWA.bmp");

    return 0;
}