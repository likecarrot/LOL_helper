﻿#pragma	once
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <map>


#pragma comment(lib, "libcurl.lib")   
#pragma comment(lib, "wldap32.lib")   
#pragma comment(lib, "ws2_32.lib")   
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "Crypt32.lib")



class LCU_REQUEST
{
public:
    enum class RequestMethod {
        GET_METHOD,
        POST_METHOD,
        PUT_METHOD,
        DELETE_METHOD,
        PATCH_METHOD
    };

    std::string request(RequestMethod method, std::string path);
    std::string request(RequestMethod method, std::string path,std::string child_web);
    std::string request(RequestMethod method, std::string path, std::string child_web, std::string params);
    static LCU_REQUEST& getInstance() {
        static LCU_REQUEST instance;
        return instance;
    }
    static void setInstance(std::string auth, std::string port) {
        LCU_REQUEST& instance = getInstance();
        instance.reset_auth(auth, port);
    }
    void reset_auth(std::string auth, std::string port);
private:
    LCU_REQUEST() {}
    ~LCU_REQUEST() {
        curl_easy_cleanup(session);
    }

    // 禁止拷贝构造函数和赋值运算符
    LCU_REQUEST(const LCU_REQUEST&) = delete;
    LCU_REQUEST& operator=(const LCU_REQUEST&) = delete;

    std::string auth_;
    std::string port_;
    // 其他成员变量
    CURL* session= curl_easy_init();
    std::string base_url;
    static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);
    const std::map<LCU_REQUEST::RequestMethod, CURLoption> method_options = {
    {RequestMethod::GET_METHOD, CURLOPT_HTTPGET},
    {RequestMethod::POST_METHOD, CURLOPT_POST},
    {RequestMethod::PUT_METHOD, CURLOPT_CUSTOMREQUEST},
    {RequestMethod::DELETE_METHOD, CURLOPT_CUSTOMREQUEST},
    {RequestMethod::PATCH_METHOD, CURLOPT_CUSTOMREQUEST}
    };

};


