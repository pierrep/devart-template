#pragma once


#include "ofMain.h"
#include "ofxJSONElement.h"


class ofApp : public ofBaseApp {
public:
    void setup();
    void draw();
    void update();
    void keyPressed (int key);

    const string genRandomString(const int len);
    const string getRandomURL();
    const string getNewVideo();

    ofxJSONElement  response;
    ofVideoPlayer 	ytVideo;
    float ratio;

    bool bLoaded;
    bool bPaused;
};
