#include "opencv2\core\core.hpp"


#include <iostream>
#include <windows.h>

#include "test_lists.h"

using namespace std;

void TestFunction( bool (*func)(), const string func_name ) {
  
  assert ( func != NULL ) ;
  // get frenquency of hi-res counter
  LARGE_INTEGER nFreq;
  QueryPerformanceFrequency(&nFreq); 
  LARGE_INTEGER start_point, end_point;
  QueryPerformanceCounter(&start_point);

  HANDLE hConsole;
  hConsole = GetStdHandle (STD_OUTPUT_HANDLE);

  cout<<"\nTesting:"<<func_name<<"..."<<endl;
  if ( func() ) {
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout<<"Passed"<<endl;
  } else {
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    cout<<"Failed"<<endl;
  }
  SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
  QueryPerformanceCounter(&end_point);
  double elapsed_time = (double)(end_point.QuadPart - start_point.QuadPart)/
      (double)(nFreq.QuadPart) * 1000;
  cout<<"Elipsed time:"<< elapsed_time << " ms"<<endl;
}

int main(){
  TestFunction(OCHL_test::color_channel_extractor_test::
      ShouldThrowExceptionWhenNotInvokeExtractNewImage, 
      "ShouldThrowExceptionWhenNotInvokeExtractNewImage");
  TestFunction(OCHL_test::color_channel_extractor_test::
    ShouldContainsLabChannels,
    "ShouldContainsLabChannels");
  TestFunction(OCHL_test::color_channel_extractor_test::
    ShouldContainsLabChannels,
    "ShouldContainsHSVChannels");
}