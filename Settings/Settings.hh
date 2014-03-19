/*

  Header For ROOT Settings Object.
  Will Store information for various builds.
  


 */
#ifndef __SETTINGS_HH
#define __SETTINGS_HH

#include <TFile.h>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Settings : public TNamed{


public:
  Settings();
 ~Settings();
  
  void PushLine(string);
  void Print();
  inline vector <string>* GetInfoVector(){return &theInfo;}

private:
  vector <string> theInfo;

public:
  ClassDef(Settings,1);
};


#endif 
