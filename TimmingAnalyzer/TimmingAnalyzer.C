#include <stdio.h>
#include <stdlib.h>
//test
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include <TRandom1.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include "TRandom3.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TGraph.h"
#include "TChain.h"
#include "TRandom2.h"

//Local Headers
#include "LendaEvent.hh"
#include "ddaschannel.h"
#include "DDASEvent.h"
#include "Filter.hh"

#include <ctime>
#include <iomanip>
#include "Settings.hh"
#include "LendaPacker.hh"

#include "TFitResult.h"
#include "TF1.h"
#define BAD_NUM -10008


using namespace std;


int main(int argc, char **argv){

  //Make a packer object
  LendaPacker *thePacker = new LendaPacker();
  
  

  //prepare files 
  ////////////////////////////////////////////////////////////////////////////////////
  TFile *inFile = new TFile("~/analysis/run401/FL6FG0d6w10run-0401--softwareCFD.root");
  TTree* inT = (TTree*)inFile->Get("flt");
  Long64_t nentry=(Long64_t) (inT->GetEntries());
  cout <<"The number of entires is : "<< nentry << endl ;
  

  // Openning output Tree and output file
  ////////////////////////////////////////////////////////////////////////////////////
  TFile *outFile = new TFile("Result.root","recreate");
  
  int FL_low=1;
  int FL_high=3;
  int FG_low=0;
  int FG_high=1;
  int w_low=0;
  int w_high=1;
  int d_low=3;
  int d_high=4;

  int NumberOfFilterSets = (FL_high-FL_low)*(FG_high-FG_low)*(w_high-w_low)*(d_high-d_low);

  vector <TH1F*> TheHistograms(NumberOfFilterSets);
  stringstream nameStream;
  int count =0;
  for (int FL=FL_low;FL<FL_high;FL++){
    for (int FG=FG_low;FG<FG_high;FG++){
      for (int w=w_low;w<w_high;w++){
	for (int d=d_low;d<d_high;d++){
	  nameStream.str("");
	  nameStream<<"FL"<<FL<<"FG"<<FG<<"w"<<w<<"d"<<d;
	  TheHistograms[count]=new TH1F(nameStream.str().c_str(),"Title",100,-5,5);
	  count++;
	}
      }
    }
  }

  TH1F * InternalTimes = new TH1F("InternalTimes","",100,-5,5);
  TH1F * SoftTimes = new TH1F("SoftTimes","",100,-5,5);


  
  // set input tree branvh variables and addresses
  ////////////////////////////////////////////////////////////////////////////////////
  
  //Specify the output branch
  LendaEvent* Event = new LendaEvent();
  inT->SetBranchAddress("Event",&Event);
  //  outT->BranchRef();
  
  //non branch timing variables 
     ////////////////////////////////////////////////////////////////////////////////////


  //  Filter theFilter; // Filter object
  ////////////////////////////////////////////////////////////////////////////////////


  //  vector <Sl_Event*> CorrelatedEvents(4,NULL);
  //  map <Long64_t,bool> mapOfUsedEntries;//Used to prevent double counting
  
  clock_t startTime;
  clock_t otherTime;
  double timeRate=0;
  bool timeFlag=true;
  startTime = clock();
  //  nentry=1000;
  for (Long64_t jentry=0; jentry<nentry;jentry++) { // Main analysis loop


    inT->GetEntry(jentry); // Get the event from the input tree 
    if (Event->N==2&&Event->channels[0]==0&&Event->channels[1]==1){
      //Loop over the all the filters in the same way as above
      SoftTimes->Fill(Event->softTimes[0]-Event->softTimes[1]);
      int count =0;
      for (int FL=FL_low;FL<FL_high;FL++){
	for (int FG=FG_low;FG<FG_high;FG++){
	  for (int w=w_low;w<w_high;w++){
	    for (int d=d_low;d<d_high;d++){
	      thePacker->SetFilter(FL,FG,d,w);
	      thePacker->RePackSoftwareTimes(Event);
	      //	      TheHistograms[count]->Fill(0.5*(Event->softTimes[0]+Event->softTimes[1]-Event->softTimes[2]-Event->softTimes[3]));
	      TheHistograms[count]->Fill(Event->softTimes[0]-Event->softTimes[1]);
	      count++;
	    }
	  }
	}
      }
    
      // InternalTimes->Fill(0.5*(Event->times[0]+Event->times[1]-
      //  			       Event->times[2]-Event->times[3]));
      InternalTimes->Fill(Event->times[0]-Event->times[1]);
      
    }
    //Periodic printing
    if (jentry % 10000 <10 && jentry >=20000 && timeFlag){
      timeFlag=false;
      otherTime=clock();
      timeRate = TMath::Abs( double((startTime-otherTime))/double(CLOCKS_PER_SEC));
      timeRate = timeRate/jentry;
    }
    //Periodic printing
    if (jentry % 1000 ==0 ){
      cout<<flush<<"\r"<<"                                                                                          "<<"\r";
      cout<<"On Event "<<jentry<<" "<<((double)jentry)/(nentry)*100<<"% minutes remaining "<<(1.0/60)*timeRate*(nentry-jentry)<<" hours remaining "<<(1.0/3600)*timeRate*(nentry-jentry);
    }
    //cout<<right<<"On event "<<setw(9)<<jentry<<" "<<setprecision(2)<<setw(3)<<((double)jentry)/maxentry*100.0<<"% seconds remaining "<<setprecision(4)<<setw(6)<<timeRate*(maxentry-jentry)<<flush<<"\r";

    
  }//End main analysis loop
  

  TF1 * aGauss = new TF1("aGauss","gaus",-2,2);
  TFitResultPtr result;

  vector <double> theResolutions(NumberOfFilterSets);
  double BestRes=1000;
  string BestName="NONE";
  for (int i=0;i<NumberOfFilterSets;i++){
    result = TheHistograms[i]->Fit("aGauss","QSNR");
    Int_t status=result;
    if (status==0){
      theResolutions[i]=result->Value(2)*2.35*4;
      if (theResolutions[i]<BestRes){
	BestRes=theResolutions[i];
	BestName=TheHistograms[i]->GetName();
      }
    } else {
      cout<<"***Bad Fit For "<<TheHistograms[i]->GetName()<<endl;
    }

  }
  
  ofstream out("./TheResoultions.txt");
  for (int i=0;i<theResolutions.size();i++){
    out<<TheHistograms[i]->GetName()<<"  "<<theResolutions[i]<<endl;
  }
  out<<endl;

  //Close the file
  outFile->Write();
  outFile->Close();
  cout<<"Best Res is "<<BestName<<" "<<BestRes<<endl;
  
  
  cout<<"\n\n**Finished**\n\n";
  
  return  0;
  
}



