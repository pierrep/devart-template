#include "ofApp.h"

const int totalVids = 3;

void ofApp::exit()
{
    ofRemoveListener(loader.youTubeURLEvent, this, &ofApp::receivedYouTubeURLEvent);
}

void ofApp::onVideoEnd(ofxOMXPlayerListenerEventData& e)
{
	ofLogVerbose(__func__) << " RECEIVED";
	cout << "video END!" << endl;
	bTriggerNextMovie = true;
}

//------------------------------------------------------------------------------
void ofApp::setup()
{
	//ofSetVerticalSync(true);
	ofSetFrameRate(25);
	ofBackground(0);
    ofSeedRandom(ofGetSystemTimeMicros());
    //ofSetLogLevel(OF_LOG_VERBOSE);

    currentIndex = 0;
    bVideoPlaying = false;
    waitForNextMovie = false;
    bShowInfo = true;
    bDoLoadVideo = false;
    bTriggerNextMovie = false;


    for(int i =0; i < totalVids;i++) {
        bLoaded[i] = false;
        ratio[i] = 0;
        
		settings.useHDMIForAudio = false;	//default true
		settings.enableLooping = false;		//default true
		settings.enableTexture = false;		//default true
		settings.listener = this;			//this app extends ofxOMXPlayerListener so it will receive events ;
		if(!settings.enableTexture) {
			settings.displayRect.width=320;
			settings.displayRect.height=240;
			settings.displayRect.x =0;
			settings.displayRect.y =0;
			
		}
		
		ofxOMXPlayer* player = new ofxOMXPlayer();
		player->setup(settings);
		ytVideo.push_back(player);		
			
        getNewVideo(i);
    }

    ofAddListener(loader.youTubeURLEvent, this, &ofApp::receivedYouTubeURLEvent);

}

//------------------------------------------------------------------------------
void ofApp::update()
{
	if(bTriggerNextMovie) {
		getNextVideo();		
	}
	
	if(bDoLoadVideo) {
		//doLoadMovie();
	}

	if(waitForNextMovie) {
		cout << "wait for next movie" << endl;
		if( (ytVideo[currentIndex]->isPlaying()) && (ytVideo[currentIndex]->isPaused()) ) {
			ytVideo[currentIndex]->setPaused(false);
			waitForNextMovie = false;			
		}		
	}
	
    for(int i =0; i < totalVids;i++) {
        if(ytVideo[i]->isPlaying()) {

            if(currentIndex == i)
            {
                //if(ytVideo[currentIndex]->getPosition() > 0.95)
                //    getNextVideo();

                if((ytVideo[i]->getWidth() > 0) && (ratio[i] <= 0)) {
                    ratio[i] = (ytVideo[i]->getWidth() / ytVideo[i]->getHeight());
                }
            }

        }
    }



}

//------------------------------------------------------------------------------
void ofApp::draw()
{
	ofSetColor(0,0,0);
	ofRect(0,0,ofGetWidth(),ofGetHeight());
	
    ofSetColor(255,255,255);
    
    if(bVideoPlaying) {
		if(ytVideo[currentIndex]->isTextureEnabled) {
			ytVideo[currentIndex]->draw(0, 0, 320, 240);
		}     
    } else {
        ofDrawBitmapString("LOADING VIDEOS", ofGetWidth()/2 - 30,ofGetHeight()/2);		
	}
	
	ofDrawBitmapString("currentIndex="+ofToString(currentIndex),0,300);
	for(int i=0; i< totalVids;i++)
	{
		if(currentIndex == i) ofSetColor(255,0,0);
		else ofSetColor(255,255,255);
		
		string paused;
		if(ytVideo[i]->isPaused()) paused = "yes";
		else paused ="no ";
		string playing;
		if(ytVideo[i]->isPlaying()) playing = "yes";
		else playing = "no ";
		
		ofDrawBitmapString("Ratio="+ofToString(ratio[i])+" paused="+paused+" playing="+playing+" URL"+ofToString(i)+"="+url[i],0,320+i*20);
	} 

    if(bShowInfo) {
		//if(ratio[currentIndex] != 0) {
		ofDrawBitmapStringHighlight(ytVideo[currentIndex]->getInfo(),330,20,ofColor(ofColor::black,90),ofColor::yellow);        
		//}
    }
}


//--------------------------------------------------------------
void ofApp::doLoadMovie()
{
   if(bVideoPlaying == false) {
        url[movieId] = moviePath;
        
        //delete ytVideo[event.id];
        //ytVideo[event.id] = new ofxOMXPlayer();
        settings.videoPath = moviePath;
        //ytVideo[movieId]->setup(settings);
        cout << "loadMovie" << endl;
        ytVideo[movieId]->loadMovie(moviePath);
        cout << "setPaused" << endl;
        ytVideo[movieId]->setPaused(true);
        
        if(movieId >= totalVids - 1) {
			ytVideo[0]->setPaused(false);
			bVideoPlaying = true;
			cout << "movies loaded" << endl;
		}
    }
    else {
        url[movieId] = moviePath;
        //delete ytVideo[event.id];
        //ytVideo[event.id] = new ofxOMXPlayer();
        settings.videoPath = moviePath;
        //ytVideo[movieId]->setup(settings);        
        ytVideo[movieId]->loadMovie(moviePath);
        cout << "next movie loaded, id:" << movieId << endl;
        ytVideo[movieId]->setPaused(true);
    }
    
    bDoLoadVideo = false;	
}


//--------------------------------------------------------------
void ofApp::receivedYouTubeURLEvent(ofxYouTubeURLEvent & event){
    //cout << "URLEvent: " << event.url << endl;

	moviePath = event.url;
	movieId = event.id;
	bDoLoadVideo = true;
	doLoadMovie();
 
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
    url[currentIndex] = "";
    ytVideo[currentIndex]->setPaused(true);
	//ytVideo[currentIndex]->close();
    
    getNewVideo(currentIndex);

    currentIndex++;
    if(currentIndex >= totalVids) currentIndex = 0;
    
    if( (ytVideo[currentIndex]->isPlaying()) && (ytVideo[currentIndex]->isPaused()) ) {
		ytVideo[currentIndex]->setPaused(false);
	}
	else waitForNextMovie = true;
	
	bTriggerNextMovie = false;
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key)
{
    switch(key){
        case ' ':
            bTriggerNextMovie = true;            
        break;
        case't':
            cout << "Total Frames=" << ytVideo[currentIndex]->getTotalNumFrames() << endl;
        break;
        case'p':
            bPaused = !bPaused;
            ytVideo[currentIndex]->setPaused(bPaused);
        break;
        case 'f':
            bShowInfo = !bShowInfo;
        break;
    }
}


