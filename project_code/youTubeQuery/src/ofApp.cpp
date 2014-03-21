#include "ofApp.h"

//------------------------------------------------------------------------------
void ofApp::setup()
{
	ofSetVerticalSync(true);
	ofBackground(0);
    ofSeedRandom(ofGetSystemTimeMicros());
    ofSetLogLevel(OF_LOG_VERBOSE);

    ratio = 0;
    currentIndex = 0;

    bLoaded[0] = false;
    bLoaded[1] = false;

    ytVideo[0].setLoopState(OF_LOOP_NONE);
	ytVideo[0].loadMovie(getNewVideo());
    ytVideo[1].setLoopState(OF_LOOP_NONE);
	ytVideo[1].loadMovie(getNewVideo());

	ytVideo[currentIndex].play();
}

//------------------------------------------------------------------------------
void ofApp::update()
{

    if((ytVideo[0].isLoaded()) && (!bLoaded[0])) {
        bLoaded[0] = true;
        ytVideo[0].update();
        cout << "loaded..." << 0 << endl;
    }

    if((ytVideo[1].isLoaded()) && (!bLoaded[1])) {
        bLoaded[1] = true;
        ytVideo[1].update();
        cout << "loaded..." << 1 << endl;
    }

    if(bLoaded[currentIndex]) {
        ytVideo[currentIndex].update();
        if((ytVideo[currentIndex].getWidth() > 0) && (ratio <= 0)) {
            ratio = (ytVideo[currentIndex].getHeight() / ytVideo[currentIndex].getWidth());
            cout << "ratio =" << ratio << endl;
            cout << "video width = " << ytVideo[currentIndex].getWidth() << "  video height=" << ytVideo[currentIndex].getHeight() << endl;
        }
        //else ratio =0;
        //cout << "ratio=" << ratio << endl;
    }
}

//------------------------------------------------------------------------------
void ofApp::draw()
{
    if(ratio != 0) {
        if(ofGetWidth()*ratio < ofGetHeight()) {
            ytVideo[currentIndex].draw(0,(ofGetHeight() - ofGetWidth()*ratio) /2,ofGetWidth(),ofGetWidth()*ratio);
        }
        else {
            ytVideo[currentIndex].draw(0,0,ofGetWidth(),ofGetWidth()*ratio);
        }
    } else {
        cout << "RATIO is zero!" << endl;
    }
}

//------------------------------------------------------------------------------
const string ofApp::getNewVideo()
{
    //string new_url = "https://www.youtube.com/watch?v=3iTV-rC8x-E";
    //string new_url = "https://www.youtube.com/watch?v=hHVzTZp7GZ8";
    //string new_url = "https://www.youtube.com/watch?v=qoZp-JOXi74";
    //string new_url = "https://www.youtube.com/watch?v=si4t-19A0HM&feature=youtube_gdata";

    string new_url = getRandomURL();
    string video_url = "youtube-dl -g \"" + new_url + "\"";

	FILE *in;
	char buff[2048];

	if(!(in = popen(video_url.c_str(), "r"))){
		cout << "failed to popen" << endl;
	}

	while(fgets(buff, sizeof(buff), in)!=NULL){
		;//cout << buff;
	}
	pclose(in);

    bLoaded[currentIndex] = false;
    video_url = buff;
    video_url.erase( std::remove_if(video_url.begin(), video_url.end(), ::isspace ), video_url.end() );
    //video_url = "\"" + video_url + "\"";
    cout << "getting video: " << video_url << endl;
	return video_url;
}

//------------------------------------------------------------------------------
const string ofApp::getRandomURL()
{
	const string search_url = "https://gdata.youtube.com/feeds/api/videos?q=\"v="+genRandomString(4)+"\"&alt=json";
	cout << "URL=" << search_url << endl;

	if (!response.open(search_url)) {
		cout  << "Failed to parse JSON\n" << endl;
	}

    //cout << response.getRawString(true) << endl;

    string encoding = response["encoding"].asString();
    cout << "encoding = " << encoding << endl;

	int numVideos = response["feed"]["entry"].size();
	cout << "numVideos = " << numVideos << endl;

    string url;

    int i = ofRandom(0,numVideos);


    Json::Value entryNode = response["feed"]["entry"][i];

    Json::StyledWriter writer;
   //cout << "JSON output:" << endl;
    //cout << writer.write(entryNode) << endl;

    //float width, height;
    //width = entryNode["media$group"]["media$thumbnail"][0]["width"].asInt();
    //height = entryNode["media$group"]["media$thumbnail"][0]["height"].asInt();
    string seconds = entryNode["media$group"]["yt$duration"]["seconds"].asString();
    cout << "seconds = " << seconds << endl;
    //cout << "width =" << width << " height =" << height << endl;
    cout << "title = " << entryNode["title"]["$t"].asString() << endl;
    cout << "author = " << entryNode["author"][0]["name"]["$t"].asString() << endl;
    cout << "link = " << entryNode["link"][0]["href"].asString() << endl;

    url = entryNode["link"][0]["href"].asString();

    return url;
}

//------------------------------------------------------------------------------
const string ofApp::genRandomString(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    string s;

    for (int i = 0; i < len; ++i) {
        char c = alphanum[rand() % (sizeof(alphanum) - 1)];
        s.push_back(c);
    }

    return s;
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key)
{
    switch(key){
        case ' ':
            ratio = 0;
            bLoaded[currentIndex] = false;
            ytVideo[currentIndex].closeMovie();
            ytVideo[currentIndex].loadMovie(getNewVideo());
            ytVideo[currentIndex].setPaused(bPaused);

            if(currentIndex == 0) currentIndex = 1;
            else currentIndex = 0;

            ytVideo[currentIndex].play();
        break;
        case't':
            cout << "Total Frames=" << ytVideo[currentIndex].getTotalNumFrames() << endl;
        break;
        case'p':
            bPaused = !bPaused;
            ytVideo[currentIndex].setPaused(bPaused);
        break;
    }
}