/*



      if ( TimeDiff < theInputManager.timeWindow ){
	countForwardNoDelay++;//just the count up to the end of the window ignoring the shift
	if ( TimeDiff <theInputManager.timeWindowShift){
	//if still in the window but less than the shift just count up until the shift 
	  countForward++;
	} else if (TimeDiff > theInputManager.timeWindowShift){
	  //if it is less than the window and greater than the shift count AND push the events
	  //this section is where events are pushed when there is no window shift
	  Sl_Event temp;
	  temp.jentry = jentry +countForward;
	  temp.dchan2= ddaschannel(*inChannel);
	  //	mapOfUsedEntries[temp.jentry]=true;
	  EventsInWindow.push_back(temp);
	  countForward++;
	}
      }else if (TimeDiff > theInputManager.timeWindow && TimeDiff< theInputManager.timeWindowShift){
	//just count if you are inbetween end of window and start of delay
	countForward++;
      }else if (TimeDiff>theInputManager.timeWindow && TimeDiff > theInputManager.timeWindowShift&&
		TimeDiff <theInputManager.timeWindow+theInputManager.timeWindowShift){
	//push events when there is a shift and you are in the shifted time window 
	  Sl_Event temp;
	  temp.jentry = jentry +countForward;
	  temp.dchan2= ddaschannel(*inChannel);
	  //	mapOfUsedEntries[temp.jentry]=true;
	  EventsInWindow.push_back(temp);
	  countForward++;
      } else{
	searching =false;
	jentry = jentry+countForward-1;
      }
    


 */

