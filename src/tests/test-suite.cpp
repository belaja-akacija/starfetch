#include "functions.cpp"
#include "tests.hpp"

static void test_timer(int opt); // test function to see which functions are the fastest

int main()
{
  //string pathc = path;
    test_timer(1);
    //pathc += RandomConst();
    //PrintConst(pathc);
    test_timer(2);
    //pathc += RandomConstRefactor();
    //PrintConst(pathc);
}

static void test_timer(int opt)
{
  Timer t;
  if (opt == 1){
    for (int i = 100000; --i;){
      RandomConst();
    }
    cout << "Time Elapsed For RandomConst, 100000 cycles: \n" << t.elapsed() << " seconds\n\n";
  } else {
    for (int i = 100000; --i;){
      RandomConstRefactor();
    }
    cout << "Time Elapsed For RandomConstRefactor, 100000 cycles: \n" << t.elapsed() << " seconds\n----------\n\n";
  }
}
