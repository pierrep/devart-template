#include "ofApp.h"

//------------------------------------------------------------------------------
void ofApp::setup()
{
	ofSetVerticalSync(true);
	ofBackground(0);
    ofSeedRandom(ofGetSystemTimeMicros());

    ratio = 0;

    ytVideo.setLoopState(OF_LOOP_NONE);
	ytVideo.loadMovie(getNewVideo());
	ytVideo.play();
}

//------------------------------------------------------------------------------
void ofApp::update()
{

    if((ytVideo.isLoaded()) && (!bLoaded)) {
        bLoaded = true;
        cout << "loaded..." << endl;
    }

    if(bLoaded) {
        ytVideo.update();
        if((ytVideo.getWidth() > 0) && (ratio <= 0)) {
            ratio = (ytVideo.getHeight() / ytVideo.getWidth());
            cout << "ratio =" << ratio << endl;
            cout << "video width = " << ytVideo.getWidth() << "  video height=" << ytVideo.getHeight() << endl;
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
            ytVideo.draw(0,(ofGetHeight() - ofGetWidth()*ratio) /2,ofGetWidth(),ofGetWidth()*ratio);
        }
        else {
            ytVideo.draw(0,0,ofGetWidth(),ofGetWidth()*ratio);
        }
    }
}

//------------------------------------------------------------------------------
const string ofApp::getNewVideo()
{
    //string new_url = "https://www.youtube.com/watch?v=3iTV-rC8x-E";
    //string new_url = "https://www.youtube.com/watch?v=hHVzTZp7GZ8";
    //string new_url = "https://www.youtube.com/watch?v=qoZp-JOXi74";

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

    bLoaded = false;
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
    cout << "JSON output:" << endl;
    cout << writer.write(entryNode) << endl;

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
            bLoaded = false;
            ytVideo.closeMovie();
            ytVideo.loadMovie(getNewVideo());
            ytVideo.play();
        break;
        case't':
            cout << "Total Frames=" << ytVideo.getTotalNumFrames() << endl;
        break;
        case'p':
            bPaused = !bPaused;
            ytVideo.setPaused(bPaused);
        break;
    }
}


