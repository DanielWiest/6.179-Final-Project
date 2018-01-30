#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>
#include <math.h>
#include <string>


const long gravitationalConst = 6.67408*pow(10,-11); //
//const double timestep = 10; //In second

class plnt_obj {
public:
    std::string name;
    long time;
    long x,y,z,vx,vy,vz,mass;
    plnt_obj(std::string name):
        name(name),x(0),y(0),z(0),vx(0),vy(0),vz(0),mass(0){};
    plnt_obj(double xPos,double yPos,double zPos, double xVel, double yVel, double zVel,double massInp):
        name(""), x(xPos),y(yPos),z(zPos),vx(xVel),vy(yVel),vz(zVel),mass(massInp){};
    plnt_obj():
        name(""),x(0),y(0),z(0),vx(0),vy(0),vz(0),mass(0){};
};

class SolarSystem {
public:
    std::vector<plnt_obj> planetVector;
    
};


double distComp(plnt_obj planet1,plnt_obj planet2) {
    return pow( (pow((planet2.x-planet1.x),2)+pow((planet2.y-planet1.y),2)+pow((planet2.z-planet1.z),2)), 0.5);
}

double forceBetweenPlanets(plnt_obj planet1,plnt_obj planet2) {
    return (gravitationalConst*planet1.mass*planet2.mass)/distComp(planet1,planet2);
}

int importPlanets(SolarSystem currentSys,std::string fileLocation) {
    int planetCount = 0;
    int count = 0;
    std::ifstream ifs;
    ifs.open (fileLocation, std::ifstream::in);
    std::string value;
    while ( ifs.good() ){
        getline ( ifs, value, ',' ); // read a string until next comma:
        std::cout << value <<std::endl;
        switch (count) {
            case 0:
                if (value == "") {
                    
                }
                else {
                    std::cout << "NAME\n";
                    currentSys.planetVector.push_back(plnt_obj(value));
                }
                break;
            case 1:
                std::cout << "Time\n";
                currentSys.planetVector[planetCount].time = stol(value);
                break;
            case 2:
                std::cout << "Mass\n";
                currentSys.planetVector[planetCount].mass = stol(value);
                break;
            case 3:
                std::cout << "x\n";
                currentSys.planetVector[planetCount].x = stol(value);
                break;
            case 4:
                std::cout << "y\n";
                currentSys.planetVector[planetCount].y = stol(value);
                break;
            case 5:
                std::cout << "z\n";
                currentSys.planetVector[planetCount].z = stol(value);
                break;
            case 6:
                std::cout << "vx\n";
                currentSys.planetVector[planetCount].vx = stol(value);
                break;
            case 7:
                std::cout << "vy\n";
                currentSys.planetVector[planetCount].vy = stol(value);
                break;
            case 8:
                std::cout << "vz\n";
                currentSys.planetVector[planetCount].vz = stol(value);
                planetCount++;
                count = -1;
                break;
        }
        count++;
    }
    return planetCount;
}

void printPlanetNames(SolarSystem currentSys) {
    for (std::vector<plnt_obj>::iterator i = currentSys.planetVector.begin(); i != currentSys.planetVector.end(); i++){
        std::cout << (*i).name << std::endl;
    }
}




int main() {
    SolarSystem currentSys; //Creates the solar system
    int numPlanets = importPlanets(currentSys,"POI.txt");
    printPlanetNames(currentSys);
    
    
    
}
