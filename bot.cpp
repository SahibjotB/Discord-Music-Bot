#include <dpp/dpp.h>
#include <nlohmann/json.hpp>
#include <fmt/format.h>
#include <iomanip>
#include <sstream>
#include <Python.h> 
#include <vector>
#include <fstream>
#include <iostream>
#include <mpg123.h>
#include <out123.h>
#include <queue>

/* For an example we will hardcode a path to some awesome music here */
#define MUSIC_FILE "song.mp3"


std::queue<std::string> songs;
std::queue<std::string> tempQ;


int qsize = 0;

dpp::voiceconn* v;
PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *presult;

bool inVoice = false;
/*
 *@brief This function intializes the vairables for the downloading song function
 *it doesn't take any inputs  and doesn't return anything as it just intializes
 *@authors Robert Liao , Dean Palimaka ,  Alexander Turgeon, Michael Ma and, Sahibjot Boyal 
 *
 */ 

void downloaderInit(){
		
		printf("just before downloading anything");
		fflush(stdout);		

		   setenv("PYTHONPATH",".",1);
		   
		   
		   
		   Py_Initialize();
		   

                   // Build the name object
                   pName = PyUnicode_FromString((char*)"youtubeDownloader");
		   
                   // Load the module object
                   pModule = PyImport_Import(pName);
		   
		   
                   // pDict is a borrowed reference 
                   pDict = PyModule_GetDict(pModule);
		  

                   // pFunc is also a borrowed reference 
                   pFunc = PyDict_GetItemString(pDict, (char*)"download");
}

/*
 *@brief This downloads a song by taking in a url and calling a python program to download the song.
 *@param const char url[] the link that you want to download
 *@return returns s the name of file it downloaded
 *@author  Robert Liao, Dean Palimaka, Akexander Turgeon, Michael Ma, Shibjot Boyal
 *
 */

std::string downloadSong(const char url[]){
		
	   
                   if (PyCallable_Check(pFunc)){
			pValue=Py_BuildValue("(z)",(char*)url);
                   	PyErr_Print();
                   	presult=PyObject_CallObject(pFunc,pValue);
                   	PyErr_Print();
                   } 
		   else{
                        PyErr_Print();
                   }
		    
                   Py_DECREF(pValue);
		   
		   Py_DECREF(pModule);
		   Py_DECREF(pName);
		   // Finish the Python Interpreter
		   
		   std::string s(PyUnicode_AsUTF8(presult));
		   return s;
			
}

/*
 *@brief decondes the song into an mp3
 *@parm int index is the song number that it wants to load
 *@return  returns pcmdatata is the buffer which is set to the voice channel to play
 *@author Robert Liao, Dean Palimaka, Alexander Turgeon, Michael Ma, Shibjot Boyal
 */

std::vector<uint8_t> loadSong(int index)
{
		
	std::vector<uint8_t> pcmdata;
	int err = 0;
    	unsigned char* buffer;
    	size_t buffer_size, done;
    	int channels, encoding;
    	long rate;

	std::string songToPlay;
	std::string lastSong;
	
	bool songFound = false;
	
	int qsize = songs.size();
	
	for (int i=0;i<qsize;i++){

		lastSong = songs.front();
		songs.pop();
	
		if(!(i == index)){
			songs.push(lastSong);
		}
		else{
			songToPlay = lastSong;
			songFound = true;
		}
	}

    	/* Note it is important to force the frequency to 48000 for Discord compatibility */
    	mpg123_handle *mh = mpg123_new(NULL, &err);
    	mpg123_param(mh, MPG123_FORCE_RATE, 48000, 48000.0);
 
    	/* Decode entire file into a vector. You could do this on the fly, but if you do that
    	* you may get timing issues if your CPU is busy at the time and you are streaming to
    	* a lot of channels/guilds.
    	*/
    	buffer_size = mpg123_outblock(mh);
    	buffer = new unsigned char[buffer_size];
 
    	/* Note: In a real world bot, this should have some error logging */
    	mpg123_open(mh, songToPlay.c_str());
    	mpg123_getformat(mh, &rate, &channels, &encoding);
 
    	unsigned int counter = 0;
   	for (int totalBytes = 0; mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK; ) {
        	for (auto i = 0; i < buffer_size; i++) {
            		pcmdata.push_back(buffer[i]);
        	}
        	counter += buffer_size;
        	totalBytes += done;
    	}
    	delete buffer;
    	mpg123_close(mh);
    	mpg123_delete(mh);
	
	return pcmdata;

}

/*
 * @brief adds song link into queue
 * @param position determines if you add to front or back. 0 is add to front and 1 is add to back
 *take in a string  link if the link was a playlist in will break it up based on the sperator ||| and add it into the queue 
 *@author Robert Liao, Dean Palimaka, Alexander Turgeon, Michael Ma, Shibjot Boyal
 *
 */


