//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Oct  4 16:14:50 2017 by ROOT version 6.10/02
// from TTree coinc/NuBall coinc tree
// found on file: out.root
//////////////////////////////////////////////////////////

#ifndef NuBall_ana_h
#define NuBall_ana_h

#define MAX_ITEMS 20
#define HIST_MIN 3
#define NCLOVER 5
#define GEGE_WINDOW_LOW -50
#define GEGE_WINDOW_HI   50
#define ADDBACK 0


#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TH1.h>
#include <TH2.h>
#include "Clover.h"


// Headers needed by this particular selector


class NuBall_ana : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderArray<Long64_t> GeTime = {fReader, "GeTime"};
   TTreeReaderArray<Long64_t> BGOTime = {fReader, "BGOTime"};
   TTreeReaderArray<Int_t> GeLabel = {fReader, "GeLabel"};
   TTreeReaderArray<Int_t> BGOLabel = {fReader, "BGOLabel"};
   TTreeReaderArray<Double_t> GeNrj = {fReader, "GeNrj"};
   TTreeReaderArray<Double_t> BGONrj = {fReader, "BGONrj"};
   TTreeReaderValue<Int_t> mult = {fReader, "mult"};
   TTreeReaderValue<Int_t> mult_bgo = {fReader, "mult_bgo"};
   TTreeReaderValue<Int_t> mult_ge = {fReader, "mult_ge"};
   TTreeReaderValue<Bool_t> has_ref = {fReader, "has_ref"};


   Clover *clover[NCLOVER];
   Long64_t dt;
   uint32_t Clover_hitpat;
   
   TH1D* Ge_tot;
   TH1D* BGO_tot;

   TH1D* Ge_CompSupSum;
   TH1D* Ge_BGOvetoSum;
   TH1D* Ge_sum;   

   TH1D *Ge_AddSum;
   TH1D *Ge_CompSupAddSum;

   TH2D *GeGe;
   TH2D *GeGe_CompSup;
   TH1D *GeGe_dt;
   TFile* OutFile;

   NuBall_ana(TTree * /*tree*/ =0) { }
   virtual ~NuBall_ana() { }
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

   void   processBgoGe (int thisBgoCount, int thisGeCount);
   void   BgoGeHist ( TH1D **thishist, int this_label, double energy );
   void   reset();


   ClassDef(NuBall_ana,0);

};

#endif

#ifdef NuBall_ana_cxx
void NuBall_ana::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t NuBall_ana::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef NuBall_ana_cxx
