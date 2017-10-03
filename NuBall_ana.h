//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Oct  3 17:24:04 2017 by ROOT version 6.10/02
// from TTree coinc/NuBall coinc tree
// found on file: 60Co_first_mini_nuball_0001.new.sample.root
//////////////////////////////////////////////////////////

#ifndef NuBall_ana_h
#define NuBall_ana_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TH1.h>
#include <TH2.h>

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
   TTreeReaderArray<Float_t> GeNrj = {fReader, "GeNrj"};
   TTreeReaderArray<Float_t> BGONrj = {fReader, "BGONrj"};
   TTreeReaderValue<Int_t> mult = {fReader, "mult"};
   TTreeReaderValue<Int_t> mult_bgo = {fReader, "mult_bgo"};
   TTreeReaderValue<Int_t> mult_ge = {fReader, "mult_ge"};
   TTreeReaderValue<Bool_t> has_ref = {fReader, "has_ref"};

   TH1F* Ge_sum;
   TH1F* BGO_sum;
   TH2F* Energy;

   TH2F *dt1_BGO1_Ge;
   TH2F *dt1_BGO2_Ge;
   TH2F *dt2_BGO1_Ge;
   TH2F *dt2_BGO2_Ge;
   TH2F *dt3_BGO1_Ge;
   TH2F *dt3_BGO2_Ge;
   TH2F *dt4_BGO1_Ge;
   TH2F *dt4_BGO2_Ge;

   
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

   Bool_t IsBGO (int label);
   void   dtBgoGe (int thisBgoLabel, float thisBgoEnergy, int thisGeLabel, float thisGeEnergy);

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

Boot_t NuBall_ana::IsBGO(int this_label)
{
   if ( this_label == 6  ||
        this_label == 7  ||
        this_label == 14 ||
        this_label == 15 ||
        this_label == 20 ||
        this_label == 21 ||
        this_label == 26 ||
        this_label == 27 ||
   ) {
      return 1;
   }
   return 0;
}



#endif // #ifdef NuBall_ana_cxx
