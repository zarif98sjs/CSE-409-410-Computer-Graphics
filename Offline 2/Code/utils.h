#include<bits/stdc++.h>
using namespace std;

#define PI 2.0*acos(0.0)

/**
 * @brief Point
 * 
 */

struct PT
{

	double x,y,z,n;

    PT()
    {
        x = y = z = 0.0;
        n = 1.0;
    }

	PT(double x, double y, double z) : x(x), y(y), z(z) , n(1.0) {}
    PT(double x, double y, double z, double n) : x(x), y(y), z(z), n(n) {}
    PT(const PT &p) : x(p.x), y(p.y), z(p.z), n(p.n) {}

	// arithemtic operations

	PT operator +(PT b)  {return PT(x+b.x,y+b.y, z+b.z);}
    PT operator -(PT b)  {return PT(x-b.x,y-b.y, z-b.z);}
	PT operator *(double b)  {return PT(x*b,y*b, z*b);}
	PT operator /(double b)  {return PT(x/b,y/b, z/b);}
	double operator *(PT b)  {return x*b.x+y*b.y+z*b.z;}
	PT operator ^(PT b)  {return PT(y*b.z-z*b.y, z*b.x-x*b.z, x*b.y-y*b.x);}

    // functions

    double length() {return sqrt(x*x+y*y+z*z);}

    void normalize()
    {
        double len = length();
        x /= len;
        y /= len;
        z /= len;
    }

    void scaleDown()
    {
        x /= n;
        y /= n;
        z /= n;
        n /= n;
    }

    // streams
    friend ostream& operator<<(ostream &out, PT p)
    {
        out << "(" << p.x << "," << p.y << "," << p.z << ")" << " : " << p.n;
        return out;
    }

    friend istream& operator>>(istream &in, PT &p)
    {
        in >> p.x >> p.y >> p.z;
        return in;
    }

    friend ofstream& operator<<(ofstream &output, PT &p)
    {
        output << fixed << setprecision(7) << p.x << " " << p.y <<" " << p.z;
        return output;
    }
};

/**
 * @brief Triangle
 * 
 */

struct Triangle{
    PT points[3];
    int rgb[3];

    Triangle()
    {
        rgb[0] = rand()%255+1;
        rgb[1] = rand()%255+1;
        rgb[2] = rand()%255+1;
    } 

    Triangle(PT p1, PT p2, PT p3)
    {
        points[0] = p1;
        points[1] = p2;
        points[2] = p3;
        rgb[0] = rand()%255+1;
        rgb[1] = rand()%255+1;
        rgb[2] = rand()%255+1;
    }
};

/**
 * @brief Matrix
 * 
 */

struct Matrix{

    vector<vector<double> > ara;
    int dim;

    Matrix()
    {
        ara.resize(4,vector<double>(4,0));
        dim = 4;
    }

    Matrix(int dim)
    {
        ara.resize(dim,vector<double>(dim,0));
        this->dim = dim;
    }

    // Matrix-Matrix Multiplication
    Matrix operator*(Matrix b)
    {
        Matrix c(dim);
        for(int i=0; i<dim; i++)
        {
            for(int j=0; j<dim; j++)
            {
                for(int k=0; k<dim; k++)
                {
                    c.ara[i][j] += ara[i][k]*b.ara[k][j];
                }
            }
        }
        return c;
    }

    // Matrix-Vector Multiplication
    PT operator*(PT b)
    {
        double res[4];
        double b_vec[4] = {b.x,b.y,b.z,b.n};

        for(int i=0; i<dim; i++)
        {
            res[i] = 0;
            for(int j=0; j<dim; j++)
            {
                res[i] += ara[i][j]*b_vec[j];
            }
        }
        PT ret(res[0],res[1],res[2],res[3]);
        ret.scaleDown();
        return ret;
    }

    // Identity Matrix
    void createIdentity() // I
    {
        for(int i=0; i<dim; i++)
        {
            for(int j=0; j<dim; j++)
            {
                if(i==j)
                    ara[i][j] = 1;
                else
                    ara[i][j] = 0;
            }
        }
    }

    // Translation Matrix
    void createTranslation(PT p) // T
    {
        createIdentity();
        ara[0][3] = p.x;
        ara[1][3] = p.y;
        ara[2][3] = p.z;
    }

    // Scaling Matrix
    void createScaling(PT p) // S
    {
        createIdentity();
        ara[0][0] = p.x;
        ara[1][1] = p.y;
        ara[2][2] = p.z;
    }

    // Rodrigues Formula
    PT applyRodriguesFormula(PT x, PT a, double theta) 
    {   
        double rad = theta*PI/180;
        return x*cos(rad)+a*(a*x)*(1-cos(rad))+(a^x)*sin(rad);
    }

    // Rodrigues Rotation Matrix
    void createRotation(double angle,PT p) // R
    {
        createIdentity();
        PT a(p.x,p.y,p.z);
        a.normalize();
        PT x(1,0,0);
        PT y(0,1,0);
        PT z(0,0,1);

        PT c1 = applyRodriguesFormula(x,a,angle);
        PT c2 = applyRodriguesFormula(y,a,angle);
        PT c3 = applyRodriguesFormula(z,a,angle);

        ara[0][0] = c1.x;
        ara[1][0] = c1.y;
        ara[2][0] = c1.z;

        ara[0][1] = c2.x;
        ara[1][1] = c2.y;
        ara[2][1] = c2.z;

        ara[0][2] = c3.x;
        ara[1][2] = c3.y;
        ara[2][2] = c3.z;
    }

    // View Matrix
    void createView(PT eye,PT look,PT up)
    {
        createIdentity();
        PT z = (look-eye);
        z.normalize();
        PT x = (z^up);
        x.normalize();
        PT y = (x^z);
        y.normalize();

        ara[0][0] = x.x;
        ara[0][1] = x.y;
        ara[0][2] = x.z;

        ara[1][0] = y.x;
        ara[1][1] = y.y;
        ara[1][2] = y.z;

        ara[2][0] = -z.x;
        ara[2][1] = -z.y;
        ara[2][2] = -z.z;

        Matrix t;
        t.createTranslation(PT(-eye.x,-eye.y,-eye.z));

        *this = t*(*this);
    }

    // Projection Matrix
    void createProjection(double fovY, double aspect, double near, double far)
    {
        createIdentity();
        double fovX = fovY*aspect;
        double angleR = (fovX/2)*PI/180.0;
        double r = near * tan(angleR);
        double angleT = (fovY/2)*PI/180.0;
        double t = near * tan(angleT);
        
        ara[0][0] = near/r;
        ara[1][1] = near/t;
        ara[2][2] = -(far+near)/(far-near);
        ara[3][2] = -1;
        ara[2][3] = -2.0*far*near/(far-near);
        ara[3][3] = 0;
    }

    // print matrix in stream
    friend ostream& operator<<(ostream &out, Matrix m)
    {
        for(int i=0; i<m.dim; i++)
        {
            for(int j=0; j<m.dim; j++)
            {
                out << m.ara[i][j] << " ";
            }
            out << endl;
        }
        return out;
    }

};