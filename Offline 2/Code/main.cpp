// include headers

#include "utils.h"

int main()
{
    PT eye,look,up;

    ifstream fin("scene.txt");
    ofstream fout("stage1.txt");

    fin >> eye.x >> eye.y >> eye.z;
    fin >> look.x >> look.y >> look.z;
    fin >> up.x >> up.y >> up.z;

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


    return 0;
}