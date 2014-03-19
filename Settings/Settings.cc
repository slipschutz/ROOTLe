


#include "Settings.hh"


Settings::Settings(){
  theInfo.clear();

  this->fName="theSettings";
  this->fTitle="The Settings from the Builder";
  
}



Settings::~Settings(){
  theInfo.clear();

}


void Settings::PushLine(string s){
  theInfo.push_back(s);
}

void Settings::Print(){
  cout<<"******Settings Information******"<<endl;
  for (int i=0;i<theInfo.size();i++){
    cout<<theInfo[i];
  }

}



