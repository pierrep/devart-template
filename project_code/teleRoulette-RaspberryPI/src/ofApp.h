#pragma once


#include "ofMain.h"
#include "ofxJSONElement.h"
#include "ofxThreadedYouTubeVideo.h"
#include "ofxOMXPlayer.h"

class ofApp : public ofBaseApp, public ofxOMXPlayerListener {
public:
    void setup();
    void draw();
    void update();
    void exit();
    void keyPressed (int key);

    void getNewVideo(int id);
    void getNextVideo();
	void doLoadMovie();

    void receivedYouTubeURLEvent(ofxYouTubeURLEvent & event);
	void onVideoEnd(ofxOMXPlayerListenerEventData& e);
	void onVideoLoop(ofxOMXPlayerListenerEventData& e){ /*empty*/ };

    ofxThreadedYouTubeVideo loader;

    //ofxOMXPlayer 	ytVideo[3];
	
	vector<ofxOMXPlayer*> ytVideo; 
	    
	ofxOMXPlayerSettings settings;    
	string url[3];
    float ratio[3];
    bool bLoaded[3];

    int currentIndex;

    bool bPaused;
    bool bShowInfo;
    bool bVideoPlaying;
    bool waitForNextMovie;
    
    int movieId;
    string moviePath;
    bool bTriggerNextMovie;
    bool bDoLoadVideo;
    
    /* timer */
    float startTime;
    float endTime;
};
