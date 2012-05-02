#include "opencv2\core\core.hpp"


#include <iostream>
#include <windows.h>

#include "test_lists.h"

using namespace std;

void TestFunction( bool (*func)(), const string func_name ) {
  
  assert ( func != NULL ) ;

  HANDLE hConsole;
  hConsole = GetStdHandle (STD_OUTPUT_HANDLE);

  cout<<"Testing:"<<func_name<<"..."<<endl;
  if ( func() ) {
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout<<"Passed"<<endl;
  } else {
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    cout<<"Failed"<<endl;
  }
}

int main(){
  TestFunction(OCHL_test::color_channel_extractor_test::
      ShouldThrowExceptionWhenNotInvokeExtractNewImage, 
      "ShouldThrowExceptionWhenNotInvokeExtractNewImage");
  
}