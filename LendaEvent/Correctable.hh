/*
//
// Class providing the map between variable names and their instances
// in the child classes.
//
// With this map the class provides dynamically defined corrections between variables in the map
// The results of these corrections are put into the array Corrections
*/

#ifndef __CORRECTABLE_HH
#define __CORRECTABLE_HH

#include "TObject.h"
#include "TF1.h"
#include <map>
#include <string>
#include <utility> 
#include <iostream>
#include "CorrectionInfo.hh"
using namespace std;

class Correctable : public TObject { 
public:
  Correctable(); //Constructor 
  virtual ~Correctable();// de-constructor


  //Hide somethings from the interpreter 
#ifndef __CINT__ 
  map <std::string,void*> theVariableMap; //map for variables (only for simples types)
  map <std::string,vector<Double_t>* > theVectorVariableMap; //map for vectors all Double_t's
  vector <CorrectionInfo> corrections; //Vector holding the correction info for each correction
  Int_t correctionCount; //how many corrections have been defined
  map <string,int> mapForCorrectionResults;//map to where each correction is in the Results vector
  vector <string> correctionKeys; //vector holding the correction names in the order they were defined
#endif



  void AddMapEntry(std::string,void*);//method to add entry to variableMap
  void AddMapEntry(std::string,vector <Double_t>*);//method to add entry to vector Variable Map
  //Define pure virutal method for defining the Maps
  virtual void DefineMap()=0;//method defining the variables in the map can be auto-generated

  void * Get(std::string); //Get variable from variable Map
  vector<Double_t>* GetVector(string); //get vector from vector map
  
  //Define a correction.  
  void DefineCorrection(string time, string otherVar,vector<Double_t> coefs,Int_t channel);//for polynomial corrections
  void DefineCorrection(string time, string otherVar,TF1 * func,Int_t channel);//arbitrary TF1


  
  Double_t Corrections[100];//the array holding the dynamical defined correction results

  //Methods for printing info containted in this class
  void DumpCorrectable(); //Dumps everything
  void DumpCorrectionsMap();//Dumps the Dynamicaly defined corrections
  void DumpMappedVariables();//Dumps the map of variables
  void DumpResultMap(); // Dump the map of where correcions are in the corrections array
  void DumpResultVector(); //dumps the Conents of the Corrections array


  void Reset();
  void ApplyDynamicCorrections(); // Method to calculate the corrections for each event

private:
  void ParseVectorInput(string in,string & out,int &index);//methods for parsing array[index] input


public:
  ClassDef(Correctable,1);
};

#endif

