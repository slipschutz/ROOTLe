




#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "Correctable.hh"
#include "TMath.h"

using namespace std;


Correctable::Correctable(){
  //Constructor
  correctionCount=0;
  //  Corrections={0};

}
Correctable::~Correctable(){


}

void Correctable::AddMapEntry(string s,void* p){
  //Adds a single entry to th map of Variables 
  //
  if ( theVariableMap.find(s) == theVariableMap.end() ){ //not in there already
    theVariableMap[s] = p;
  } else {
    cout<<"***Warning entry "<<s<<" already in the Variable Map***"<<endl;
  }
  
}

void Correctable::AddMapEntry(string s,vector <Double_t>* p){
  //Adds a single entry to th map of vectors
  //
  string parsedName;
  int index; 
  ParseVectorInput(s,parsedName,index);
  void* temp =(void*)p;

  AddMapEntry(parsedName,temp);//Add it to the variable map
  if ( theVectorVariableMap.find(parsedName) == theVectorVariableMap.end()){ //not in there already
    theVectorVariableMap[parsedName] = p;
  } else {
    cout<<"***Warning entry "<<s<<" already in the Vector Variable Map***"<<endl;
  }
  
}

void* Correctable::Get(string s){
  //the get methods for simple types.  Needs to be cast to 
  //the appropriate type when called
  //
  if (theVariableMap.find(s) != theVariableMap.end() ){
    
    return theVariableMap[s];

  } else {
    cout<<"***Bad input in variable map "<<s<<" ***"<<endl;
    return NULL;
  }
  
}
vector<Double_t>* Correctable::GetVector(string s){
  //The get methods for Vectors.  Vectors must be of type Double_t
  //
  if(theVectorVariableMap.find(s) != theVectorVariableMap.end() ){
    return theVectorVariableMap[s];
  }else {
    cout<<"***Bad input in vector variable map "<<s<<" ***"<<endl;
    return NULL;
  }
}


void Correctable::ParseVectorInput(string in,string &out,int &index){
  //Method takes string "in" of form array[index] and sets "out" to array
  //and sets "index" to index

  vector <string> elems;
  stringstream ss(in);
  string item;

  getline(ss, item,'[');
  elems.push_back(item); //push array part
  getline(ss, item,'[');
  elems.push_back(item); //push num] part

  stringstream ss2(elems[1]);
  getline(ss2, item,']');

  out =elems[0];
  index =atoi(item.c_str());

}


void Correctable::DefineCorrection(string time, string otherVar,vector<Double_t> coefs,Int_t channel){

  string temp; int index;
  ParseVectorInput(otherVar,temp,index);// void ParseVectorInput(string in,string & out,int &index);

  CorrectionInfo i;
  if (temp != otherVar){
    //it was an array/vector 
    //check that the base name was mapped
    if ( Get(time)!=NULL &&Get(temp)!=NULL){
      i.time = (Double_t*)Get(time);
      i.correctingVarVec = GetVector(temp);
      cout<<"address is "<<i.correctingVarVec<<endl;
      i.isArray=true;
      i.index=index;
    }
  }else {
    //it was not an array input 
    if (Get(time)!=NULL && Get(otherVar)!=NULL){
      i.time = (Double_t*)Get(time);
      i.correctingVar = (Double_t*)Get(otherVar);
    }
  }
  

  i.firstName = time;
  i.secondName = otherVar; 
  i.coefs = coefs;
  i.channel =channel;
  stringstream s;
  s<<time<<"_"<<otherVar<<"ch"<<channel;
  
  corrections.push_back(i);
  correctionKeys.push_back(s.str());
  mapForCorrectionResults[s.str()]=correctionCount;
  correctionCount++;
  //    theDynamicCorrectionResults.resize(correctionCount,-1);
  AddMapEntry(s.str(),&Corrections[correctionCount-1]);
  //theDynamicCorrectionResults[correctionCount -1] gets set to something when corrections are applied
  //in Appply dynamic Corrections

}
void Correctable::DefineCorrection(string time, string otherVar,TF1 * func,Int_t channel){

  string temp; int index;
  ParseVectorInput(otherVar,temp,index);// void ParseVectorInput(string in,string & out,int &index);

  CorrectionInfo i;
  if (temp != otherVar){
    //it was an array/vector 
    //check that the base name was mapped
    if ( Get(time)!=NULL &&Get(temp)!=NULL){
      i.time = (Double_t*)Get(time);
      i.correctingVarVec = GetVector(temp);
      cout<<"address is "<<i.correctingVarVec<<endl;
      i.isArray=true;
      i.index=index;
    }
  }else {
    //it was not an array input 
    if (Get(time)!=NULL && Get(otherVar)!=NULL){
      i.time = (Double_t*)Get(time);
      i.correctingVar = (Double_t*)Get(otherVar);
    }
  }

  i.firstName = time;
  i.secondName = otherVar; 
  i.theFunction = func;
  i.isTF1=true;
  i.channel =channel;
  stringstream s;
  s<<time<<"_"<<otherVar<<"ch"<<channel;
  
  corrections.push_back(i);
  correctionKeys.push_back(s.str());
  mapForCorrectionResults[s.str()]=correctionCount;
  correctionCount++;
  //    theDynamicCorrectionResults.resize(correctionCount,-1);
  AddMapEntry(s.str(),&Corrections[correctionCount-1]);
  //theDynamicCorrectionResults[correctionCount -1] gets set to something when corrections are applied
  //in Appply dynamic Corrections
  
}


