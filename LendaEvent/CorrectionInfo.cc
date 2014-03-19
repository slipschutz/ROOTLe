

#include "CorrectionInfo.hh"

CorrectionInfo::CorrectionInfo(): time(NULL), correctingVar(NULL),firstName(""),secondName(""),correctingVarVec(NULL),channel(-1),
				  index(-1),isArray(false),theFunction(NULL),isTF1(false){
  coefs.clear();

  
}
CorrectionInfo::~CorrectionInfo(){
  coefs.clear();


}
