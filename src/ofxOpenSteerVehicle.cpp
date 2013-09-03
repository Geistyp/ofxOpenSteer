#include "ofxOpenSteerVehicle.h"

#include "Draw.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

// No OpenSteerDemo but need annotation here
// add extern definition
bool OpenSteer::enableAnnotation = true;

AVGroup ofxOpenSteerVehicle::neighbors;

ofxOpenSteerVehicle::ofxOpenSteerVehicle(){
    color = gGray30;
    reset();
}
ofxOpenSteerVehicle::~ofxOpenSteerVehicle(){
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
	recordTrailVertex (curTime, position());
}
void ofxOpenSteerVehicle::draw(){
    
    // NOTE:
    // After 0.8.0 oF use OpenGLES2.0 and draw everything in shader
    // so should use more oF draw method for more compatiblility
    
	//drawBasic3dSphericalVehicle (*this, color);
    
    AbstractVehicle& vehicle = *this;
    
    // "aspect ratio" of body (as seen from above)
    const float x = 0.5f;
    const float y = sqrtXXX (1 - (x * x));
    
    // radius and position of vehicle
    const float r = vehicle.radius();
    const Vec3& p = vehicle.position();
    
    // body shape parameters
    const Vec3 f = r * vehicle.forward();
    const Vec3 s = r * vehicle.side() * x;
    const Vec3 u = r * vehicle.up() * x * 0.5f;
    const Vec3 b = r * vehicle.forward() * -y;
    
    // vertex positions
    const Vec3 nose   = p + f;
    const Vec3 side1  = p + b - s;
    const Vec3 side2  = p + b + s;
    const Vec3 top    = p + b + u;
    const Vec3 bottom = p + b - u;
    
    // colors
    const float j = +0.05f;
    const float k = -0.05f;
    const Color color1 = color + Color(j, j, k);
    const Color color2 = color + Color(j, k, j);
    const Color color3 = color + Color(k, j, j);
    const Color color4 = color + Color(k, j, k);
    const Color color5 = color + Color(k, k, j);
    
#define OpenSteerToOfColor(c) ofSetColor(c.r()*255, c.g()*255, c.b()*255)
#define OpenSteerToOfTriangle(a, b, c) ofTriangle(a.x, a.y, a.z, b.x, b.y, b.z, c.x, c.y, c.z)
    // draw body
    OpenSteerToOfColor(color1);
    OpenSteerToOfTriangle(nose, side1, top);
    OpenSteerToOfColor(color2);
    OpenSteerToOfTriangle(nose, top, side2);
    OpenSteerToOfColor(color3);
    OpenSteerToOfTriangle(nose, bottom, side1);
    OpenSteerToOfColor(color4);
    OpenSteerToOfTriangle(nose, side2, bottom);
    OpenSteerToOfColor(color5);
    OpenSteerToOfTriangle(side1, side2, top);
    OpenSteerToOfColor(color5);
    OpenSteerToOfTriangle(side2, side1, bottom);
	//annotationVelocityAcceleration ();
	//drawTrail();
    
    Color tickColor = grayColor(0.7);
    Color trailColor = gWhite;
    
    int index = trailIndex;
    for (int j = 0; j < trailVertexCount; j++)
    {
        // index of the next vertex (mod around ring buffer)
        const int next = (index + 1) % trailVertexCount;
        
        // "tick mark": every second, draw a segment in a different color
        const int tick = ((trailFlags [index] & 2) ||
                          (trailFlags [next] & 2));
        const Color color = tick ? tickColor : trailColor;
        
        // draw every other segment
        if (trailFlags [index] & 1)
        {
            if (j == 0)
            {
                // draw segment from current position to first trail point
                ofSetColor(color.r()*255, color.g()*255, color.b()*255, 255);
                ofLine(curPosition.x, curPosition.y, curPosition.z,
                       trailVertices[index].x, trailVertices[index].y, trailVertices[index].z);
            }
            else
            {
                // draw trail segments with opacity decreasing with age
                const float minO = 0.05f; // minimum opacity
                const float fraction = (float) j / trailVertexCount;
                const float opacity = (fraction * (1 - minO)) + minO;
                ofSetColor(color.r()*255, color.g()*255, color.b()*255, opacity*255);
                ofLine(trailVertices[index].x, trailVertices[index].y, trailVertices[index].z,
                       trailVertices[next].x, trailVertices[next].y, trailVertices[next].z);
            }
        }
        index = next;
    }
    ofSetColor(255);
}
Vec3 ofxOpenSteerVehicle::getSteeringForce(const float elapsedTime){
	return Vec3(0, 0, 0);
}

ofVec3f ofxOpenSteerVehicle::getPosition()
{
	return toOf(position());
}
