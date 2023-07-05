#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <ctime>
#include <filesystem>
#include <random>
#include <regex>
#include <list>
//#include <unistd.h> for getpid()
#include "../include/json.hpp"

using namespace std;
using json = nlohmann::json;
static void setColor(string color); //sets given color to the REQUESTED_COLOR variable to colorize the output constellation
static inline void PrintConst(string &pathc);  //formats the template file with the requested data and prints out the constellation info
static string RandomConst();   //select a random constellation from the available ones
static string RandomConstRefactor();   //select a random constellation from the available ones
static void PrintList();   //prints out the list of the available constellations
static void Error(const char *err, int type);   //shows an error message
static void Help();    //prints out the help message

#ifdef _WIN32
static string path = "C:\\starfetch\\";
static string SEP = "\\";
#else
static string path = "/usr/local/share/starfetch/";
static string SEP = "/";
string directories[2] = {"constellations", "norse-constellations"}; // array that holds all the directory paths. Consider using a multidimensional array to hold the directory name and also the "nickname" to be used for <type> when using "starfetch -n <type> <constellation>"
#endif // _WIN32

static string REQUESTED_COLOR = "\033[1;37m"; // white color