/*
else { //out side of window plus clock tic
	searching =false;// stop looping 
	if (JitteredEventNums.size()==0){
	  jentry =jentry+countForward-1;
	} else {
	  //should go back to first jitter event
	  for (int i=0;i<JitteredEventNums.size();i++){
	    cout<<JitteredEventNums[i]<<endl;
	  }
	  jentry = JitteredEventNums[0]-1;
	  cout<<"in jittered land"<<endl;
	  int t ;cin>>t;
	  
	}
      }

 */

/*
    ///////////////////////////////////////////////////////////////////////////////////////////
    if (CorrelatedEvents[inChannel->chanid]==NULL){
      Sl_Event *e=new Sl_Event();
      e->jentry=jentry;
      e->dchan =  new ddaschannel(*inChannel);//copy the info
      CorrelatedEvents[inChannel->chanid] =e;
    } else {
      if (TMath::Abs(CorrelatedEvents[inChannel->chanid]->dchan->time 
		     -inChannel->time)<10){
	//repeat channel within window throw away
	delete CorrelatedEvents[inChannel->chanid];
	CorrelatedEvents[inChannel->chanid]=NULL;
      }else {
	//update the spot if the current event happened after the stored one
	//ie keep the most recent one
	delete CorrelatedEvents[inChannel->chanid];
	Sl_Event *e=new Sl_Event();
	e->jentry=jentry;
	e->dchan =  new ddaschannel(*inChannel);//copy the info
	CorrelatedEvents[inChannel->chanid] =e;
      }
    }
  
  
    //bar 11 check
    if(CorrelatedEvents[0]!=NULL && CorrelatedEvents[1] !=NULL&&
       TMath::Abs(CorrelatedEvents[0]->dchan->time - CorrelatedEvents[1]->dchan->time)<10){
       for (int i=0;i<2;i++){
	cout<<"I is "<<i<<endl;
	cout<<"Jentry "<<CorrelatedEvents[i]->jentry<<endl;
	cout<<"time is "<<CorrelatedEvents[i]->dchan->time<<endl;
	}
      vector<Sl_Event*>::const_iterator first = CorrelatedEvents.begin() + 0;
      vector<Sl_Event*>::const_iterator last = CorrelatedEvents.begin() + 2;

      vector<Sl_Event*> newVec(first, last);
      packEvent(Event,newVec,theFilter,FL,FG,CFD_delay,CFD_scale_factor);
      outT->Fill();
      Event->Clear();
      delete CorrelatedEvents[0];CorrelatedEvents[0]=NULL;
      delete CorrelatedEvents[1];CorrelatedEvents[1]=NULL;
    }

    //bar 23 check
    if(CorrelatedEvents[2]!=NULL && CorrelatedEvents[3] !=NULL&&
       TMath::Abs(CorrelatedEvents[2]->dchan->time - CorrelatedEvents[3]->dchan->time)<10){
      for (int i=2;i<4;i++){
	cout<<"I is "<<i<<endl;
	cout<<"Jentry "<<CorrelatedEvents[i]->jentry<<endl;
	cout<<"time is "<<CorrelatedEvents[i]->dchan->time<<endl;
	}
      vector<Sl_Event*>::const_iterator first = CorrelatedEvents.begin() + 2;
      vector<Sl_Event*>::const_iterator last = CorrelatedEvents.begin() + 4;

      vector<Sl_Event*> newVec(first, last);
      packEvent(Event,newVec,theFilter,FL,FG,CFD_delay,CFD_scale_factor);
      outT->Fill();
      Event->Clear();
      //remove those events from CorrelatedEvents to prevent over counting
      delete CorrelatedEvents[2];CorrelatedEvents[2]=NULL;
      delete CorrelatedEvents[3];CorrelatedEvents[3]=NULL;

    }
*/
