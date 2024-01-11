READ ME 
Download the files from bitbucket onto a Linux operating system.
You may need to dowload dependencies in order for the files to run correctly. 
To dowload dependencies you can follow the following steps:

sudo apt install wget
sudo wget -O dpp.deb https://dl.dpp.dev/
sudo dpkg -i dpp.deb
sudo apt-get install opus-tools
apt --fix-broken install
sudo apt --fix-broken install
sudo dpkg -i dpp.deb
sudo apt-get install libssl-dev
sudo apt-get install nlohmann-json3-dev
sudo add-apt-repository universe
sudo apt install libfmt-dev
sudo apt-get install mpg123
sudo apt install zlib1g
sudo apt-get install libsodium-dev
sudo apt-get install libmpg123-dev
sudo apt install python3-pip
pip install pytube
sudo pip install moviepy


Compiling instructions:
g++ -std=c++17 bot.cpp -lmpg123 -ldpp -I/usr/include/python3.10 -lpython3.10



Make sure to have a Discord server created that has a voice channel. 
Make sure you have Admin permission on the server.
Add the bot using the following link: 

https://discord.com/oauth2/authorize?client_id=1082699462360703139&permissions=8&scope=bot
Make sure to give the bot permissions as well.

Join a voice channel on the Discord server. You must be in a voice channel for .join command to work. 
You can use the following commands with the bot:



.join 
 #gets the bot to join the voice channel

.disconnect
#gets the bot to disconnect from the voice channel

.next
#plays next song in the queue, pops it and immediately stops the current song being played (if there is one)

.add "song name"
# adds a song to the back of the queue
#for example if you want to add "song1" you would do the following:
#.add song1

.addFront "song name"
#adds a song to the front of the queue
#for example if you want to add "song1" you would do the following:
#.addFront song1

.delete num
#deletes a song from the queue (the queue starts at number 1)
# if your queue was the following:
#song1
#song2
#song3
#to delete song2 do the following:
#.delete 2
 
.view
#displays the queue 
