//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Sep 26 12:36:31 2017 by ROOT version 6.08/06
// from TTree DataTree/faster to tree root test : DATAFILENAME
// found on file: 60Co_first_mini_nuball_0001.root
//////////////////////////////////////////////////////////

#ifndef NuballEventBuilder_h
#define NuballEventBuilder_h

#define MAX_ITEMS 15
#define COINC_WIDTH 400
#define T_REF 28

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TH2.h>

// Headers needed by this particular selector




class NuballEventBuilder : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Int_t> label = {fReader, "label"};
   TTreeReaderValue<Int_t> nrj = {fReader, "nrj"};
   TTreeReaderValue<long long> time = {fReader, "time"};

/////branches for coinc tree
   long long coinc_time [MAX_ITEMS];
   Int_t    coinc_label [MAX_ITEMS];
   Int_t    coinc_nrj [MAX_ITEMS];
   Int_t    coinc_mult;
   Int_t    coinc_mult_bgo;
   Int_t    coinc_mult_ge;
   Bool_t   coinc_has_ref;

/////////

   long long T0;
   long long dT;

   TTree *coinc_tree;
   TFile *outfile;
   TH2D  *hdt;

   NuballEventBuilder(TTree * /*tree*/ =0) { }
   virtual ~NuballEventBuilder() { }
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
     memset(coinc_time, 0, sizeof(double)*MAX_ITEMS);
     memset(coinc_label, 0, sizeof(int)*MAX_ITEMS);
     memset(coinc_nrj, 0, sizeof(int)*MAX_ITEMS);
     coinc_mult=0;
     coinc_mult_bgo=0;
     coinc_mult_ge=0;
     coinc_has_ref=0;
   }

   ClassDef(NuballEventBuilder,0);

};

#endif

#ifdef NuballEventBuilder_cxx
void NuballEventBuilder::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t NuballEventBuilder::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef NuballEventBuilder_cxx
