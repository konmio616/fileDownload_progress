# fileDownload_progress
Download file using libcurl, and get current progress value using callback.

# How to use
#include "FileDownloader.h"

float progressValue = 0.f;


static int ProgressCallback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{

	if (dltotal != 0)//dltotal == 0 -> size not checked
	{
 
		progressValue = static_cast<float>(dlnow / dltotal);
	}
 
	return 0;
}


void main()
{
    FileDownloader downloader(ProgressCallback);
    
    downloader.DownloadFile("file link", "file path");
}


progressValue will be 0 ~ 1
