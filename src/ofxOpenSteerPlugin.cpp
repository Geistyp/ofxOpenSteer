#include "ofxOpenSteerPlugin.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

// we not use OpenSteerDemo.cpp
// 2 extern define here
namespace OpenSteer {
    bool updatePhaseActive = false;
    bool drawPhaseActive = false;
}

ofxOpenSteerPlugin::ofxOpenSteerPlugin(){
    setup();
}
ofxOpenSteerPlugin::~ofxOpenSteerPlugin(){
	exit();
}
void ofxOpenSteerPlugin::setup(){
}

void ofxOpenSteerPlugin::update(){
	update(ofGetElapsedTimef(), 1.f/ofGetFrameRate());
}
void ofxOpenSteerPlugin::update(float currentTime, float elapsedTime){
	updatePhaseActive = true; //XXX ideally should be moved to main app's events, kepping it here so app can be cleaner. it's only safe while single threaded
	for (VehicleIterator i = vehicles.begin(); i != vehicles.end(); i++) {
		(*i)->update (currentTime, elapsedTime);
	}
	updatePhaseActive = false; //XXX ideally should be moved to main app's events, kepping it here so app can be cleaner. it's only safe while single threaded
}
void ofxOpenSteerPlugin::draw(){
	drawPhaseActive = true; //XXX ideally should be moved to main app's events, kepping it here so app can be cleaner. it's only safe while single threaded
	for (VehicleIterator i = vehicles.begin(); i != vehicles.end(); i++) {
		(*i)->draw();
        //ofCircle((*i)->getPosition(), 1);
	}
	drawPhaseActive = false; //XXX ideally should be moved to main app's events, kepping it here so app can be cleaner. it's only safe while single threaded
}
void ofxOpenSteerPlugin::exit(){
	// clear vehicles
	while (vehicles.size() > 0){
		const AbstractVehicle* v = (AbstractVehicle*)vehicles.back();
		vehicles.pop_back();
		delete v;
	}
}

void ofxOpenSteerPlugin::addVehicle(ofxOpenSteerVehicle* v){
	vehicles.push_back(v);
}
void ofxOpenSteerPlugin::removeVehicle(ofxOpenSteerVehicle* v){
	vehicles.erase(std::remove(vehicles.begin(), vehicles.end(), v), vehicles.end());
}
VehicleGroup ofxOpenSteerPlugin::getVehicles(){
	return vehicles;
}

