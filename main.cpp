#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>
#include <math.h>
#include <string>
#include <sstream>


#define earthIndex 3
#define sunIndex 0
#define moonIndex 9
#define smudgeFactor 0.0000000005 //how off dot product can be to be considered an eclipse
#define numberSimRuns 10000000

const long double gravitationalConst = 6.6740831*(pow(10.0,-11.0)); 
const long double timestep = 120.0; //In seconds

class plnt_obj { //contains all the needed info on a planet
public:
    std::string name;
    long double time;
    long double x,y,z,vx,vy,vz,mass; 
    plnt_obj(std::string name):
        name(name),x(0),y(0),z(0),vx(0),vy(0),vz(0),mass(0){};
    friend std::ostream &operator<<(std::ostream &os, const plnt_obj &po) {
        os << po.name;
        return os;
    }
};

class SolarSystem {
public:
    std::vector<plnt_obj> planetVector; //contains all planets (and moon and sun) that make up the solar system
    friend std::ostream &operator<<(std::ostream &os, const SolarSystem &ss) {
        std::vector<plnt_obj> copyOfPlanetVector;
        copyOfPlanetVector = ss.planetVector; //could not be used in for loop with .begin() unless initialized out of loop
        for (std::vector<plnt_obj>::iterator i = (copyOfPlanetVector).begin(); i != (copyOfPlanetVector).end(); i++){
            os << "Name: "<< (*i).name << " xPosition: "<< (*i).x <<" yPosition: "<< (*i).y << " zPosition: "<< (*i).z << " xVelocity: "<< (*i).vx << " yVelocity: "<< (*i).vy << " zVelocity: "<< (*i).vz << " Mass: " << (*i).mass << std::endl;
        //for loop creates ostream for every planent in solar system
		}
        return os;
    }
    int importPlanets(SolarSystem currentSys,std::string fileLocation); //gets planents from text file
    bool updateForTimestep(); //updates planet objects value for an increased time by one timestep, true = eclipse, false= no eclipse 
    bool isSolar(); //true = solar eclipse, false = lunar eclipse
    
};

std::string timeStampToHReadble(const time_t rawtime) //converts raw time to month day hour year
{
    struct tm * dt;
    char buffer [50];
    dt = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "Month: %m | Day: %d | Hour: %H | Year: %y", dt);
    return std::string(buffer);
}

long double sciToDub(const std::string& str) { //changes text files scientific notation to doubles
    
    std::stringstream ss(str);
    long double d = 0;
    ss >> d;
    return (d);
}

long double distComp(plnt_obj planet1,plnt_obj planet2) { //computes distance between two planet objects
    //std::cout << "distComp is running!\n";
    return pow( (pow((planet2.x-planet1.x),2.0)+pow((planet2.y-planet1.y),2.0)+pow((planet2.z-planet1.z),2.0)), 0.5);
}

long double forceBetweenPlanets(plnt_obj planet1,plnt_obj planet2) { //computes force on one planet due to another planet
    //std::cout << "forceBetweenPlanets is running!\n";
    return (gravitationalConst * planet1.mass * planet2.mass)/pow(distComp(planet1,planet2), 2.0);
}

std::vector<long double> unitVecBetweenPlanets(plnt_obj p1, plnt_obj p2){ //computes unit vector between two planets
    //std::cout << "unitVecBetweenPlanets is running!\n";
    std::vector<long double> unitVector;
    long double distance = distComp(p1, p2);
    long double x_unit = (p2.x-p1.x)/distance;
    long double y_unit = (p2.y-p1.y)/distance;
    long double z_unit = (p2.z-p1.z)/distance;
    unitVector.push_back(x_unit); //all push_back to ensure x, y, z order
    unitVector.push_back(y_unit);
    unitVector.push_back(z_unit);
    //vector from p1 to p2
    return unitVector;
}

