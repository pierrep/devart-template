#pragma once


#include "ofMain.h"
#include "ofxJSONElement.h"
#include "ofxThreadedYouTubeVideo.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void draw();
    void update();
    void exit();
    void keyPressed (int key);

    void getNewVideo(int id);
    void getNextVideo();


    void receivedYouTubeURLEvent(ofxYouTubeURLEvent & event);


    ofxThreadedYouTubeVideo loader;

    ofVideoPlayer 	ytVideo[8];
    float ratio[8];
    bool bLoaded[8];

    int currentIndex;

    bool bPaused;
    bool bShowInfo;
    bool bVideoPlaying;
};
