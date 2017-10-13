//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Oct  6 16:35:55 2017 by ROOT version 6.10/02
// from TTree DataTree/faster to tree root test : DATAFILENAME
// found on file: RAWDATA/rootfiles/60Co_first_mini_nuball_0001.root
//////////////////////////////////////////////////////////

#ifndef NuBall_eventbuilder_h
#define NuBall_eventbuilder_h

#define MAX_ITEMS 20
#define MAX_LABEL 40
#define COINC_WIDTH 2000
#define T_REF 28
#define CALIBFILE 

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TH2.h>

// Headers needed by this particular selector


class NuBall_eventbuilder : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Int_t> label = {fReader, "label"};
   TTreeReaderValue<Int_t> nrj = {fReader, "nrj"};
   TTreeReaderValue<Long64_t> time = {fReader, "time"};
   NuBall_eventbuilder(TTree * /*tree*/ =0) { }
/////branches for coinc tree
   Long64_t coinc_GeTime [MAX_ITEMS];
   Int_t    coinc_GeLabel [MAX_ITEMS];
   Double_t  coinc_GeNrj [MAX_ITEMS];
   Long64_t coinc_BGOTime [MAX_ITEMS];
   Int_t    coinc_BGOLabel [MAX_ITEMS];
   Double_t  coinc_BGONrj [MAX_ITEMS];
   Int_t    coinc_mult;
   Int_t    coinc_Gemult;
   Int_t    coinc_BGOmult;
   Bool_t   coinc_has_ref;

/////////
   char CALIB_FILE[256];
   char TSHIFTS_FILE[256];
   long long T0;
   long long dT;
   long long nCoincidences;
   long long coinc_entry;
   double    calParameters[MAX_LABEL][2];
   int       tshifts[MAX_LABEL];
   Long64_t  lasttime;

   TTree *coinc_tree;
   TFile *outfile;
   TH2D  *hdt;
   TH2D  *hg;
   TH2D  *hGeGe;


   virtual ~NuBall_eventbuilder() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();


   void reset_coinc() {
     memset(coinc_GeTime, 0, sizeof(double)*MAX_ITEMS);
     memset(coinc_GeLabel, 0, sizeof(int)*MAX_ITEMS);
     memset(coinc_GeNrj, 0, sizeof(int)*MAX_ITEMS);
     memset(coinc_BGOTime, 0, sizeof(double)*MAX_ITEMS);
     memset(coinc_BGOLabel, 0, sizeof(int)*MAX_ITEMS);
     memset(coinc_BGONrj, 0, sizeof(int)*MAX_ITEMS);
     coinc_mult=0;
     coinc_BGOmult=0;
     coinc_Gemult=0;
     coinc_has_ref=0;
   }
   void  printCoinc();
   void  printCurrBranch();
   void  fillHistograms();
   void  readCalibration(); // initialses calParameters to 0
   void  readTimeShifts();  // initialises timeshifts to -10000
   double nrjCal(int this_label, int this_nrj);
   bool  isBGO(int this_label);


   ClassDef(NuBall_eventbuilder,0);

};

#endif

#ifdef NuBall_eventbuilder_cxx
void NuBall_eventbuilder::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t NuBall_eventbuilder::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef NuBall_eventbuilder_cxx
