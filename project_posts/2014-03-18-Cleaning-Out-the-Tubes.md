

Over the weekend I checked in my first batch of code. It's an *openFrameworks* application that loads up a random Youtube Video and plays it without any overlays or advertising material. Pressing the space bar will load up a new random video. Currently I'm using the standard openFrameworks GStreamer based video player for Linux. I'm developing on Ubuntu, my preferred operating system. Linux is especially good at harnessing the power of the open source world, which is a space that I believe is particularly useful to artists.

[GStreamer](gstreamer.freedesktop.org), at least the version found in *openFrameworks* has a couple of issues with certain YouTube clips, namely ones that seem to change frame size over time (possibly interlaced?). There may also be restricted videos in there as well that are not playing, but it shouldn't be too hard to filter all these duds out eventually. Right now I just skip a video if it hasn't loaded properly. About 90% of them do however, so I've been watching a LOT of YouTube content. It's pretty interesting and fascinating. Some clips are obviously tiresome and quickly get dismissed, but there's a whole load of unexpected content up there, or at least content so bizarre yet so mundane that you can't help but be transfixed. Like the kid who narrates in a breathless voice and with a very very shaky camera how he feeds his guineapigs each morning. Or the monologue by a slightly depressed looking Russian girl whose story you can only grasp by assigning a narrative to her yourself. Or the Brazilian youths performing crazy, camp, screeching dances in a kind of madcap drag. Then there are the endless minecraft playthroughs, usually voiced-over in such dead-pan voices and referencing so many odd game terms that you might think yourself on another planet.

In any case, the hardest part was figuring out how to best thread the videos so that loading them didn't introduce to much of a delay. My initial tests without any threading resulted in long delays of 5-10 seconds from the time that a query was made to the beginning of the video playback. Adding threading made it more or less instantaneous, as I would load new video data on a separate thread to be cued in when needed.

The code for the threaded loading looks something like this:
```
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

    		urls_to_load.pop_front();
        }
	}
}
```
This code was based on the ThreadedImageLoader example that comes with openFrameworks, so this wasn't too hard to get going.
