//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Sep 28 16:56:30 2017 by ROOT version 5.34/14
// from TTree coinc/NuBall coinc tree
// found on file: out.root
//////////////////////////////////////////////////////////

#ifndef NuBall_ana_h
#define NuBall_ana_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH2.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class NuBall_ana : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Long64_t        time[40];
   Int_t           label[40];
   Int_t           nrj[40];
   Int_t           mult;
   Int_t           mult_bgo;
   Int_t           mult_ge;
   Bool_t          has_ref;

   // List of branches
   TBranch        *b_time;   //!
   TBranch        *b_label;   //!
   TBranch        *b_nrj;   //!
   TBranch        *b_mult;   //!
   TBranch        *b_mult_bgo;   //!
   TBranch        *b_mult_ge;   //!
   TBranch        *b_has_ref;   //!

///Histograms
   TH2D *htd;


   NuBall_ana(TTree * /*tree*/ =0) : fChain(0) { }
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

   ClassDef(NuBall_ana,0);
};

#endif

#ifdef NuBall_ana_cxx
void NuBall_ana::Init(TTree *tree)
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

   fChain->SetBranchAddress("time", time, &b_time);
   fChain->SetBranchAddress("label", label, &b_label);
   fChain->SetBranchAddress("nrj", nrj, &b_nrj);
   fChain->SetBranchAddress("mult", &mult, &b_mult);
   fChain->SetBranchAddress("mult_bgo", &mult_bgo, &b_mult_bgo);
   fChain->SetBranchAddress("mult_ge", &mult_ge, &b_mult_ge);
   fChain->SetBranchAddress("has_ref", &has_ref, &b_has_ref);
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
