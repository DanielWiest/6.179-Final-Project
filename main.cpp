#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>
#include <math.h>
#include <string>


#define earthIndex 3
#define sunIndex 0
#define moonIndex 9
#define smudgeFactor 0.0000000005

const long gravitationalConst = 6.67408*pow(10,-11); //
const double timestep = 120; //In seconds

class plnt_obj {
public:
    std::string name;
    long time;
    long x,y,z,vx,vy,vz,mass;
    plnt_obj(std::string name):
        name(name),x(0),y(0),z(0),vx(0),vy(0),vz(0),mass(0){};
    plnt_obj(long xPos,long yPos,long zPos, long xVel, long yVel, long zVel,long massInp):
        name(""), x(xPos),y(yPos),z(zPos),vx(xVel),vy(yVel),vz(zVel),mass(massInp){};
    plnt_obj():
        name(""),x(0),y(0),z(0),vx(0),vy(0),vz(0),mass(0){};
};

class SolarSystem {
public:
    std::vector<plnt_obj> planetVector;
    friend std::ostream &operator<<(std::ostream &os, const SolarSystem &ss) {
        std::vector<plnt_obj> currentVector = ss.planetVector;
            for (std::vector<plnt_obj>::iterator i = currentVector.begin(); i != currentVector.end(); i++){
                os << (*i).name << std::endl;
            }
            return os;
    }
    
};

long distComp(plnt_obj planet1,plnt_obj planet2) {
    std::cout << "distComp is running!\n";
    return pow( (pow((planet2.x-planet1.x),2)+pow((planet2.y-planet1.y),2)+pow((planet2.z-planet1.z),2)), 0.5);
}

long forceBetweenPlanets(plnt_obj planet1,plnt_obj planet2) {
    std::cout << "forceBetweenPlanets is running!\n";
    return (gravitationalConst*planet1.mass*planet2.mass)/distComp(planet1,planet2);
}

std::vector<long> unitVecBetweenPlanets(plnt_obj p1, plnt_obj p2){
    std::cout << "unitVecBetweenPlanets is running!\n";
    long distance = distComp(p1, p2);
    std::vector<long> unitVector;
    long x_unit = (p1.x-p2.x)/distance;
    long y_unit = (p1.y-p2.y)/distance;
    long z_unit = (p1.z-p2.z)/distance;
    unitVector.push_back(x_unit);
    unitVector.push_back(y_unit);
    unitVector.push_back(z_unit);
    //vector from p2 to p1
    return unitVector;
}

std::vector<long> ForceVector(plnt_obj p1, plnt_obj p2){
    std::cout << "ForceVector is running!\n";
    //vector from p2 to p1
    long force = forceBetweenPlanets(p1, p2);
    std::vector<long> unitVec = unitVecBetweenPlanets(p1,p2);
    std::vector<long> forceVec;
    for(std::vector<long>::iterator i=unitVec.begin() ; i != unitVec.end() ;i++){
        forceVec.push_back( (*i) * force);
    }
    return forceVec;
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

int updatePositionAndVelocity(std::vector<long> forceVector,plnt_obj planetFrom, plnt_obj planetTo) {
    std::cout << "updatePositionAndVelocity is running!\n";
    planetFrom.vx += forceVector[0] / (planetFrom.mass) * timestep;
    planetFrom.vy += forceVector[1] / (planetFrom.mass) * timestep;
    planetFrom.vz += forceVector[2] / (planetFrom.mass) * timestep;
    planetFrom.x += planetFrom.vx * timestep;
    planetFrom.y += planetFrom.vy * timestep;
    planetFrom.z += planetFrom.vz * timestep;
    return 0;
}

bool updateForTimestep(SolarSystem currentSys) {
    std::cout << "updateForTimestep is running!\n";
    std::vector<plnt_obj> planetVector = currentSys.planetVector;
    for (std::vector<plnt_obj>::iterator i = planetVector.begin(); i != planetVector.end()-1; ++i) {
        for (std::vector<plnt_obj>::iterator j = i+1; j != planetVector.end(); ++j) {
            std::cout << "updateForTimestep loops!\n";
            std::vector<long> forceVector = ForceVector(*j,*i); // i is FROM, j is TO
            std::cout << "updateForTimestep after force!\n";
            updatePositionAndVelocity(forceVector,*i,*j);

        }
    }
    std::vector<long> unitVecSunEarth = unitVecBetweenPlanets(planetVector[earthIndex], planetVector[sunIndex]); //to p1 from p2
    std::vector<long> unitVecSunMoon = unitVecBetweenPlanets(planetVector[moonIndex], planetVector[sunIndex]);
    long dotProductEarthMoonSun = unitVecSunEarth[0]*unitVecSunMoon[0] + unitVecSunEarth[1]*unitVecSunMoon[1] + unitVecSunEarth[2]*unitVecSunMoon[2];
    if (dotProductEarthMoonSun > 1.0-smudgeFactor) {
        return true;
    }
    else {
        return false;
    }
    
    
}





int main() {
    long safetyCuttoff = 0; //Prevents infinite loop during testing
    SolarSystem currentSys; //Creates the solar system
    int numPlanets = importPlanets(currentSys,"POI.txt");
    std::cout << currentSys;
    std::cout << "Doing simulation step!\n";
    
    while (safetyCuttoff<1000000) {
        std::cout << "Doing simulation step!\n";
        if (updateForTimestep(currentSys)){
            //Do more precise calculations
            std::cout << "Found an eclipse!\n";
        }

        safetyCuttoff++; //Remove later
    }
    
    
    
}
