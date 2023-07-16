#include	"request_utils.h"

void    LCU_REQUEST::reset_auth(std::string auth, std::string port) {
	auth_ = auth;
	port_ = port;
	// 其他重置逻辑
	std::string protocol = "https";
	base_url = protocol + "://127.0.0.1:" + port_;
	//session = curl_easy_init();
	std::string basic_auth = "riot:" + auth_;
	curl_easy_setopt(session, CURLOPT_USERPWD, basic_auth.c_str());
	curl_easy_setopt(session, CURLOPT_SSL_VERIFYPEER, 0L); // Disable SSL verification
	curl_easy_setopt(session, CURLOPT_SSL_VERIFYHOST, 0L); // Disable SSL verification
	curl_easy_setopt(session, CURLOPT_FOLLOWLOCATION, 1L); // Allow redirects
	//curl_easy_setopt(session, CURLOPT_COOKIEFILE, ""); // Enable cookie handling
	//curl_easy_setopt(session, CURLOPT_COOKIEJAR, ""); // Enable cookie handling
	curl_easy_setopt(session, CURLOPT_AUTOREFERER, 1L); // Set referer
	curl_easy_setopt(session, CURLOPT_ENCODING, ""); // Enable compression
	//curl_easy_setopt(session, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
}

std::string LCU_REQUEST::request(RequestMethod method, std::string path) {
	std::string url = base_url + path;
	CURLcode res;
	std::string response;
	std::lock_guard<std::mutex> lock(mtx);
	auto it = method_options.find(method);
	if (it != method_options.end()) {
		curl_easy_setopt(session, (it->second), 1L);
		if (method == RequestMethod::PUT_METHOD) {
			curl_easy_setopt(session, CURLOPT_CUSTOMREQUEST, "PUT");
		}
		else if (method == RequestMethod::DELETE_METHOD) {
			curl_easy_setopt(session, CURLOPT_CUSTOMREQUEST, "DELETE");
		}
		else if (method == RequestMethod::POST_METHOD) {
			curl_easy_setopt(session, CURLOPT_POST, 1L);
		}
		else if (method == RequestMethod::PATCH_METHOD) {
			curl_easy_setopt(session, CURLOPT_CUSTOMREQUEST, "PATCH");
		}
	}
	else {
		// handle error
		return "";
	}
	curl_easy_setopt(session, CURLOPT_URL, url.c_str());
	curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(session, CURLOPT_WRITEDATA, &response);
	res = curl_easy_perform(session);
	return response;
}
std::string LCU_REQUEST::request(RequestMethod method, std::string path, std::string child_web) {
	std::string url = base_url + path+child_web;
	CURLcode res;
	std::string response;
	std::lock_guard<std::mutex> lock(mtx);
	auto it = method_options.find(method);
	if (it != method_options.end()) {
		curl_easy_setopt(session, (it->second), 1L);
		if (method == RequestMethod::PUT_METHOD) {
			curl_easy_setopt(session, CURLOPT_CUSTOMREQUEST, "PUT");
		}
		else if (method == RequestMethod::DELETE_METHOD) {
			curl_easy_setopt(session, CURLOPT_CUSTOMREQUEST, "DELETE");
		}
		else if (method == RequestMethod::POST_METHOD) {
			curl_easy_setopt(session, CURLOPT_POST, 1L);
		}
		else if (method == RequestMethod::PATCH_METHOD) {
			curl_easy_setopt(session, CURLOPT_CUSTOMREQUEST, "PATCH");
		}
	}
	else {
		// handle error
		return "";
	}
	curl_easy_setopt(session, CURLOPT_URL, url.c_str());
	curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(session, CURLOPT_WRITEDATA, &response);
	res = curl_easy_perform(session);
	return response;
}
std::string LCU_REQUEST::request(RequestMethod method, std::string path, std::string child_web, std::string params) {
	std::string url = base_url + path + child_web;
	CURLcode res;
	std::string response;
	std::lock_guard<std::mutex> lock(mtx);
	// Set request options
	auto it = method_options.find(method);
	if (it != method_options.end()) {
		if (method == RequestMethod::GET_METHOD) {
			curl_easy_setopt(session, CURLOPT_HTTPGET, 1);
		}
		if (method == RequestMethod::PUT_METHOD) {
			curl_easy_setopt(session, CURLOPT_CUSTOMREQUEST, "PUT");
		}
		else if (method == RequestMethod::DELETE_METHOD) {
			curl_easy_setopt(session, CURLOPT_CUSTOMREQUEST, "DELETE");
		}
		else if (method == RequestMethod::POST_METHOD) {
			curl_easy_setopt(session, CURLOPT_POST, 1L);
		}
		else if (method == RequestMethod::PATCH_METHOD) {
			curl_easy_setopt(session, CURLOPT_CUSTOMREQUEST, "PATCH");
			// 设置请求头
			struct curl_slist* headers = nullptr;
			headers = curl_slist_append(headers, "Content-Type: application/json");
			curl_easy_setopt(session, CURLOPT_HTTPHEADER, headers);
		}
	}
	else {
		// handle error
		return "";
	}
	curl_easy_setopt(session, CURLOPT_URL, url.c_str());
	curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(session, CURLOPT_WRITEDATA, &response);
	// Set request parameters
	curl_easy_setopt(session, CURLOPT_POSTFIELDS, params.c_str());
	// Perform the request
	res = curl_easy_perform(session);
	return response;
}
bool LCU_REQUEST::request(RequestMethod method, std::string path,  FILE* fp){
	std::string url = base_url + path;
	CURLcode res;
	std::lock_guard<std::mutex> lock(mtx);
	auto it = method_options.find(method);
	if (it != method_options.end()) {
		curl_easy_setopt(session, (it->second), 1L);
		if (method == RequestMethod::PUT_METHOD) {
			curl_easy_setopt(session, CURLOPT_CUSTOMREQUEST, "PUT");
		}
		else if (method == RequestMethod::DELETE_METHOD) {
			curl_easy_setopt(session, CURLOPT_CUSTOMREQUEST, "DELETE");
		}
		else if (method == RequestMethod::POST_METHOD) {
			curl_easy_setopt(session, CURLOPT_POST, 1L);
		}
		else if (method == RequestMethod::PATCH_METHOD) {
			curl_easy_setopt(session, CURLOPT_CUSTOMREQUEST, "PATCH");
		}
	}
	else {
		return "";
	}
	curl_easy_setopt(session, CURLOPT_URL, url.c_str());
	curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, write_file_callback);
	curl_easy_setopt(session, CURLOPT_WRITEDATA, fp);

	res = curl_easy_perform(session);
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
// 回调函数，用于写入文件
size_t LCU_REQUEST::write_file_callback(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}


