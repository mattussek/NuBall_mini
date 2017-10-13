#ifndef Clover_h
#define Clover_h

//add CloverEvent class for eventbuilder later
#define BGO_VETO_THRESH 5
#define BGO_GE_WINDOW_LOW -200
#define BGO_GE_WINDOW_HI  200
#define ADDBACK_THRESH 20
#define ADDBACK_WINDOW_LOW -400
#define ADDBACK_WINDOW_HI   400

#define DO_ADDBACK 1 //Not implemented yet!

#include <TH1.h>
#include <TH2.h>

class Clover 
{
private:
   int      GeLabel[4];
   int      BgoLabel[2];
   bool     useAddBack;
public:
   int detNr;
   int hasGe;
   int hasBgo;
   bool didAddBack;
   bool hasVeto;

   Double_t dt;

   Long64_t CloverTime;
   Double_t CloverEnergy;

   Long64_t GeTime[4];
   Double_t GeEnergy[4];
   
   Long64_t BgoTime[2];
   Double_t BgoEnergy[2];



   TH1D hGe_BgoVeto; // only from events where no add-back was done, vetoed Ge gammas
   TH1D hGe_single; // all Ge gammas, multi-hit not added back
   TH1D hGe_noAdd; //only non-add-back events
   TH1D hGe_Add; // only add-back events
   TH1D hGe_CompSup; // only from events where no add-back was done, compton suppressed
   TH1D hGe_CompSupAdd;// only compton suppressed add-back events.
   TH1D hBgo_single; //
   TH2D hsubdet; // all segments and bgos seperately
   TH2D hGeBgo; // coinc. matrix of Bgo and vetoed Ge gammas
 
   Clover(int number, int GeA, int GeB, int GeC, int GeD, int Bgo1, int Bgo2);
   ~Clover() {};
   void reset();
   void PrintSetting();
   void WriteHistograms();
   bool ProcessGe (int this_label, Double_t this_energy, Long64_t this_time); //returns 1 iff label belongs to this detector
   bool ProcessBgo(int this_label, Double_t this_energy, Long64_t this_time); //returns 1 iff label belongs to this detector      
   bool AnalyseEvent();
   
//   Double_t E_Add();
//   Double_t E_CompSup();
//   Double_t E_CompSupAdd();
   

   ClassDef(Clover,0);
};

#endif