std::vector<long double> ForceVector(plnt_obj p1, plnt_obj p2){ //uses unit vector and force magnitude to compute force on p1 due to p2
    //std::cout << "ForceVector is running!\n";
    //vector from p1 to p2
    long double force = forceBetweenPlanets(p1, p2);
    std::vector<long double> unitVec = unitVecBetweenPlanets(p1,p2);
    std::vector<long double> forceVec;
    for(std::vector<long double>::iterator i=unitVec.begin() ; i != unitVec.end() ;i++){
        forceVec.push_back( (*i) * force ); //to ensure x, y, z order
    }
    return forceVec;
}

int SolarSystem::importPlanets(SolarSystem currentSys,std::string fileLocation) { //gets planets info from text file
    int planetCount = 0;
    int count = 0;
    std::ifstream ifs;
    ifs.open (fileLocation, std::ifstream::in);
    std::string value;
    while ( ifs.good() ){
        getline ( ifs, value, ',' ); // read a string until next comma:
        //std::cout << value <<std::endl;
        switch (count) {
            case 0:
                if (value == "") {
                    
                }
                else {
                    //std::cout << "NAME\n";
                    this->planetVector.push_back(plnt_obj(value));
                }
                break;
            case 1:
                //std::cout << "Time\n";
                this->planetVector[planetCount].time = stold(value);
                break;
            case 2:
                //std::cout << "Mass\n";
                this->planetVector[planetCount].mass = stold(value);
                break;
            case 3:
                //std::cout << "x\n";
                this->planetVector[planetCount].x = sciToDub(value)* 149600000000.0;
                break;
            case 4:
                //std::cout << "y\n";
                this->planetVector[planetCount].y = sciToDub(value)*149600000000.0;
                break;
            case 5:
                //std::cout << "z\n";
                this->planetVector[planetCount].z = sciToDub(value)*149600000000.0;
                break;
            case 6:
                //std::cout << "vx\n";
                this->planetVector[planetCount].vx = (sciToDub(value)*149600000000.0) / 86400.0;
                break;
            case 7:
                //std::cout << "vy\n";
                this->planetVector[planetCount].vy = (sciToDub(value)*149600000000.0) / 86400.0;
                break;
            case 8:
                //std::cout << "vz\n";
                this->planetVector[planetCount].vz = (sciToDub(value)*149600000000.0) / 86400.0;
                planetCount++;
                count = -1;
                break;
        }
        count++;
    }
    return planetCount;
}

int updateVelocity(plnt_obj &planetFrom, plnt_obj &planetTo) { //forward Euler to update velocities of planets

    std::vector<long double> forceVector = ForceVector(planetFrom,planetTo);

    planetFrom.vx += (forceVector[0] / (planetFrom.mass)) * timestep;
    planetFrom.vy += (forceVector[1] / (planetFrom.mass)) * timestep;
    planetFrom.vz += (forceVector[2] / (planetFrom.mass)) * timestep;
    
    planetTo.vx += (forceVector[0] / (planetTo.mass) )* timestep * -1.0;
    planetTo.vy += (forceVector[1] / (planetTo.mass) )* timestep * -1.0;
    planetTo.vz += (forceVector[2] / (planetTo.mass) )* timestep * -1.0;
    
    return 0;
}

int updatePosition(plnt_obj &planet) { //forward Euler to update the positions of planets
    
    planet.x += planet.vx * timestep;
    planet.y += planet.vy * timestep;
    planet.z += planet.vz * timestep;
    planet.time += timestep;
    
    return 0;
    
}

