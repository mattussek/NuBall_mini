#ifndef Clover_h
#define Clover_h

//This is of course not ideal... have this set in NuBall_ana.h! Here only default value...
#ifndef BGO_SETTINGS
   #define BGO_SETTINGS
   #define BGO_VETO_THRESH 5
   #define BGO_GE_WINDOW_LOW -200
   #define BGO_GE_WINDOW_HI  200
#endif

   #define MAKE_BGO_GE_MULT 1
   
#ifndef ADDBACK_SETTINGS
   #define ADDBACK_SETTINGS
   #define ADDBACK_THRESH 5           //must be >= 0!
   #define ADDBACK_WINDOW_LOW -200
   #define ADDBACK_WINDOW_HI 200
#endif

#include <TH1.h>
#include <TH2.h>
#include <TFile.h>

class Clover 
{
private:
   int      GeLabel[4];
   int      BgoLabel[2];
   bool     useAddBack;
public:
   int ringNr;
   int detNr;
   int hasGe;
   int hasBgo;
   bool isGood;
   bool hasVeto;

   Double_t dt;

   Long64_t CloverTime;    //Only really meaningful with AddBack enabled
   Double_t CloverEnergy;  //Only really meaningful with AddBack enabled
   Int_t    CloverNr;

   Long64_t GeTime[4];
   Double_t GeEnergy[4];
   
   Long64_t BgoTime[2];
   Double_t BgoEnergy[2];



   TH2D hsubdet;     //- Always has all the events (all multiplicities)
   TH1D hGe_single;  //-
   TH1D hBgo_single; //-
   TH1D hGeMult;     //-
   
   TH2D hBgoGeMult;
   
   TH1D hGe_BgoVeto; //+ From here on depending on useAddBack. If ON:
   TH1D hGe_CompSup; //+ Spectra are only multiplicty 1 and 2 per detector. mult > 2 are discarded
   TH2D hGeBgo;      //+ as well as mult 2 events with diagonal hit pattern.
   TH1D hdt;         //+ if add-back is not enabled then also mult==2 events are discarded.
   TH1D hGe;         //+
   TH2D hGeGe;       //+
   TH1D hAddBackdt[4];//+    Only filled if addback is ON
   TH1D hGeBgodt;    //+
 
   Clover(int ring, int number, int GeA, int GeB, int GeC, int GeD, int Bgo1, int Bgo2, bool addback = 1);
   ~Clover() {};
   void reset();
   void PrintSetting();
   void WriteHistograms(TFile *thisFile);
   bool ProcessGe (int this_label, Double_t this_energy, Long64_t this_time); //returns 1 iff label belongs to this detector
   bool ProcessBgo(int this_label, Double_t this_energy, Long64_t this_time); //returns 1 iff label belongs to this detector      
   bool AnalyseEvent(); //returns 1 if CloverEnergy and CloverTime could be defined, otherwise 0.
   
//   Double_t E_Add();
//   Double_t E_CompSup();
//   Double_t E_CompSupAdd();
   

   ClassDef(Clover,0);
};

#endif




