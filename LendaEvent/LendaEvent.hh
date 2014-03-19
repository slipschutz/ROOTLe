#ifndef __LENDA_EVENT_HH
#define __LENDA_EVENT_HH

#include "TObject.h"
#include "TFile.h"
#include "Correctable.hh"
#include <vector>
#include <map>
#include "Settings.hh"
using namespace std;

class LendaEvent :  public Correctable  {
public:

  LendaEvent(); //Constructor 
  LendaEvent(bool BuildMap);
  void Clear(); //Clear the Event.  Should be called after every write to a tree

  void Finalize(); //Applies internal corrections and calculates convient Branches/leaves

  virtual void DefineMap(); //Inherited from Correctable defines var look up
  void PrintList();
 
  //Convenient quantities calculated in Finalize
  Double_t TOF; // Time of flight //__var
  Double_t Dt; //Diference between Top and botom PMTs //__var
  Double_t ShiftTOF;
  Double_t ShiftTOFInternal; //Shift TOF for internal Tumes 
  Double_t TOFEnergy;
  Double_t TOFEnergyInternal;


  Bool_t ErrorBit;
  
  Bool_t lean;//Flag for whether to save traces/filters

  Double_t CDt;//__var

  Int_t N;


  
  Int_t NumBadPoints;//__var

  Double_t PulseShape; //long gate over short gate  //__var
  Double_t GOE;   //Center of Gravity (E1-E2)/(E1+E2)  //__var
  Double_t CorGOE;//Center of Gravity after gain matching //__var

  Double_t GammaPeakTime;
  Double_t GammaPeakTimeLiqLiq;
  
  Double_t TOFFudge;
  
  vector <Bool_t> OverFlows;
  vector <Int_t > ZeroCrossings;
  //  vector<Double_t> Corrections;
  #ifndef __CINT__
  map <string,int> CorMap;
  #endif
  //Main information holders
  vector <Double_t> energiesCor; //the corrected energies 
  vector <Double_t> times; //the times
  vector <Double_t> softTimes;
  vector <Double_t> cubicTimes;
  vector <Double_t> energies; // the raw energies
  vector <Double_t> internEnergies;
  vector <Double_t> channels; // the channels
  vector <Double_t> softwareCFDs;
  vector <Double_t> cubicCFDs;
  vector <Double_t> internalCFDs;
  vector <Long64_t> entryNums;

  vector <Double_t> shortGates;  //short gate energies
  vector <Double_t> longGates;  //long gate energies

  vector <Double_t> pulseHeights;

  //The Traces and Filters
  vector < vector <UShort_t> > Traces; // a vector of vectors holding the traces if present
  vector < vector <Double_t> > Filters; //a vector of vectors for the filters if present
  vector < vector <Double_t> > CFDs;  // a vector of vectors for the CFDs if present
  


  //C Trace thing
  //root like c arrays more then vectors of vectors
  void MakeC(int spot=0);  //copy trace,FF,CFD data for the spot-th trace into C-arrays
  //the c-array to store the trace in



  //pushing methods expects the series of times and energies and everything else to be ordered by channel
  void pushLongGate(Double_t);
  void pushShortGate(Double_t);
  void pushEnergy(Double_t);
  void pushInternEnergy(Double_t);
  void pushTime(Double_t);
  void pushChannel(Double_t);
  void pushTrace (vector <UShort_t>);
  void pushFilter (vector <Double_t>);
  void pushCFD (vector <Double_t>);
  void pushInternalCFD(Double_t);
  void pushSoftwareCFD(Double_t);
  void pushSoftTime(Double_t);
  void pushEntryNum(Long64_t);
  void pushCubicTime(Double_t);
  void pushCubicCFD(Double_t);

  void pushPulseHeight(Double_t);
  void pushNumZeroCrossings(Int_t);
  Int_t NumOfChannelsInEvent; //The Number of channels associated with this event


  //Corrections setting methods

  void setGainCorrections(Double_t slope,Double_t c,Int_t channel);//sets the gain correction for a channel
  void setGainCorrections(vector <pair <Double_t,Double_t> > );//wrapper to the above assumes the vector has channel 0 at 
                                              //index 0 and channel 1 at index 1 ...

  inline void SetGammaPeakTime(Double_t t){GammaPeakTime=t;}
  inline void SetGammaPeakTimeLiqLiq(Double_t t){GammaPeakTimeLiqLiq=t;}
  inline void SetTOFFudge(Double_t t){TOFFudge=t;}
  inline void SetLean(Bool_t b){lean=b;}

  void DumpGainCorrections();
  void DumpAllCorrections();



  
  LendaEvent& operator = (const LendaEvent &right);



  void Fatal();//overload this stupid inherited method


  void WriteSettings(Settings * theSettings);

private:

  void gainCor(); //method to apply the walk corrections



  vector < pair<Double_t,Double_t> > fgainCorrections; //the gain corrections 


  int fnumOfGainCorrections;// number of Gain corrections

  
public:
  ClassDef(LendaEvent, 19);
};

#endif



