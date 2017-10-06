//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Sep 28 15:29:15 2017 by ROOT version 5.34/14
// from TTree DataTree/faster to tree root test : DATAFILENAME
// found on file: RAWDATA/rootfiles/60Co_first_mini_nuball_0001.root
//////////////////////////////////////////////////////////

#ifndef NuBall_eventbuilderR5_h
#define NuBall_eventbuilderR5_h

#define MAX_ITEMS 20
#define MAX_LABEL 40
#define COINC_WIDTH 2000
#define T_REF 28
#define CALIBFILE 

#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH2.h>


// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class NuBall_eventbuilderR5 : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Int_t           label;
   Int_t           nrj;
   Long64_t        time;

   // List of branches
   TBranch        *b_label;   //!
   TBranch        *b_nrj;   //!
   TBranch        *b_time;   //!

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


   NuBall_eventbuilderR5(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~NuBall_eventbuilderR5() { }
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

   ClassDef(NuBall_eventbuilderR5,0);
};

#endif

#ifdef NuBall_eventbuilderR5_cxx
void NuBall_eventbuilderR5::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("label", &label, &b_label);
   fChain->SetBranchAddress("nrj", &nrj, &b_nrj);
   fChain->SetBranchAddress("time", &time, &b_time);
}

Bool_t NuBall_eventbuilderR5::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef NuBall_eventbuilderR5_cxx
