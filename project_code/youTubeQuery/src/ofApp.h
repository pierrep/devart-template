#pragma once


#include "ofMain.h"
#include "ofxJSONElement.h"
#include "ofxThreadedVideo.h"


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
    ofVideoPlayer 	ytVideo[2];
    int currentIndex;
    float ratio;

    bool bLoaded[2];
    bool bPaused;
};
