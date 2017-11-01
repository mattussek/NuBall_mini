#ifndef LaBr_h
#define LaBr_h

#include <TH1.h>
#include <TH2.h>
#include <TFile.h>

class LaBr
{
private:
   int      LBLabel;
public:
   int ringNr;
   int detNr;
   int hasGamma;

   Double_t dt;

   Long64_t LBTime;
   Double_t LBEnergy;

   TH1D hGe_single;  //-
 
   LaBr(int ring, int number);
   ~LaBr() {};
   void reset();
   void PrintSetting();
   void WriteHistograms(TFile *thisFile);
   bool ProcessLaBr (int this_label, Double_t this_energy, Long64_t this_time); //returns 1 iff label belongs to this detector
   
   ClassDef(LaBr,0);
};

#endif




