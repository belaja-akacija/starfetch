#include <random>
#include "functions.h"
static void setColor(string color)
{
  if (color == "black")
    REQUESTED_COLOR = "\033[1;30m";
  else if (color == "white")
    REQUESTED_COLOR = "\033[1;37m";
  else if (color == "cyan")
    REQUESTED_COLOR = "\033[1;36m";
  else if (color == "magenta")
    REQUESTED_COLOR = "\033[1;35m";
  else if (color == "yellow")
    REQUESTED_COLOR = "\033[1;33m";
  else if (color == "red")
    REQUESTED_COLOR = "\033[1;31m";
  else if (color == "blue")
    REQUESTED_COLOR = "\033[1;34m";
}

static inline void PrintConst(string &pathc)
{
  ifstream c(pathc);  //opens the file containing constellation info
  ifstream f(path+"template");    //opens the output template file
  stringstream strStream;
  string s, l;
  json j;

  if(f.is_open())
  {
    strStream << f.rdbuf(); //read the template
    s = strStream.str();    //converts it to a string
    replace(s.begin(), s.end(), '^', '\033');   //replace '^' with the '\e' to print bold/colored text
    f.close();  //closes the template
  }

  if(c.is_open())
  {
    c >> j;     //parse the selected JSON file
                //fills the template with dats
    s.replace(s.find("%0"), string("%0").size(), j["title"].get<string>());
    s.replace(s.find("%11"), string("%11").size(), j["name"].get<string>());
    s.replace(s.find("%12"), string("%12").size(), j["quadrant"].get<string>());
    s.replace(s.find("%13"), string("%13").size(), j["right ascension"].get<string>());
    s.replace(s.find("%14"), string("%14").size(), j["declination"].get<string>());
    s.replace(s.find("%15"), string("%15").size(), j["area"].get<string>());
    s.replace(s.find("%16"), string("%16").size(), j["main stars"].get<string>());

    //renders the constellation's graph from the coordinates specified in the JSON file
    for(int i=1;i<=10;i++)  //for each of the lines (10)
    {
      l="";
      for(int k=1;k<=22;k++)  //for each of the columns of the graph (22)
                              //if the JSON file specifies a star at position k
        if(j["graph"]["line"+to_string(i)].find(to_string(k)) != j["graph"]["line"+to_string(i)].end())
          l+=REQUESTED_COLOR + j["graph"]["line"+to_string(i)][to_string(k)].get<string>() + "\033[0;0m"; //put the star (which is stored into the JSON fine, might change this in the future)
        else
          l+=" "; //put a space

      //insert the line into the template
      s.replace(s.find("%"+to_string(i)), string("%"+to_string(i)).size(), l);
      s = std::regex_replace(s, std::regex("requestedColor"), REQUESTED_COLOR);
    }

    c.close();
    cout << s << endl;  //prints the output
  }else
    Error("", 2);
}


static string RandomConstRefactor()
{

// I'm not a C++ programmer at all, so this is probably super messy, but it works
// refer to the lisp version to get an idea of what to do for the logic
// then reimplement it in C++

  int size_of_directories = sizeof(directories)/sizeof(string);
  std::random_device rd;
  std::mt19937 e(rd());
  std::uniform_int_distribution<int> randdir(0, (size_of_directories - 1));
  std::uniform_real_distribution<double> udev(0.0, (size_of_directories - 1.0));
  auto random_dir = randdir(e);

  size_t pos;
  size_t n = 1;
  string s;

  for (const auto & entry : filesystem::directory_iterator(path + directories[random_dir] + SEP))
  {
      if (udev(e) < 1.0 / n) {
        pos = entry.path().u8string().find(directories[random_dir] + SEP);
        s = entry.path().u8string().substr(pos);
      }
      n++;
  }


  return s;
}


static string RandomConst()
{
  //srand(static_cast<unsigned int>(time(NULL)) ^ static_cast<unsigned int>(getpid()));
  std::random_device rd;
  std::mt19937 e{rd()};
  std::uniform_int_distribution<int> udist(0, 11);
  size_t pos;
  string s;

  //SHOULD BE IMPROVED IN THE FUTURE
  //gets every constellation name in the "constellation/" directory, and exits when two randomly generated numbers are equal, resulting in picking a random file
  for (const auto & entry : filesystem::directory_iterator(path+"constellations" + SEP))
  {
    pos = entry.path().u8string().find("constellations" + SEP);
    s = entry.path().u8string().substr(pos);
    if(s != "constellations/.DS_Store" && udist(e) == udist(e))
      break;
  }


  return s;
}

static void PrintList()
{
  string s;

  //cout << REQUESTED_COLOR + "✦  available constellations\033[0;0m:" << endl;
  //prints every constellation name from the files name in the directories array
  for (long unsigned int i = 0; i < sizeof(directories)/sizeof(string); i++){
    cout << "\n" + REQUESTED_COLOR + "✦ available " + directories[i] + "\033[0;0m:" << endl;
    for (const auto & entry : filesystem::directory_iterator(path + directories[i] + SEP))
    {
      s = entry.path().u8string().substr(entry.path().u8string().find("constellations" + SEP)+15); //from "/usr/local/opt/starfetch/res/constellations/xxxxxx" to "xxxxxx"
      s = s.substr(0, s.length()-5);
      if(s != ".DS_")    cout << REQUESTED_COLOR + s + "\033[0;0m" << endl;
    }
  }
}

static void Error(const char *err, int code)
{
  switch(code)    //each error has a specific code
  {
    case 0: //0 for the missing input
      cout << "Error: you must input a constellation name after -n." << endl << endl;
      break;
    case 1: //1 for the invalid argument
      cout << "Error: '" << err << "' isn't a valid argument." << endl << endl;
      break;
    case 2: //2 for the invalid constellation name
      cout << "Error: the constellation you asked for isn't recognized." << endl << endl;
      break;
  }

  Help(); //after any error occours, the help message is shown
}

static void Help()
{
  ifstream f(path + "help_message.txt");
  cout << f.rdbuf();
  f.close();
  exit(EXIT_SUCCESS);
}