void queueAdder(std::string link, int position){

	std::string lastSong;

	qsize = songs.size();

	int location = 0;

	//if song link was a playlist breaks it up based on ||| and adds it into the queue

	if(link.find("|||") != std::string::npos){
		
		while((location = link.find("|||")) != std::string::npos){
			std::string current = link.substr(0, location);
			//if position is 0 it adds to the front of the queue
			
			if(position == 0){
				for(int i = 0; i < qsize; i++){
	  				lastSong = songs.front();
					songs.pop();
					tempQ.push(lastSong);
				}
		
				songs.push(current);
	    
 				for(int i=0;i<qsize; i++){
					songs.push(tempQ.front());
					tempQ.pop();
				}
			}
			//if position is 1 it adds to the back of the queue
			else{
				songs.push(current);
			}
		
			link.erase(0, location+3);
		}
	}
	//if it wasn't a playlist it just adds to the queue
	else{

//if position is 0 it adss to the front of the queue
                if(position == 0){
                        for(int i = 0; i < qsize; i++){
                                lastSong = songs.front();
                                songs.pop();
                                tempQ.push(lastSong);
                        }

                        songs.push(link);

                        for(int i=0;i<qsize; i++){
                                songs.push(tempQ.front());
                                tempQ.pop();
                        }
                }

//if position is 1 it adds to the back of the queue
                else{
                        songs.push(link);
                }
	}

}



/* 
 * Starter code was derived from https://dpp.dsv/joinvc.html
*/

/*
 *@brief main fuction sets up the bot and gets it ready to read commands from users
 *@param takes in a command form the user such as .add Happy Birthday splits the command and sonng name
 *
 */

int main(int argc, char const *argv[]) {

    	/* This will hold the decoded MP3.
    	* The D++ library expects PCM format, which are raw sound
    	* data, 2 channel stereo, 16 bit signed 48000Hz.
    	*/

	std::vector<uint8_t> pcmdata;
 
    	mpg123_init();
	
       downloaderInit();	
 
    	/* Setup the bot */
    	dpp::cluster bot("MTA4MjY5OTQ2MjM2MDcwMzEzOQ.Gz_XDf.uoy-uyas-3UWDxzc2l8m6F1GEsBsFZ2tLs1-VM", dpp::i_default_intents | dpp::i_message_content);
 
        bot.on_log(dpp::utility::cout_logger());
 
	
    	/* Use the on_message_create event to look for commands */
    	bot.on_message_create([&bot, &pcmdata](const dpp::message_create_t & event) {

        std::string ss(event.msg.content);
       	std::string command = ss;
	std::istringstream iss(command);
	std::string token;

	std::cout << command << std::endl;

	std::getline(iss, token, ' ');
	
        std::string display = "";
 	std::string lastSong;
	int delNum;


        /* Tell the bot to join the discord voice channel the user is on. Syntax: .join */
        if (token == ".join") {
            	dpp::guild * g = dpp::find_guild(event.msg.guild_id);
            	if (!g->connect_member_voice(event.msg.author.id)) {
                	bot.message_create(dpp::message(event.msg.channel_id, "You don't seem to be on a voice channel! :("));
		}
            	v = event.from->get_voice(event.msg.guild_id);
		inVoice = true;
        }
 
	
	// Disconnects discord bot from server
	if (token == ".disconnect") {
	    	event.from->disconnect_voice(event.msg.guild_id);
		inVoice = false;
	}

	//this skips the next song
	if (token == ".next"){
		
		//if its not in a voice channel it out puts its not in  one
		if(!inVoice)
		{
			dpp::message msg(event.msg.channel_id, "Please join the bot to a voice channel using .join");
			bot.message_create(msg);	
		}
		else{	
		//stops current audio that is playing and plays next song	
		if(v->voiceclient->is_playing()){
			v->voiceclient->stop_audio();
		}
		if (songs.size() > 0) 
		{

		pcmdata = loadSong(0);
		

		
		
	
            	if (v && v->voiceclient && v->voiceclient->is_ready()) 
		{
                	/* Stream the already decoded MP3 file. This passes the PCM data to the library to be encoded to OPUS */
                	v->voiceclient->send_audio_raw((uint16_t*)pcmdata.data(), pcmdata.size());
		}

		}

		
		if (songs.size() == 0) {
        		dpp::message msg(event.msg.channel_id, "Queue is empty...");
        		bot.message_create(msg);
       		}
        	else {
			dpp::message msg(event.msg.channel_id, "Deleted a song");
			bot.message_create(msg);
        	}
		}
	}

	// Adds song to front of queue	
		if (token == ".addFront")
	{
		std::getline(iss, token, ' ');
        	token = downloadSong(token.c_str());

		queueAdder(token, 0);
	}
	
	// Adds song to end of queue
	if (token == ".add"){
		std::cout << "Add command: " << command << std::endl;
        	std::getline(iss, token, '\n');

		token = downloadSong(token.c_str());
		
		queueAdder(token, 1);

	}

	// Displays current queue
	if (token == ".view"){
		qsize = songs.size();

		if(qsize == 0){
			dpp::message msg(event.msg.channel_id, "There is nothing in the queue");
		}
		else{
			//loops through all the songs in the queue and displays them
	   
			for(int i = 0; i < qsize; i++){
				display = display + songs.front() + "\n";
	  			lastSong = songs.front();
				songs.pop();
				songs.push(lastSong);
		
	   		}
	   		dpp::message msg(event.msg.channel_id, display);
	   		bot.message_create(msg);
	   		display = "";
		}
	}

	// Deletes specified song by its ID
	if (token == ".delete"){
		qsize = songs.size();
		std::getline(iss, token, ' ');
		delNum = stoi(token);

		for (int i=0;i<qsize;i++){
			
	  		lastSong = songs.front();
			songs.pop();

			if(!(i+1 == delNum)){
				songs.push(lastSong);
			}

		}
	}

    });
 
    /* Start bot */
    bot.start(dpp::st_wait);
 
    /* Clean up */
    mpg123_exit();
 
    return 0;
}

