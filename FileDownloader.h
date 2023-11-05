#pragma once
#ifndef FILE_DOWNLOADER_H
#define FILE_DOWNLOADER_H

#include <stdexcept>
#include <curl/curl.h>
#include <functional>

class FileDownloader {
public:
    FileDownloader(std::function<int(void*, curl_off_t, curl_off_t, curl_off_t, curl_off_t)> progressCallback)
        : curl(curl_easy_init()), progressCallback(progressCallback) 
    {
        if (!curl) 
        {
            throw std::runtime_error("libcurl initialization failed");
        }
    }

    ~FileDownloader()
    {
        if (curl) 
        {
            curl_easy_cleanup(curl);
        }
    }

    void DownloadFile(const char* url, const char* outputPath) 
    {
        if (curl) 
        {
            curl_easy_setopt(curl, CURLOPT_URL, url);

            FILE* outputFile = fopen(outputPath, "wb");
            if (!outputFile)
            {
                throw std::runtime_error("Failed to open output file for writing");
                return;
            }
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, outputFile);

            curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, &FileDownloader::ProgressCallbackWrapper);
            curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

            CURLcode res = curl_easy_perform(curl);
            fclose(outputFile);

            if (res != CURLE_OK) 
            {
                throw std::runtime_error(curl_easy_strerror(res));
            }
        }
    }

    static int ProgressCallbackWrapper(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
    {
        FileDownloader* downloader = static_cast<FileDownloader*>(clientp);
        if (downloader) 
        {
            return downloader->progressCallback(downloader, dltotal, dlnow, ultotal, ulnow);
        }
        return 0;
    }

private:
    CURL* curl;
    std::function<int(void*, curl_off_t, curl_off_t, curl_off_t, curl_off_t)> progressCallback;
};

#endif