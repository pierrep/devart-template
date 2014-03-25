#include "ofxThreadedYouTubeVideo.h"

ofxThreadedYouTubeVideo::ofxThreadedYouTubeVideo()
{
    startThread();
}

ofxThreadedYouTubeVideo::~ofxThreadedYouTubeVideo()
{
    condition.signal();

    // stop threading
    waitForThread(true);
}


// Load a url
//--------------------------------------------------------------
void ofxThreadedYouTubeVideo::loadYouTubeURL(string _url, int _id)
{
	ofYouTubeLoaderEntry entry(_url, _id);

    lock();
	urls_to_load_buffer.push_back(entry);
    condition.signal();
    unlock();
}

//------------------------------------------------------------------------------
const string ofxThreadedYouTubeVideo::genRandomString(const int len) {
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

//------------------------------------------------------------------------------
const string ofxThreadedYouTubeVideo::getRandomURL()
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

// Get a new url - called from within thread
//--------------------------------------------------------------
bool ofxThreadedYouTubeVideo::getNewURL(ofYouTubeLoaderEntry& entry )
{
    string new_url = entry.input_url;

    if(new_url == "") {
        new_url = getRandomURL();
    }
    string video_url = "youtube-dl -g \"" + new_url + "\"";

	FILE *in;
	char buff[2048];

	if(!(in = popen(video_url.c_str(), "r"))){
		cout << "failed to popen" << endl;
		return false;
	}

	while(fgets(buff, sizeof(buff), in)!=NULL){
		;//cout << buff;
	}
	pclose(in);

    entry.bLoaded = false;
    video_url = buff;
    video_url.erase( std::remove_if(video_url.begin(), video_url.end(), ::isspace ), video_url.end() );
    //video_url = "\"" + video_url + "\"";

    entry.url = video_url;
    return true;

}
// Reads from the queue and loads new images.
//--------------------------------------------------------------
void ofxThreadedYouTubeVideo::threadedFunction()
{
    deque<ofYouTubeLoaderEntry> urls_to_load;

	while( isThreadRunning() ) {
		lock();
		if(urls_to_load_buffer.empty()) condition.wait(mutex);
		urls_to_load.insert( urls_to_load.end(),
							urls_to_load_buffer.begin(),
							urls_to_load_buffer.end() );

		urls_to_load_buffer.clear();
		unlock();


        while( !urls_to_load.empty() ) {
            ofYouTubeLoaderEntry  & entry = urls_to_load.front();

            if(!getNewURL(entry)) {
                ofLogError("ofxThreadedYouTubeVideo") << "couldn't load url: \"" << entry.input_url << "\"";
            }
            else {
                cout << "ofxThreadedYouTubeVideo got video url: " << entry.url << endl;

                ofxYouTubeURLEvent e = ofxYouTubeURLEvent(entry.url, entry.id);
                ofNotifyEvent(youTubeURLEvent, e, this);
            }

//                lock();
//                images_to_update.push_back(entry);
//                unlock();

    		urls_to_load.pop_front();
        }
	}
}
