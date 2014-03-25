#include "ofApp.h"

const int totalVids = 8;

void ofApp::exit()
{
    ofRemoveListener(loader.youTubeURLEvent, this, &ofApp::receivedYouTubeURLEvent);
    ytVideo[0].stop();
    ytVideo[1].stop();
}
//------------------------------------------------------------------------------
void ofApp::setup()
{
	ofSetVerticalSync(true);
	ofBackground(0,0,0);

    ofSeedRandom(ofGetSystemTimeMicros());
    //ofSetLogLevel(OF_LOG_VERBOSE);

    currentIndex = 0;
    bVideoPlaying = false;
    bShowInfo = false;


    for(int i =0; i < totalVids;i++) {
        bLoaded[i] = false;
        ratio[i] = 0;
        ytVideo[i].setLoopState(OF_LOOP_NONE);
        getNewVideo(i);
    }

    ofAddListener(loader.youTubeURLEvent, this, &ofApp::receivedYouTubeURLEvent);

}

//------------------------------------------------------------------------------
void ofApp::update()
{

    for(int i =0; i < totalVids;i++) {
        if(ytVideo[i].isLoaded()) {
            ytVideo[i].update();

            if(currentIndex == i)
            {
                if(ytVideo[currentIndex].getPosition() > 0.95)
                {
                    getNextVideo();
                }

                if((ytVideo[i].getWidth() > 0) && (ratio[i] <= 0)) {

                    ratio[i] = (ytVideo[i].getHeight() / ytVideo[i].getWidth());
                    cout << "ratio =" << 1.0f/ratio[i] << endl;
                    cout << "video width = " << ytVideo[i].getWidth() << "  video height=" << ytVideo[i].getHeight() << endl;
                     cout << "loaded..." << i << endl;
                    bLoaded[i] = true;
                }
            }

        }
    }



}

//------------------------------------------------------------------------------
void ofApp::draw()
{
    if(!bVideoPlaying) {
        //ofDrawBitmapString("LOADING VIDEOS", ofGetWidth()/2 - 20,ofGetHeight()/2);
    }

    ofSetHexColor(0xFFFFFF);
    if(ratio[currentIndex] != 0) {
        if(ofGetWidth()*ratio[currentIndex] < ofGetHeight()) {
            ytVideo[currentIndex].draw(0,(ofGetHeight() - ofGetWidth()*ratio[currentIndex]) /2.0f,ofGetWidth(),ofGetWidth()*ratio[currentIndex]);
        }
        else {
            ytVideo[currentIndex].draw(0,0,ofGetWidth(),ofGetWidth()*ratio[currentIndex]);
        }
    } else {
        ;//cout << "RATIO is zero!" << endl;
    }

    if(bShowInfo) {
        ofPushStyle();
        ofSetColor(255,0,0);
        ofDrawBitmapString("Current Index = "+ofToString(currentIndex)+ " Current ratio = "+ofToString(ratio[currentIndex])+ " position = "+ofToString(ytVideo[currentIndex].getPosition()), 20,ofGetHeight()-20);
        ofPopStyle();
    }
}

//--------------------------------------------------------------
void ofApp::receivedYouTubeURLEvent(ofxYouTubeURLEvent & event){
    //cout << "URLEvent: " << event.url << endl;

    if((bVideoPlaying == false) && (event.id == currentIndex)) {
        ytVideo[currentIndex].loadMovie(event.url);
        ytVideo[currentIndex].play();
        bVideoPlaying = true;
        cout << "first movie loaded" << endl;
    }
    else {
        cout << "next movie loaded, id:" << event.id << endl;
        ytVideo[event.id].loadMovie(event.url);
        //ytVideo[event.id].update();
        ytVideo[event.id].setPaused(true);
    }
}

//------------------------------------------------------------------------------
void ofApp::getNewVideo(int id)
{
    loader.loadYouTubeURL("",id); //empty string = random url
}

//--------------------------------------------------------------
void ofApp::getNextVideo()
{
    ratio[currentIndex] = 0;
    bLoaded[currentIndex] = false;
    ytVideo[currentIndex].setPaused(true);
    getNewVideo(currentIndex);

    currentIndex++;
    if(currentIndex >= totalVids) currentIndex = 0;

    ytVideo[currentIndex].play();

}

//--------------------------------------------------------------
void ofApp::keyPressed (int key)
{
    switch(key){
        case ' ':
            getNextVideo();
        break;
        case't':
            cout << "Total Frames=" << ytVideo[currentIndex].getTotalNumFrames() << endl;
        break;
        case'p':
            bPaused = !bPaused;
            ytVideo[currentIndex].setPaused(bPaused);
        break;
        case 'f':
            bShowInfo = !bShowInfo;
        break;
    }
}


