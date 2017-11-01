#ifndef Phase1_h
#define Phase1_h

//This is of course not ideal... have this set in NuBall_ana.h! Here only default value...
#ifndef BGO_SETTINGS
   #define BGO_SETTINGS
   #define BGO_VETO_THRESH 5
   #define BGO_GE_WINDOW_LOW -200
   #define BGO_GE_WINDOW_HI  200
#endif

#include <TH1.h>
#include <TH2.h>
#include <TFile.h>

class Phase1
{
private:
   int      GeLabel;
   int      BgoLabel[2];
public:
   int ringNr;
   int detNr;
   int hasGe;
   int hasBgo;
   bool isGood;
   bool hasVeto;

   Double_t dt;

   Long64_t GeTime;
   Double_t GeEnergy;
   
   Long64_t BgoTime[2];
   Double_t BgoEnergy[2];

   TH2D hsubdet;     //- Always has all the events (all multiplicities)
   TH1D hGe_single;  //-
   TH1D hBgo_single; //-   
   TH1D hGe_BgoVeto; //+ From here on depending on useAddBack. If ON:
   TH1D hGe_CompSup; //+ Spectra are only multiplicty 1 and 2 per detector. mult > 2 are discarded
   TH2D hGeBgo;      //+ as well as mult 2 events with diagonal hit pattern.
   TH1D hdt;         //+ if add-back is not enabled then also mult==2 events are discarded.
   TH1D hGe;         //+
   TH1D hGeBgodt;    //+
 
   Phase1(int ring, int number, int Ge, int Bgo1, int Bgo2);
   ~Phase1() {};
   void reset();
   void PrintSetting();
   void WriteHistograms(TFile *thisFile);
   bool ProcessGe (int this_label, Double_t this_energy, Long64_t this_time); //returns 1 iff label belongs to this detector
   bool ProcessBgo(int this_label, Double_t this_energy, Long64_t this_time); //returns 1 iff label belongs to this detector      
   bool AnalyseEvent(); //returns 1 if GeEnergy and GeTime were defined, otherwise 0.
   
//   Double_t E_Add();
//   Double_t E_CompSup();
//   Double_t E_CompSupAdd();
   

   ClassDef(Phase1,0);
};

#endif




