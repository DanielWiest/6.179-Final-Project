#include <iostream>
#include <cmath>

const long gravitationalConst = 6.67408*pow(10,-11); //
const double timestep = 10; //In seconds

class plnt_obj {
public:
    double x,y,z,vx,vy,vz,mass;
    plnt_obj(double xPos,double yPos,double zPos, double xVel, double yVel, double zVel,double massInp):
        x(xPos),y(yPos),z(zPos),vx(xVel),vy(yVel),vz(zVel),mass(massInp){};
};

double distComp(plnt_obj planet1,plnt_obj planet2) {
    return pow( (pow((planet2.x-planet1.x),2)+pow((planet2.y-planet1.y),2)+pow((planet2.z-planet1.z),2)), 0.5);
}

double forceBetweenPlanets(plnt_obj planet1,plnt_obj planet2) {
    return (gravitationalConst*planet1.mass*planet2.mass)/distComp(planet1,planet2);
}



int main(int argc, const char * argv[]) {

}