bool SolarSystem::updateForTimestep() {  //updates planet objects value for an increased time by one timestep, true = eclipse, false= no eclipse
    //std::cout << "updateForTimestep is running!\n";
    std::vector<plnt_obj>::iterator it1; //nested for loops using it1 and and an iterator 1 ahead it1 to ensure no planet is updated more than once per timestep
    for (it1 = this->planetVector.begin(); it1 != this->planetVector.end()-1; it1++) {
        std::vector<plnt_obj>::iterator it2 = it1;
        for (++it2; it2 != this->planetVector.end(); it2++) {
            //std::cout << "updateForTimestep loops!\n";
            //std::cout << "planet1: " << *it1 << " planet2: "<< *it2 << std::endl;
            
            std::vector<long double> forceVector = ForceVector(*it1,*it2); // it1 is FROM, it2 is TO
            //std::cout << "updateForTimestep after force!\n";
            updateVelocity((*it1),(*it2));
        }
    }
    for (std::vector<plnt_obj>::iterator it3 = this->planetVector.begin(); it3 != this->planetVector.end(); it3++) {
        updatePosition(*it3);
    }
    
    std::vector<long double> unitVecSunEarth = unitVecBetweenPlanets(planetVector[sunIndex],planetVector[earthIndex]); //from sun to earth
    std::vector<long double> unitVecSunMoon = unitVecBetweenPlanets(planetVector[sunIndex],planetVector[moonIndex]);
    //use two above vectors to see if the sun, moon, and earth are all in a line
    long double dotProductEarthMoonSun = (unitVecSunEarth[0]*unitVecSunMoon[0]) + (unitVecSunEarth[1]*unitVecSunMoon[1]) + (unitVecSunEarth[2]*unitVecSunMoon[2]);
    
    std::cout << std::fixed;
    std::cout << std::setprecision(10);
    //std::cout << dotProductEarthMoonSun <<std::endl;
    
    if (dotProductEarthMoonSun > (1.0-smudgeFactor)) { //tolerance for eclipse
        return true;
    }
    else {
        return false;
    }
    
    
}

bool SolarSystem::isSolar() { // its a solar eclipse if true, lunar if false
    if (distComp(planetVector[sunIndex],planetVector[earthIndex]) > distComp(planetVector[sunIndex],planetVector[moonIndex])) {
        return true;
    }
    else {
        return false;
    }
}





int main() {
    long double safetyCuttoff = 0; //Prevents infinite loop during testing
    SolarSystem currentSys; //Creates the solar system
    int numPlanets = currentSys.importPlanets(currentSys,"POI.txt"); //imports planet data from text file
    std::cout << "Solar system successfully created with the following planets: "<<std::endl;
    std::cout << currentSys;
    //std::cout << currentSys.planetVector[earthIndex] << currentSys.planetVector[moonIndex] <<std::endl;
    std::cout << "Beginning Simulation at time: \n" << timeStampToHReadble(currentSys.planetVector[0].time) <<std::endl;
    long beginTime = 0; // initialize
	int count = 0; //initialize
    while (safetyCuttoff<numberSimRuns) {
        //std::cout << currentSys<<std::endl;
        //std::cout << currentSys.planetVector[0].time <<std::endl;
        //std::cout << "Doing simulation step!\n";
        if ( currentSys.updateForTimestep() ){
            //Do more precise calculations
			if(count != 0 && (currentSys.planetVector[0].time-beginTime)>50000){ //to ensure an eclipse is only printed once, if comment says once
				count = 0; //resets after ample time ensuring eclipse is over so it is only printed once
			}
            if (currentSys.isSolar()){
				count = count+1; //once
				if(count == 1){ //once
					beginTime = currentSys.planetVector[0].time; //once
            		std::cout << "Found an solar eclipse at time: \n";
				}
            }
            else {
				count = count+1; //once
				if(count == 1){ //once
					beginTime = currentSys.planetVector[0].time; //once
                	std::cout << "Found an lunar eclipse at time: \n";
				}
            }
			if(count ==1){ //once
            	std::cout<< timeStampToHReadble(currentSys.planetVector[0].time) << std::endl;
			}
        }

        safetyCuttoff++; //Remove later
    }
    
    std::cout << "Ending Simulation at time: \n" << currentSys.planetVector[0].time <<std::endl;
}
