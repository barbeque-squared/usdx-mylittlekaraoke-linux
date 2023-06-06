#ifndef MLKHS_LINUX_H
#define MLKHS_LINUX_H

#include <stdio.h>
#include <string>

struct SWebsiteInfo {
  char Name[30];
  int ID;
};

struct SDownloadScore {
  char Md5[33];
  int Level;
  char Response[200];
};

struct SEncryptPassword {
  char Username[201];
  char Password[201];
  char Response[201];
};

struct SEncryptScore {
  char Username[201];
  char Password[201];
  int ScoreInt;
  int ScoreLineInt;
  int ScoreGoldenInt;
  char Md5[33];
  int Level;
  char Response[2001];
};

struct SSendScore {
  char Username[201];
  char Password[201];
  int ScoreInt;
  int ScoreLineInt;
  int ScoreGoldenInt;
  char Md5[33];
  int Level;
};

struct LoginInfo {
  std::string Username;
  std::string Password;
};

struct SendInfo {
  std::string Username;
  std::string Password;
  int ScoreInt;
  int ScoreLineInt;
  int ScoreGoldenInt;
  std::string MD5Song;
  char Level;
};

// should just set Name in passed-in info object
extern "C" void WebsiteInfo(SWebsiteInfo* info);

// Return value: 0 for error connecting, 1 for OK, 2 for login error
extern "C" char Login(const LoginInfo* info);

// ???
extern "C" void EncryptPassword(SEncryptPassword* ep);

// Return value:
// 0 for error connecting
// 2 for login error
// 3 for OK
// 4 for error with the score (needs response parsing)
// 5 for duplicate score (needs response parsing
// 7 for song error (needs response parsing; probably the MD5 is unknown
extern "C" int SendScore(const SSendScore* ss);

// probably needs to return whatever you're sending to the website?
extern "C" void EncryptScore(SEncryptScore* es);

// Needs to return a string in a very specific format, ie:
// 0768006477Elena_PM
// first 5 numbers = highscore, second 5 is average, the rest is the user that has the highscore
extern "C" void DownloadScore(SDownloadScore* dl);

// ???
extern "C" std::string VerifySong(const std::string MD5Song);

#endif
