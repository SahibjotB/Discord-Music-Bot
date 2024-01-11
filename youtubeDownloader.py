## @brief Downloads a YouTube Playlist or a song given a link or a search. It will check if the link sent is a playlist, video and search. It will return a concatenated string if it is a playlist and will return a normal string if it is a search or single video.
# @param link: YouTube link (string)
# @return videoString or yt.title+"mp3" (String containing the name(s) of the YouTube video(s) to put into the queue)
# @authors Alex Turgeon, Robert Liao

from pytube import YouTube
from pytube import Playlist
from pytube import Search
from moviepy.editor import *
import os

# @brief Checks to see what type of link is sent (Playlist, YouTube video, Search) and will download the video(s) accordingly
def download(link):
	# If the link is a YouTube playlist
	if "list=" in link:
		# Creates a playlist object using PyTube
		p = Playlist(link)
		videoString = ""

		# For loop to loop through every video, download it and concatenate it into a string
		for video in p.videos:
			ys = video.streams.filter(only_audio=True).first()
			out_file = ys.download()
			print("downloaded: " + out_file)
			base, ext = os.path.splitext(out_file)
			
			# Converts the audio file into an mp3
			fileConvert = AudioFileClip(out_file);
			fileConvert.write_audiofile(video.title + ".mp3")
			fileConvert.close()

			print("Downloading...")
			print("Download completed!!")

			# Concatenates the string with the new song title
			videoString = videoString + video.title + ".mp3|||"
		return videoString
	# If the link is a YouTube video
	elif "watch?" in link:
		yt= YouTube(link)
		# Showing details

		# Getting the highest resolution possible
		ys = yt.streams.filter(only_audio=True).first()

		out_file = ys.download()
		print("downloaded: " + out_file)
		base, ext = os.path.splitext(out_file)

		# Similar to before, converts the mp4 file to an mp3
		fileConvert = AudioFileClip(out_file);
		fileConvert.write_audiofile(yt.title + ".mp3")
		fileConvert.close()

		# Starting download
		print("Downloading...")
		print("Download completed!!")
		return yt.title + ".mp3"
	# If the link is actually a YouTube search
	else:
		# Creates a search object and gets the first video from the search result
		s = Search(link)
		searchResult = "https://youtube.com/watch?v=" + s.results[0].video_id
		
		yt= YouTube(searchResult)
		ys = yt.streams.filter(only_audio=True).first()

		# The rest of the code is similar to the previous if statement
		out_file = ys.download()
		print("downloaded: " + out_file)
		base, ext = os.path.splitext(out_file)

		fileConvert = AudioFileClip(out_file);
		fileConvert.write_audiofile(yt.title + ".mp3")
		fileConvert.close()

		print("Downloading...")
		print("Download completed!!")
		return yt.title + ".mp3"

