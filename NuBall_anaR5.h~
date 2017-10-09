//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Oct  3 20:09:18 2017 by ROOT version 5.34/14
// from TTree coinc/NuBall coinc tree
// found on file: out.root
//////////////////////////////////////////////////////////

#ifndef NuBall_anaR5_h
#define NuBall_anaR5_h

#define MAX_ITEMS 20
#define HIST_MIN 3
#define BGO_VETO_THRESH 0



#define BGO_GE_WINDOW_LOW 40
#define BGO_GE_WINDOW_HI  200

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1.h>
#include <TH2.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class NuBall_anaR5 : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Long64_t        GeTime[20];
   Long64_t        BGOTime[20];
   Int_t           GeLabel[20];
   Int_t           BGOLabel[20];
   Double_t        GeNrj[20];
   Double_t        BGONrj[20];
   Int_t           mult;
   Int_t           mult_bgo;
   Int_t           mult_ge;
   Bool_t          has_ref;

   // List of branches
   TBranch        *b_GeTime;   //!
   TBranch        *b_BGOTime;   //!
   TBranch        *b_GeLabel;   //!
   TBranch        *b_BGOLabel;   //!
   TBranch        *b_GeNrj;   //!
   TBranch        *b_BGONrj;   //!
   TBranch        *b_mult;   //!
   TBranch        *b_mult_bgo;   //!
   TBranch        *b_mult_ge;   //!
   TBranch        *b_has_ref;   //!

   Int_t BGOveto[MAX_ITEMS];

   TH1D* Ge_sum;
   TH1D* BGO_sum;
   TH2D* Energy;
   TH2D* Ge_CompSupSum;
   TH2D* Ge_BGOvetoSum;
   TH1D *Ge_CompSup[4];
   TH1D *Ge_BGOveto[4];
   TH1D *Ge_single[4];

   TH2D* GeBGO;

   TH2D *dt1_BGO1_Ge;
   TH2D *dt1_BGO2_Ge;
   TH2D *dt2_BGO1_Ge;
   TH2D *dt2_BGO2_Ge;
   TH2D *dt3_BGO1_Ge;
   TH2D *dt3_BGO2_Ge;
   TH2D *dt4_BGO1_Ge;
   TH2D *dt4_BGO2_Ge;
   
   TFile* OutFile;

   NuBall_anaR5(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~NuBall_anaR5() { }
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

   ClassDef(NuBall_anaR5,0);
};

#endif

#ifdef NuBall_anaR5_cxx
void NuBall_anaR5::Init(TTree *tree)
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

   fChain->SetBranchAddress("GeTime", GeTime, &b_GeTime);
   fChain->SetBranchAddress("BGOTime", BGOTime, &b_BGOTime);
   fChain->SetBranchAddress("GeLabel", GeLabel, &b_GeLabel);
   fChain->SetBranchAddress("BGOLabel", BGOLabel, &b_BGOLabel);
   fChain->SetBranchAddress("GeNrj", GeNrj, &b_GeNrj);
   fChain->SetBranchAddress("BGONrj", BGONrj, &b_BGONrj);
   fChain->SetBranchAddress("mult", &mult, &b_mult);
   fChain->SetBranchAddress("mult_bgo", &mult_bgo, &b_mult_bgo);
   fChain->SetBranchAddress("mult_ge", &mult_ge, &b_mult_ge);
   fChain->SetBranchAddress("has_ref", &has_ref, &b_has_ref);
}

Bool_t NuBall_anaR5::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef NuBall_anaR5_cxx
