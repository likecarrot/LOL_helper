#pragma	once
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <map>
#include    <functional>
#include    <mutex>

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
    bool request(RequestMethod method, std::string path,FILE* fp);

    static LCU_REQUEST& getInstance() {
        static LCU_REQUEST instance;
        return instance;
    }
    static void setInstance(std::string auth, std::string port) {
        LCU_REQUEST& instance = getInstance();
        instance.reset_auth(auth, port);
    }
    void reset_auth(std::string auth, std::string port);
    std::string get_auth() { return auth_; };
    std::string get_port() { return port_; };
private:
    std::mutex mtx;//单线程

    LCU_REQUEST() {}
    ~LCU_REQUEST() {
    }

    // 禁止拷贝构造函数和赋值运算符
    LCU_REQUEST(const LCU_REQUEST&) = delete;
    LCU_REQUEST& operator=(const LCU_REQUEST&) = delete;
    CURL* create_curl(RequestMethod method, std::string path);//需要自己清理 并且path中没有包含localhost 自己填写
    std::string auth_;
    std::string port_;
    std::string base_url;
    std::string basic_auth;
    static size_t write_file_callback(void* ptr, size_t size, size_t nmemb, FILE* stream);
    static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);

};


