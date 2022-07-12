// include headers

#include "utils.h"
#include "bitmap_image.hpp"

int main()
{
    PT eye,look,up;

    ifstream fin("scene.txt");
    ofstream fout("stage1.txt");

    fin >> eye >> look >> up;
   
    double fovY, aspectRatio, near, far;
    fin >> fovY >> aspectRatio >> near >> far;

    /**
     * @brief STAGE 1 : Modeling Transformation
     * 
     */

    stack<Matrix>stk;
    Matrix IDT(4);
    IDT.createIdentity();
    stk.push(IDT);

    int num_triangles = 0;

    while(true)
    {
        string cmd;
        fin>>cmd;

        if(cmd == "triangle")
        {
            PT p1,p2,p3;
            fin>>p1>>p2>>p3;

            p1 = stk.top()*p1;
            p2 = stk.top()*p2; 
            p3 = stk.top()*p3;

            fout<<p1<<endl;
            fout<<p2<<endl;
            fout<<p3<<endl; 
            fout<<endl; 

            num_triangles++; 
        }
        else if(cmd == "translate")
        {
            PT p;
            fin>>p;

            Matrix T(4);
            T.createTranslation(p);
            Matrix M = stk.top()*T;
            stk.pop();
            stk.push(M);
        }
        else if(cmd == "scale")
        {
            PT p;
            fin>>p.x>>p.y>>p.z;

            Matrix S(4);
            S.createScaling(p);
            Matrix M = stk.top()*S;
            stk.pop();
            stk.push(M);
        }
        else if(cmd == "rotate")
        {
            double angle;
            PT p;
            fin>>angle>>p.x>>p.y>>p.z;

            Matrix R(4);
            R.createRotation(angle,p);
            Matrix M = stk.top()*R;
            stk.pop();
            stk.push(M);
        }
        else if(cmd == "push")
        {
            stk.push(stk.top());
        }
        else if(cmd == "pop")
        {
            if(stk.empty())
            {
                cout<<"Error: stack is empty"<<endl;
                return 0;
            }

            stk.pop();
        }
        else if(cmd == "end")
        {
            break;
        }
        else
        {
            cout<<"Invalid Command"<<endl;
            break;
        }
    }

    fin.close();
    fout.close();


    /**
     * @brief STAGE 2 : View Transformation
     * 
     */

    fin.open("stage1.txt");
    fout.open("stage2.txt");

    Matrix viewTransform(4);
    viewTransform.createView(eye,look,up);

    for(int i=0;i<num_triangles;i++)
    {
        PT p1,p2,p3;
        fin>>p1>>p2>>p3;

        p1 = viewTransform*p1;
        p2 = viewTransform*p2;
        p3 = viewTransform*p3;

        fout<<p1<<endl;
        fout<<p2<<endl;
        fout<<p3<<endl;
        fout<<endl;
    }

    fin.close();
    fout.close();

    
    /**
     * @brief STAGE 3 : Projection Transformation
     * 
     */

    fin.open("stage2.txt");
    fout.open("stage3.txt");

    Matrix projTransform(4);
    projTransform.createProjection(fovY,aspectRatio,near,far);

    for(int i=0;i<num_triangles;i++)
    {
        PT p1,p2,p3;
        fin>>p1>>p2>>p3;

        p1 = projTransform*p1;
        p2 = projTransform*p2;
        p3 = projTransform*p3;

        fout<<p1<<endl;
        fout<<p2<<endl;
        fout<<p3<<endl;
        fout<<endl;
    }

    fin.close();
    fout.close();

    /**
     * @brief STAGE 4 : Z BUFFER Algorithm 
     * 
     */

    fin.open("config.txt");
    fout.open("z_buffer.txt");

    int screenWidth, screenHeight;
    double boxLeft, boxRight, boxUp, boxDown;
    double zMin, zMax;

    fin>>screenWidth>>screenHeight;
    fin>>boxLeft>>boxDown;
    fin>>zMin>>zMax;

    fin.close();

    // calculate necessary variables

    boxRight = -boxLeft;
    boxUp = -boxDown;

    double dx = (boxRight-boxLeft)/screenWidth;
    double dy = (boxUp-boxDown)/screenHeight;

    double topY = boxUp - dy/2; // center of top left pixel
    double bottomY = boxDown + dy/2;
    double leftX = boxLeft + dx/2;
    double rightX = boxRight - dx/2;

    // create z buffer with each cell initialized to zMax
    vector<vector<double>> z_buffer(screenHeight,vector<double>(screenWidth,zMax));

    // init image as all black cells
    bitmap_image image(screenWidth, screenHeight);
    for(int i=0;i<screenWidth;i++)
    {
        for(int j=0;j<screenHeight;j++)
        {
            image.set_pixel(i,j,0,0,0);
        }
    }

    fin.open("stage3.txt");

    for(int tr=0;tr<num_triangles;tr++)
    {
        PT p1,p2,p3;
        fin>>p1>>p2>>p3;

        Triangle t;
        t.points[0] = p1;
        t.points[1] = p2;
        t.points[2] = p3;

        double minX, maxX, minY, maxY;

        // calculate min max values of triangles
        minX = min(min(p1.x,p2.x),p3.x);
        maxX = max(max(p1.x,p2.x),p3.x);
        minY = min(min(p1.y,p2.y),p3.y);
        maxY = max(max(p1.y,p2.y),p3.y);

        // clip appropriately
        minX = max(minX,leftX);
        maxX = min(maxX,rightX);

        minY = max(minY,bottomY);
        maxY = min(maxY,topY);

        // find the Y value to start iterating from
        int startY = round((topY-minY)/dy);
        int endY = round((topY-maxY)/dy);

        // start doing scanline along the specified Y values
        for(int scanY = endY;scanY <= startY; scanY++)
        {
            double ys = topY - scanY*dy;

            vector<double>x_ab(2),z_ab(2);
            int cnt = 0;

            for(int i=0;i<3;i++)
            {
                // (0,1) (1,2) (2,0)
                int j = (i+1)%3;

                if(t.points[i].y == t.points[j].y) continue;

                // check if the segments intersects with the scanline, can be atmost 2 of them
                if(ys >= min(t.points[i].y,t.points[j].y) && ys <= max(t.points[i].y,t.points[j].y))
                {
                    // formula from slide ;-;
                    x_ab[cnt] = t.points[i].x - (t.points[i].x - t.points[j].x)*(t.points[i].y - ys)/(t.points[i].y - t.points[j].y);
                    z_ab[cnt] = t.points[i].z - (t.points[i].z - t.points[j].z)*(t.points[i].y - ys)/(t.points[i].y - t.points[j].y);
                    cnt++;
                }
            }

            vector<double>tempx_ab(2);
            tempx_ab = x_ab;

            for(int i=0;i<2;i++)
            {
                // clip x on both sides
                if(x_ab[i]<minX) x_ab[i] = minX;
                if(x_ab[i]>maxX) x_ab[i] = maxX;
            }

            // re-adjust
            z_ab[0] = z_ab[1] - (z_ab[1] - z_ab[0])*(tempx_ab[1] - x_ab[0])/(tempx_ab[1] - tempx_ab[0]);
            z_ab[1] = z_ab[1] - (z_ab[1] - z_ab[0])*(tempx_ab[1] - x_ab[1])/(tempx_ab[1] - tempx_ab[0]);


            // a is the left point, b is the right one according to the image of slide
            double xa,za,xb,zb;

            xa = x_ab[0];
            xb = x_ab[1];

            za = z_ab[0];
            zb = z_ab[1];

            if(x_ab[0] >= x_ab[1])
            {
                swap(xa,xb);
                swap(za,zb);
                swap(tempx_ab[0],tempx_ab[1]);
            }

            int startX = round((xa-leftX)/dx);
            int endX = round((xb-leftX)/dx);
            
            // for a specific scanline, iterate through all the pixels along X
            for(int scanX=startX;scanX<=endX;scanX++)
            {
                double xp = leftX + scanX*dx;

                // formula from slide again ;-;
                double zp = zb - (zb-za)*((xb-xp)/(xb-xa));

                if (zp < zMin) continue;

                // assert for double checking
                assert(scanY >= 0 && scanY < screenHeight);
                assert(scanX >= 0 && scanX < screenWidth);

                if(zp < z_buffer[scanY][scanX])
                {
                    z_buffer[scanY][scanX] = zp;
                    image.set_pixel(scanX,scanY,t.rgb[0],t.rgb[1],t.rgb[2]);
                }
            }
        }
    }

    for (int i = 0; i < screenHeight; i++) 
    {
        for (int j = 0; j < screenWidth; j++)
        {
            if (z_buffer[i][j] < zMax)
            {
                fout << setprecision(6) << fixed << z_buffer[i][j] << "\t";
            }
        }
        fout << endl;
    }

    fout.close();

    image.save_image("out.bmp");

    return 0;
}