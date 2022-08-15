#include<bits/stdc++.h>
#include <GL/glut.h>
using namespace std;

#define pi (2*acos(0.0))

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

    /** arithemtic operations  **/

	PT operator +(PT b)  {return PT(x+b.x,y+b.y, z+b.z);}
    PT operator -(PT b)  {return PT(x-b.x,y-b.y, z-b.z);}
	PT operator *(double b)  {return PT(x*b,y*b, z*b);}
	PT operator /(double b)  {return PT(x/b,y/b, z/b);}
	double operator *(PT b)  {return x*b.x+y*b.y+z*b.z;} // DOT PRODUCT
	PT operator ^(PT b)  {return PT(y*b.z-z*b.y, z*b.x-x*b.z, x*b.y-y*b.x);} // CROSS PRODUCT

    /** functions  **/

    double length() {return sqrt(x*x+y*y+z*z);}

    void normalize()
    {
        double len = length();
        x /= len;
        y /= len;
        z /= len;
    }

    /** streams  **/

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

struct Color{
    double r,g,b;
    Color()
    {
        r = g = b = 0.0;
    }
};

struct Light{
    PT pos;
    Color color;

    void draw()
    {
        glPointSize(5);
        glBegin(GL_POINTS);
        glColor3f(color.r, color.g, color.b);
        glVertex3f(pos.x, pos.y, pos.z);
        glEnd();
        glPointSize(1);
    }

    // input stream
    friend istream& operator>>(istream &in, Light &l)
    {
        in >> l.pos.x >> l.pos.y >> l.pos.z;
        in >> l.color.r >> l.color.g >> l.color.b;
        return in;
    }

};

struct Ray{
    PT origin, dir;
    
    Ray(PT origin, PT dir){
        this->origin = origin;
        dir.normalize();
        this->dir = dir;
    }

    // stream
    friend ostream& operator<<(ostream &out, Ray r)
    {
        out << "Origin : " << r.origin << ", Direction : " << r.dir;
        return out;
    }
};


class Object {
public:
		PT reference_point;
		double height, width, length;
		Color color;
		vector <double> coefficients; // ambient, diffuse,specular, reflection coefficients
		int shine; // exponent term of specular component
		
		Object(){
            coefficients = vector <double> (4,0);
		}

		void setColor(Color color){
            this->color = color;
        }
		
		void setShine(int shine){
            this->shine = shine;
        }

		void setCoefficients(vector<double> coefficients){
            this->coefficients = coefficients;
        }

        virtual void draw() = 0;
		virtual double intersect(Ray ray, Color color, int level) = 0;

        // destructor
        virtual ~Object(){}
};

double determinant(double ara[3][3]){
	double v1 = ara[0][0] * (ara[1][1] * ara[2][2] - ara[1][2] * ara[2][1]);
	double v2 = ara[0][1] * (ara[1][0] * ara[2][2] - ara[1][2] * ara[2][0]);
	double v3 = ara[0][2] * (ara[1][0] * ara[2][1] - ara[1][1] * ara[2][0]);
	return v1 - v2 + v3;
}


struct Triangle: public Object
{
    PT a,b,c;

    Triangle(){

    }

    Triangle(PT a, PT b, PT c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
    }

    virtual void draw(){
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        }
        glEnd();
    }

    virtual double intersect(Ray ray, Color color, int level){

        double betaMat[3][3] = {
				{a.x - ray.origin.x, a.x - c.x, ray.dir.x},
				{a.y - ray.origin.y, a.y - c.y, ray.dir.y},
				{a.z - ray.origin.z, a.z - c.z, ray.dir.z}
			};
			double gammaMat[3][3] = {
				{a.x - b.x, a.x - ray.origin.x, ray.dir.x},
				{a.y - b.y, a.y - ray.origin.y, ray.dir.y},
				{a.z - b.z, a.z - ray.origin.z, ray.dir.z}
			};
			double tMat[3][3] = {
				{a.x - b.x, a.x - c.x, a.x - ray.origin.x},
				{a.y - b.y, a.y - c.y, a.y - ray.origin.y},
				{a.z - b.z, a.z - c.z, a.z - ray.origin.z}
			};
			double AMat[3][3] {
				{a.x - b.x, a.x - c.x, ray.dir.x},
				{a.y - b.y, a.y - c.y, ray.dir.y},
				{a.z - b.z, a.z - c.z, ray.dir.z}
			};

        double Adet = determinant(AMat);
        double beta = determinant(betaMat) / Adet;
        double gamma = determinant(gammaMat) / Adet;
        double t = determinant(tMat) / Adet;

        if (beta + gamma < 1 && beta > 0 && gamma > 0 && t > 0){
            return t;
        }
        else{
            return -1;
        }
    }

    // input stream
    friend istream& operator>>(istream &in, Triangle &t)
    {
        in >> t.a >> t.b >> t.c; // 3 vertices
        in >> t.color.r >> t.color.g >> t.color.b; // color
        for(int i = 0; i < 4; i++) in >> t.coefficients[i];
        in >> t.shine;
        return in;
    }
};