void Correctable::Reset(){
  mapForCorrectionResults.clear();
  //  mapForCorrectionResults["null"]=-1;
}


void Correctable::DumpMappedVariables(){

  cout<<"****Dump Mapped Variables****"<<endl<<endl;
  for (map<string,void*>::iterator ii=theVariableMap.begin();ii!=theVariableMap.end();ii++){
    cout<<ii->first<<endl;
  }

  cout<<"****Dump Mapped Vector Variables****"<<endl<<endl;
  for (map<string,vector<Double_t>* >::iterator ii=theVectorVariableMap.begin();ii!=theVectorVariableMap.end();ii++){
    cout<<ii->first<<endl;
  }
}
void Correctable::DumpResultVector(){
  cout<<"****Dump Result Vector****"<<endl;
  for (int i=0;i<correctionCount;i++){
    cout<<"Corrected value at "<<i<<" is "<<Corrections[i]<<endl;

  }

}



void Correctable::DumpCorrectionsMap(){
  
  cout<<"\n****Dumping Dynamically defined corrections****\n"<<endl;
  for (int i=0;i<corrections.size();i++){
    if (Get(correctionKeys[i]) != NULL){
      cout<<"Correction for variables "<<correctionKeys[i]<<" For Channel "<<corrections[i].channel<<endl;
      
      int size = corrections[i].coefs.size();
      cout<<"Corrected time = "<<corrections[i].firstName<<" - (";
      for (int j=0;j<size;j++){
	cout<<corrections[i].coefs[j]<<"*"<<corrections[i].secondName<<"^"<<j+1;
	if (j == size-1) //the last one
	  cout<<")"<<endl;
	else
	  cout<<"+";
      }
      
    }
  }
}

void Correctable::DumpResultMap(){
  cout<<"****Dump Results Map****"<<endl;
  for (map <string,int>::iterator ii = mapForCorrectionResults.begin();
       ii!=mapForCorrectionResults.end();ii++){
    
    cout<<"The correction "<<ii->first<<" is at spot "<<ii->second<<" in result vector"<<endl;

  }
}
void Correctable::DumpCorrectable(){
  cout<<"__________Dump all Correction info_______________"<<endl;
  DumpCorrectionsMap();
  DumpMappedVariables();
  DumpResultMap();
  DumpResultVector();
}



void Correctable::ApplyDynamicCorrections(){
  int spot=-1;
  for (int ii=0;ii<corrections.size();ii++){ 
    CorrectionInfo theInfo = corrections[ii];
    string theName = correctionKeys[ii];
    if (mapForCorrectionResults.find(theName) != mapForCorrectionResults.end() ){
      //      cout<<"Apply correction for "<<ii->first<<endl;
      
      spot=mapForCorrectionResults[theName];//get the spot for this correction
      //Calculate the correction;
      int degree = theInfo.coefs.size();
      Double_t tempTotal=0;
      Double_t theValue=0;
      //if it is a correction to with respect to a simle variable just get corVar from info
      if (!theInfo.isArray) 
	theValue=*theInfo.correctingVar; 
      else{// if it is a vector get the vector from the pointer in the vector spot in info
	vector <Double_t> v =*(theInfo.correctingVarVec);
	if(v.size()<=theInfo.index){ // if the size of the vector is less then the num in
	  // vector[num] that was given when correcton was defined throw a warning
	  cout<<"***Warning attempting to apply correction with "<<theInfo.secondName<<" which is undecleared.  Size is "<<v.size()<<" ***"<<endl;
	}else{ // else set the value to the coresponding spot in the vector
	  theValue=v[theInfo.index];
	}
      }
      //total up the correction
      if (!theInfo.isTF1){ // if was given as a vector of coefs total it up
	for (int i=0;i<degree;i++){
	  tempTotal=tempTotal+theInfo.coefs[i]*(TMath::Power(theValue,i+1));
	}
      } else { //if it was given as a TF1 then call it to get the value
	tempTotal=(*theInfo.theFunction)(theValue);
      }
      //put the result in result vector.  The mappig to this value was created 
      //in define correction when the correction was defined 
      Corrections[spot]=(*theInfo.time-tempTotal);
    } else {
      cout<<"*** Warning the correction "<<theName<<" not found"<<endl;
    }
  }
  
}






 










/*
void Correctable::PrintCorrection(string in){
  if (correctionsMap.find(in) == correctionsMap.end() ){

    cout<<"Correction "<<in <<" not found in map "<<endl;
    cout<<"Avaiable Corrections are "<<endl;
    DumpCorrectionsMap();
  } else {
    
    CorrectionInfo II = correctionsMap[in];
    
    cout<<"Correction for Variable "<<in <<" for channel "<<II.channel<<endl;
    int size = II.coefs.size();
    cout<<"Corrected time = "<<II.firstName<<" - (";
    for (int i=0;i<size;i++){
      cout<<II.coefs[i]<<"*"<<II.secondName<<"^"<<i;
      if (i == size-1) //the last one
	cout<<")"<<endl;
      else
	cout<<"+";
    }
  }
}
*/
