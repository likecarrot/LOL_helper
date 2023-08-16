#include	"request_utils.h"

void    LCU_REQUEST::reset_auth(std::string auth, std::string port) {
	auth_ = auth;
	port_ = port;
	// 其他重置逻辑
	std::string protocol = "https";
	base_url = protocol + "://127.0.0.1:" + port_;
	basic_auth = "riot:" + auth_;
}

std::string LCU_REQUEST::request(RequestMethod method, std::string path) {
	std::string url = base_url + path;
	CURLcode res;
	std::string response;
	std::lock_guard<std::mutex> lock(mtx);
	auto session = create_curl(method,url);
	curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(session, CURLOPT_WRITEDATA, &response);
	res = curl_easy_perform(session);
	curl_easy_cleanup(session);
	return response;
}
std::string LCU_REQUEST::request(RequestMethod method, std::string path, std::string child_web) {
	std::string url = path+child_web;
	return	request(method, url);
}
std::string LCU_REQUEST::request(RequestMethod method, std::string path, std::string child_web, std::string params) {
	std::string url = base_url + path + child_web;
	CURLcode res;
	std::string response;
	std::lock_guard<std::mutex> lock(mtx);
	auto session = create_curl(method, url);
	curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(session, CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(session, CURLOPT_POSTFIELDS, params.c_str());
	res = curl_easy_perform(session);
	curl_easy_cleanup(session);
	return response;
}
bool LCU_REQUEST::request(RequestMethod method, std::string path,  FILE* fp){
	std::string url = base_url + path;
	CURLcode res;
	std::lock_guard<std::mutex> lock(mtx);
	auto session = create_curl(method, url);
	curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, write_file_callback);
	curl_easy_setopt(session, CURLOPT_WRITEDATA, fp);

	res = curl_easy_perform(session);
	curl_easy_cleanup(session);
	session = nullptr;
	if (res != CURLE_OK)
	{
		OutputDebugStringA(curl_easy_strerror(res));
		return	false;
	}
	return	true;
}
size_t LCU_REQUEST::write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}
CURL* LCU_REQUEST::create_curl(RequestMethod method,std::string path)
{
	CURL* ret = curl_easy_init();
	// 设置请求头
	struct curl_slist* headers = nullptr;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	switch (method)
	{
	case LCU_REQUEST::RequestMethod::GET_METHOD:
		curl_easy_setopt(ret, CURLOPT_CUSTOMREQUEST, "GET");
		break;
	case LCU_REQUEST::RequestMethod::POST_METHOD:
		curl_easy_setopt(ret, CURLOPT_CUSTOMREQUEST, "POST");
		break;
	case LCU_REQUEST::RequestMethod::PUT_METHOD:
		curl_easy_setopt(ret, CURLOPT_CUSTOMREQUEST, "PUT");
		break;
	case LCU_REQUEST::RequestMethod::DELETE_METHOD:
		curl_easy_setopt(ret, CURLOPT_CUSTOMREQUEST, "DELETE");
		break;
	case LCU_REQUEST::RequestMethod::PATCH_METHOD:
		curl_easy_setopt(ret, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(ret, CURLOPT_CUSTOMREQUEST, "PATCH");
		break;
	default:
		curl_easy_setopt(ret, CURLOPT_CUSTOMREQUEST, "GET");
		break;
	}
	curl_easy_setopt(ret, CURLOPT_URL, path.c_str());
	curl_easy_setopt(ret, CURLOPT_USERPWD, basic_auth.c_str());
	curl_easy_setopt(ret, CURLOPT_SSL_VERIFYPEER, 0L); // Disable SSL verification
	curl_easy_setopt(ret, CURLOPT_SSL_VERIFYHOST, 0L); // Disable SSL verification
	curl_easy_setopt(ret, CURLOPT_FOLLOWLOCATION, 1L); // Allow redirects
	curl_easy_setopt(ret, CURLOPT_AUTOREFERER, 1L); // Set referer
	curl_easy_setopt(ret, CURLOPT_ENCODING, ""); // Enable compression
	return ret;
}
// 回调函数，用于写入文件
size_t LCU_REQUEST::write_file_callback(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}


