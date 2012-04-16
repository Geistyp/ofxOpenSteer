#include "ofxOpenSteerVehicle.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

AVGroup ofxOpenSteerVehicle::neighbors;

ofxOpenSteerVehicle::ofxOpenSteerVehicle(){
    proximityDB = NULL;
	proximityToken = NULL;
	path = NULL;
    reset();
}
ofxOpenSteerVehicle::~ofxOpenSteerVehicle(){
	proximityDB = NULL;
	path = NULL;
	if(proximityToken) delete proximityToken;
}

void ofxOpenSteerVehicle::reset(){
	SimpleVehicle::reset();  
	clearTrailHistory();
}
void ofxOpenSteerVehicle::update(){
	update(ofGetElapsedTimef(), 1.f/ofGetFrameRate());
}
void ofxOpenSteerVehicle::update(float curTime, float elapsedTime){
	applySteeringForce (getSteeringForce (elapsedTime), elapsedTime);
	if(proximityToken) proximityToken->updateForNewPosition (position());
	
	recordTrailVertex (curTime, position());	
}
void ofxOpenSteerVehicle::draw(){    
	
	drawBasic3dSphericalVehicle (*this, gGray30);
	annotationVelocityAcceleration ();
	drawTrail();
}
Vec3 ofxOpenSteerVehicle::getSteeringForce(const float elapsedTime){
	return Vec3(0, 0, 0);
}

void ofxOpenSteerVehicle::setProximityDatabase(ProximityDatabase* db){
	proximityDB = db;
	proximityToken = proximityDB->allocateToken (this);
}
ProximityDatabase* ofxOpenSteerVehicle::getProximityDatabase(){
	return proximityDB;
}
ProximityToken* ofxOpenSteerVehicle::getProximityToken(){
	return proximityToken;
}

void ofxOpenSteerVehicle::setPath(ofxOpenSteerPath* p){
	path = p;
}
ofxOpenSteerPath* ofxOpenSteerVehicle::getPath(){
	return path;
}
void ofxOpenSteerVehicle::unsetPath(){
	path = NULL;
}

void ofxOpenSteerVehicle::addObstacle(Obstacle* o){
	obstacles.push_back(o);
}
void ofxOpenSteerVehicle::removeObstacle(Obstacle* o){
    obstacles.erase(std::remove(obstacles.begin(), obstacles.end(), o), obstacles.end());
}
ObstacleGroup ofxOpenSteerVehicle::getObstacles(){
    return obstacles;
}