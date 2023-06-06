#include "mlkhs-linux.h"

#include <cstring>
#include <curl/curl.h>
#include <codecvt>
#include <locale>

#include <iostream>


using namespace std;

extern "C" void WebsiteInfo(SWebsiteInfo* info) {
	strcpy(info->Name, "My Little Karaoke");
}

// always returns OK
extern "C" char Login(const LoginInfo* info) {
	return 1;
}

// just immediately returns the password
extern "C" void EncryptPassword(SEncryptPassword* ep) {
	strcpy(ep->Response, ep->Password);
}

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
	//~ cout << "WRITECALLBACK: " << contents << endl;
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

string toJson(
	const char* username,
	const char* password,
	const int scoreInt,
	const int scoreLineInt,
	const int scoreGoldenInt,
	const char* md5,
	const int level
) {
	string us(username);
	string pw(password);
	return "{\"username\": \""
		+ us
		+ "\", \"password\": \""
		+ pw
		+ "\", \"score\": "
		+ to_string(scoreInt)
		+ ", \"scoreline\": "
		+ to_string(scoreLineInt)
		+ ", \"scoregolden\": "
		+ to_string(scoreGoldenInt)
		+ ", \"song\": \""
		+ md5+"\", \"level\": "
		+ to_string(level)
		+ "}";
}

string toSaveJson(
	const char* username,
	const int scoreInt,
	const int scoreLineInt,
	const int scoreGoldenInt,
	const char* md5,
	const int level
) {
	string us(username);
	return "{username: \""
		+ us
		+ "\", score: "
		+ to_string(scoreInt)
		+ ", scoreline: "
		+ to_string(scoreLineInt)
		+ ", scoregolden: "
		+ to_string(scoreGoldenInt)
		+ ", song: \""
		+ md5+"\", level: "
		+ to_string(level)
		+ "}";
}

extern "C" int SendScore(const SSendScore* ss) {
	string json = toJson(
		ss->Username,
		ss->Password,
		ss->ScoreInt,
		ss->ScoreLineInt,
		ss->ScoreGoldenInt,
		ss->Md5,
		ss->Level
	);
	cout << "THE JSON= " << json << endl;
	
	string s;
	struct curl_slist *list = NULL;
	CURL *curl = curl_easy_init();
	if (curl) {
		char errbuf[CURL_ERROR_SIZE];
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
		errbuf[0] = 0;
		string url = "https://sco.mylittlekaraoke.com/highscores/score/submit";
		//~ string url = "http://localhost:8080/score/submit";
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		list = curl_slist_append(list, "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
		//~ curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		CURLcode res = curl_easy_perform(curl);
		if(res != CURLE_OK) {
			size_t len = strlen(errbuf);
			fprintf(stderr, "\nlibcurl: (%d) ", res);
			if(len)
				fprintf(stderr, "%s%s", errbuf,
								((errbuf[len - 1] != '\n') ? "\n" : ""));
			else
				fprintf(stderr, "%s\n", curl_easy_strerror(res));
		}
		curl_slist_free_all(list);
		curl_easy_cleanup(curl);
	}
	cout << "MLK response for md5=" << ss->Md5 << ", level=" << to_string(ss->Level) << ": " << s << endl;
	if (s.length()) {
		return (int)s[0];
	}
	return 4;
}

extern "C" void EncryptScore(SEncryptScore* es) {
	string res = toSaveJson(
		es->Username,
		es->ScoreInt,
		es->ScoreLineInt,
		es->ScoreGoldenInt,
		es->Md5,
		es->Level
	);
	cout << "ENCRYPTEDSCORE= " << res << endl;
	strcpy(es->Response, res.c_str());
}

extern "C" void DownloadScore(SDownloadScore* dl) {
	//~ cout << "MD5" << dl->Md5 << endl;
	string s;
	curl_global_init(CURL_GLOBAL_ALL);
	
	struct curl_slist *list = NULL;
	CURL *curl = curl_easy_init();
	if (curl) {
		char errbuf[CURL_ERROR_SIZE];
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
		errbuf[0] = 0;
		string url = "https://sco.mylittlekaraoke.com/highscores/score/retrieve?level="+to_string(dl->Level);
		//~ string url = "http://localhost:8080/score/retrieve?level="+to_string(dl->Level);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		list = curl_slist_append(list, "Content-Type: text/plain");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dl->Md5);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
		//~ curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		CURLcode res = curl_easy_perform(curl);
		if(res != CURLE_OK) {
			size_t len = strlen(errbuf);
			fprintf(stderr, "\nlibcurl: (%d) ", res);
			if(len)
				fprintf(stderr, "%s%s", errbuf,
								((errbuf[len - 1] != '\n') ? "\n" : ""));
			else
				fprintf(stderr, "%s\n", curl_easy_strerror(res));
		}
		curl_slist_free_all(list);
		curl_easy_cleanup(curl);
	}
	//~ cout << "we got a response from MLK server:" << endl;
	//~ cout << s << endl;
	strcpy(dl->Response, s.c_str());
}

extern "C" string VerifySong(const string MD5Song) {
	return "s";
}
