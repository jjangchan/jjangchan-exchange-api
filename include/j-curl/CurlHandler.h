//
// Created by jjangchan on 2022/01/02.
//

#ifndef CURLHANDLER_H
#define CURLHANDLER_H

#include <string>
#include <future>
#include <thread>
#include <iostream>
#include <curl/curl.h>
#include <curl/easy.h>
#include <j-boost-logger/BoostLogger.h>

using namespace std;

inline static size_t callBackFunk(char* ptr, size_t size, size_t nmemb, string* stream)
{
    int realSize = size * nmemb;
    stream->append(ptr, realSize);
    return realSize;
}

inline static std::mutex _mutex_curl;

class CurlHandler {
public:
    inline static string url_get_json (const string &url)
    {
        std::lock_guard<std::mutex> lock(_mutex_curl);
        BOOST_LOG_TRIVIAL(info) << ">>> Target url is " << url;
        assert(url.compare("") > 0);
        if(url.compare("") <= 0) {
            throw invalid_argument("invalid url");
        }
        CURL *curl = NULL;
        CURLcode res;
        struct curl_slist *headers=NULL; // init to NULL is important
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charsets: utf-8");
        string chunk;
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callBackFunk);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (string*)&chunk);
            curl_easy_setopt(curl, CURLOPT_PROXY, "");
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
        curl_slist_free_all(headers);
        curl_global_cleanup();
        if(res == CURLE_OK) {
            return chunk;
        } else {
            throw std::runtime_error("CURL error, CURLE_CODE (" + to_string(res) + ")");
        }
    }

    inline static string url_post_json (const string &url,const string &payload)
    {
        std::lock_guard<std::mutex> lock(_mutex_curl);
        BOOST_LOG_TRIVIAL(info) << ">>> Target url is " << url;
        assert(url.compare("") > 0);
        if(url.compare("") <= 0) {
            throw invalid_argument("invalid url");
        }
        CURL *curl = NULL;
        CURLcode res;
        struct curl_slist *headers=NULL; // init to NULL is important
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charsets: utf-8");
        string chunk;
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(payload.c_str()));
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callBackFunk);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (string*)&chunk);
            curl_easy_setopt(curl, CURLOPT_PROXY, "");
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
        curl_slist_free_all(headers);
        curl_global_cleanup();
        if(res == CURLE_OK) {
            return chunk;
        } else {
            throw std::runtime_error("CURL error, CURLE_CODE (" + to_string(res) + ")");
        }
    }
    inline static future<string> url_get_json_async (const string &url)
    {
        BOOST_LOG_TRIVIAL(info) << ">>> Target url is " << url;
        assert(url.compare("") > 0);
        if(url.compare("") <= 0) {
            throw invalid_argument("invalid url");
        }
        return std::async(std::launch::async, [](const string &url) {
            std::lock_guard<std::mutex> lock(_mutex_curl);
            CURL *curl = NULL;
            CURLcode res;
            struct curl_slist *headers=NULL; // init to NULL is important
            curl_global_init(CURL_GLOBAL_ALL);
            curl = curl_easy_init();
            headers = curl_slist_append(headers, "Accept: application/json");
            headers = curl_slist_append(headers, "Content-Type: application/json");
            headers = curl_slist_append(headers, "charsets: utf-8");
            string chunk;
            if (curl)
            {
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callBackFunk);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, (string*)&chunk);
                curl_easy_setopt(curl, CURLOPT_PROXY, "");
                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);
            }
            curl_slist_free_all(headers);
            curl_global_cleanup();
            if(res == CURLE_OK) {
                return chunk;
            } else {
                throw std::runtime_error("CURL error, CURLE_CODE (" + to_string(res) + ")");
            }
        }, url);
    }

    inline static future<string> url_post_json_async (const string &url,const string &payload)
    {
        BOOST_LOG_TRIVIAL(info) << ">>> Target url is " << url;
        assert(url.compare("") > 0);
        if(url.compare("") <= 0) {
            throw invalid_argument("invalid url");
        }
        return std::async(std::launch::async, [](const string &url, const string &payload) {
            std::lock_guard<std::mutex> lock(_mutex_curl);
            CURL *curl = NULL;
            CURLcode res;
            struct curl_slist *headers=NULL; // init to NULL is important
            curl_global_init(CURL_GLOBAL_ALL);
            curl = curl_easy_init();
            headers = curl_slist_append(headers, "Accept: application/json");
            headers = curl_slist_append(headers, "Content-Type: application/json");
            headers = curl_slist_append(headers, "charsets: utf-8");
            string chunk;
            if (curl)
            {
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1);
                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(payload.c_str()));
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callBackFunk);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, (string*)&chunk);
                curl_easy_setopt(curl, CURLOPT_PROXY, "");
                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);
            }
            curl_slist_free_all(headers);
            curl_global_cleanup();
            if(res == CURLE_OK) {
                return chunk;
            } else {
                throw std::runtime_error("CURL error, CURLE_CODE (" + to_string(res) + ")");
            }
        }, url, payload);
    }
};

#endif //SEGMENT_MEASUREMENT_SEARCH_CURLHANDLER_H