struct Sphere : public Object{

        Sphere(){
        }

		Sphere(PT center, double radius){
			reference_point = center;
			length = radius;
		}

		virtual void draw(){
            int stacks = 30;
			int slices = 20;

			PT points[100][100];
			int i, j;
			double h, r;
			// generate points
			for (i = 0; i <= stacks; i++)
			{
				h = length * sin(((double)i / (double)stacks) * (pi / 2));
				r = length * cos(((double)i / (double)stacks) * (pi / 2));
				for (j = 0; j <= slices; j++)
				{
					points[i][j].x = r * cos(((double)j / (double)slices) * 2 * pi);
					points[i][j].y = r * sin(((double)j / (double)slices) * 2 * pi);
					points[i][j].z = h;
				}
			}
			//draw quads using generated points
			for (i = 0; i < stacks; i++)
			{
				glPushMatrix();
				glTranslatef(reference_point.x, reference_point.y, reference_point.z);
				glColor3f(color.r, color.g, color.b);
				for (j = 0; j < slices; j++)
				{
					glBegin(GL_QUADS);
					{
						//upper hemisphere
						glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
						glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
						glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
						glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
						//lower hemisphere
						glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
						glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
						glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
						glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
					}
					glEnd();
				}
				glPopMatrix();
			}
		}

        virtual double intersect(Ray ray, Color color, int level){

            ray.origin = ray.origin - reference_point; // adjust ray origin
            
            double a = 1;
            double b = 2 * (ray.dir*ray.origin);
            double c = (ray.origin*ray.origin) - (length*length);

            double discriminant = pow(b, 2) - 4 * a * c;
            if (discriminant < 0){
                return -1;
            }
            else{
                double t1 = (-b - sqrt(discriminant)) / (2 * a);
                double t2 = (-b + sqrt(discriminant)) / (2 * a);
                if (t1 > 0){
                    return t1;
                }
                else if (t2 > 0){
                    return t2;
                }
                else{
                    return -1;
                }
            }
        }

        // input stream
        friend std::istream& operator>>(std::istream& in, Sphere& s){
            in >> s.reference_point >> s.length; // center and radius
            in >> s.color.r >> s.color.g >> s.color.b; // color
            for(int i = 0; i < 4; i++) in >> s.coefficients[i];
            in >> s.shine;
            return in;
        }
};

struct Floor : public Object{

    int tiles;

    Floor(){
        tiles = 1;
    }

    Floor(int floorWidth,int tileWidth){
        tiles = floorWidth / tileWidth;
        reference_point = PT(-floorWidth / 2, -floorWidth / 2, 0);
        length = tileWidth;
    }

    virtual void draw(){
        for (int i = 0; i < tiles; i++)
		{
			for (int j = 0; j < tiles; j++)
			{
				if (((i + j) % 2) == 0) glColor3f(1, 1, 1);
				else glColor3f(0, 0, 0);

				glBegin(GL_QUADS);
				{
					glVertex3f(reference_point.x + i * length, reference_point.y + j * length, 0);
					glVertex3f(reference_point.x + (i + 1) * length, reference_point.y + j * length, 0);
					glVertex3f(reference_point.x + (i + 1) * length, reference_point.y + (j + 1) * length, 0);
					glVertex3f(reference_point.x + i * length, reference_point.y + (j + 1) * length, 0);
				}
				glEnd();
			}
		}
    }

    virtual double intersect(Ray ray, Color color, int level){
        PT normal = PT(0, 0, 1);
        double dotP = normal * ray.dir;
        double t = -(normal * ray.origin) / dotP;

        PT p = ray.origin + ray.dir * t;

        if(p.x <= reference_point.x || p.x >= abs(reference_point.x) && p.y <= reference_point.y && p.y >= abs(reference_point.y)){
            return -1;
        }
        
        return t;
    }
};