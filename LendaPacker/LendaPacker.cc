
#include "LendaPacker.hh"


LendaPacker::LendaPacker(){
  ///Use bad Defaults to ensure that the calling program set 
  //everything
  fFL=-1; 
  fFG=-1;
  fd=-1;
  fw=-1;
  lg=-1;
  sg=-1;
  lg2=-1;
  sg2=-1;
  traceDelay=-1;
  jentry=-1;
  Reset();
  theChannel=NULL;
}

void LendaPacker::SetFilter(Int_t _FL,Int_t _FG,Int_t _d,Int_t _w){
  fFL=_FL;
  fFG=_FG;
  fd=_d;
  fw=_w;
}

void LendaPacker::SetGates(Double_t _lg,Double_t _sg,Double_t _lg2,Double_t _sg2){

  lg=_lg;
  sg=_sg;
  lg2=_lg2;
  sg2=_sg2;
} 


LendaPacker::~LendaPacker(){


}

void LendaPacker::Reset(){

  thisEventsFF.clear();
  thisEventsCFD.clear();
  theChannel=NULL;

  thisEventsIntegral=0;
  thisEventsPulseHeight=0;
  longGate=0;
  shortGate=0;
  cubicCFD=0;
  softwareCFD=0;
  start=0;
  numZeroCrossings=0;
  

}
void LendaPacker::CalcTimeFilters(){


  theFilter.FastFilter(theChannel->trace,thisEventsFF,fFL,fFG); //run FF algorithim
  thisEventsCFD = theFilter.CFD(thisEventsFF,fd,fw); //run CFD algorithim

  softwareCFD=theFilter.GetZeroCrossing(thisEventsCFD,numZeroCrossings)-traceDelay; //find zeroCrossig of CFD

  //  cubicCFD = theFilter.GetZeroCubic(thisEventsCFD)-traceDelay;

 
}
void LendaPacker::CalcEnergyGates(){
  //theTrace = trace;//Stor the trace in packer for later 
  if (softwareCFD!=0)
    start = theFilter.getStartForPulseShape(softwareCFD,traceDelay);
  else{
    softwareCFD=theFilter.GetZeroCrossing(thisEventsCFD,numZeroCrossings)-traceDelay;
    start = theFilter.getStartForPulseShape(softwareCFD,traceDelay);
  }

  thisEventsPulseHeight=theFilter.getMaxPulseHeight(theChannel->trace);
    
  thisEventsIntegral = theFilter.getEnergy(theChannel->trace);
  if (theChannel->chanid ==9){
    longGate = theFilter.getGate(theChannel->trace,start,lg);
    shortGate = theFilter.getGate(theChannel->trace,start,sg);
  } else if (theChannel->chanid ==8 ){
    longGate = theFilter.getGate(theChannel->trace,start,lg2);
    shortGate = theFilter.getGate(theChannel->trace,start,sg2);
  }
  // if ((softwareCFD <0 || softwareCFD>3)&&theChannel->chanid!=0&&theChannel->chanid!=1){
  //   cout<<"jentry is "<<jentry<<endl;
  //   cout<<"softCFD "<<softwareCFD<<endl;
  //   cout<<"start is "<<start<<endl;
  //   cout<<"A Problem with "<<theChannel->chanid<<endl;
  // }
}

void LendaPacker::PackEvent(LendaEvent * Event){


  Event->pushTrace(theChannel->trace);//save the trace for later 
  
  Event->pushFilter(thisEventsFF); //save filter if it is there
  Event->pushCFD(thisEventsCFD); //save CFD if it is there
  
  //Push other thing into the event
  Event->pushLongGate(longGate); //longer integration window
  Event->pushShortGate(shortGate);//shorter integration window
  Event->pushChannel(theChannel->chanid);//the channel for this pulse
  Event->pushEnergy(thisEventsIntegral);;//push trace energy if there
  Event->pushInternEnergy(theChannel->energy);//push internal energy
  Event->pushTime(theChannel->time);
  Event->pushTimeLow(theChannel->timelow);
  Event->pushTimeHigh(theChannel->timehigh);
  Event->pushSoftTime(2*(theChannel->timelow + theChannel->timehigh * 4294967296.0) +
		      softwareCFD -theChannel->GetCFDTriggerSourceBit());
  //  Event->pushSoftTime(2*(theChannel->timelow + theChannel->timehigh * 4294967296.0) - theChannel->GetCFDTriggerSourceBit()+softwareCFD)
  Event->pushSoftwareCFD(softwareCFD);
  Event->pushCubicCFD(cubicCFD);
  Event->pushCubicTime(theChannel->timelow +theChannel->timehigh*4294967296.0+cubicCFD);//WRong
  Event->pushInternalCFD((theChannel->timecfd)/32768.0);
  Event->pushEntryNum(jentry);
  Event->pushNumZeroCrossings(numZeroCrossings);
  Event->pushPulseHeight(thisEventsPulseHeight);

  Reset();//Reset the Packers variables
}

void LendaPacker::CalcAll(){
   CalcTimeFilters();
   CalcEnergyGates();
}




void LendaPacker::RePackEvent(LendaEvent * Event){



}

void LendaPacker::PackManySoftwareTimes(LendaEvent *Event){
  //Event should already be packed  

  int FL_low=5; 
  int FL_high=6;
  
  int FG_low=0;
  int FG_high=4;
  
  int d_low=2;
  int d_high=6;
  
  int w_low=0;
  int w_high=4;


  int num;
  for (int i=0;i<(int)Event->times.size();i++){
    for (int FL=FL_low; FL<FL_high ;FL++){
      for (int FG=FG_low; FG<FG_high ;FG++){
	for (int d=d_low; d<d_high ;d++){
	  for (int w=w_low; w<w_high ;w++){
	    vector <Double_t> tempFF;
	    vector <Double_t> tempCFD;
	    
	    theFilter.FastFilterOp(Event->Traces[i],tempFF,FL,FG); //run FF algorithim
	    tempCFD = theFilter.CFDOp(tempFF,d,w); //run CFD algorithim
	    Double_t Basetime = 2*(Event->timeLows[i] + Event->timeHighs[i] * 4294967296.0);
	    Double_t temp = theFilter.GetZeroCrossingOp(tempCFD,num)-traceDelay+Basetime;
	    
	    Event->pushSoftwareTime(Event->channels[i],FL,FG,d,w,temp);
	  }
	}
      }
    }
  }



}
