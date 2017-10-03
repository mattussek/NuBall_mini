#define NuBall_ana_cxx
// The class definition in NuBall_ana.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("NuBall_ana.C")
// root> T->Process("NuBall_ana.C","some options")
// root> T->Process("NuBall_ana.C+")
//


#include "NuBall_ana.h"
#include <TH2.h>
#include <TStyle.h>

void NuBall_ana::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   char outname[200];
   sprintf(outname, "anaout.root");
//   printf("%s.ana.root", fReader.GetTree()->GetTitle());
   OutFile = new TFile(outname,"recreate");
   OutFile->cd();

   Ge_sum = new TH1F("Ge_sum","Germanium Energy Sum",4000,0,4000);
   BGO_sum = new TH1F("BGO_sum","BGO Energy Sum",4000,0,4000);
   Energy = new TH2F("Energy","Energy Singles",4000,0,4000,50,0,50);

   dt1_BGO1_Ge = new TH2F ("dt1_BGO1_Ge", "Det1, BGO-Ge time difference", 5000,-2500,2500, 4,0,4);
   dt1_BGO2_Ge = new TH2F ("dt1_BGO2_Ge", "Det1, BGO-Ge time difference", 5000,-2500,2500, 4,0,4);
   dt2_BGO1_Ge = new TH2F ("dt2_BGO1_Ge", "Det1, BGO-Ge time difference", 5000,-2500,2500, 4,0,4);
   dt2_BGO2_Ge = new TH2F ("dt2_BGO2_Ge", "Det1, BGO-Ge time difference", 5000,-2500,2500, 4,0,4);
   dt3_BGO1_Ge = new TH2F ("dt3_BGO1_Ge", "Det1, BGO-Ge time difference", 5000,-2500,2500, 4,0,4);
   dt3_BGO2_Ge = new TH2F ("dt3_BGO2_Ge", "Det1, BGO-Ge time difference", 5000,-2500,2500, 4,0,4);
   dt4_BGO1_Ge = new TH2F ("dt4_BGO1_Ge", "Det1, BGO-Ge time difference", 5000,-2500,2500, 4,0,4);
   dt4_BGO2_Ge = new TH2F ("dt4_BGO2_Ge", "Det1, BGO-Ge time difference", 5000,-2500,2500, 4,0,4);

   printf("starting...\n");
}

void NuBall_ana::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t NuBall_ana::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   fReader.SetEntry(entry);
   int i,j;
   for( i=0; i<*mult_ge; i++)  {
      Ge_sum->Fill(GeNrj[i]);
      Energy->Fill(GeNrj[i],GeLabel[i]);
   }
   for( i=0; i<*mult_bgo; i++)  {
      BGO_sum->Fill(BGONrj[i]);
      Energy->Fill(BGONrj[i],BGOLabel[i]);
      
   }
   return kTRUE;
}

void NuBall_ana::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void NuBall_ana::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
   Ge_sum->Write();
   BGO_sum->Write();
   Energy->Write();

   dt1_BGO1_Ge->Write();
   dt1_BGO2_Ge->Write();
   dt2_BGO1_Ge->Write();
   dt2_BGO2_Ge->Write();
   dt3_BGO1_Ge->Write();
   dt3_BGO2_Ge->Write();
   dt4_BGO1_Ge->Write();
   dt4_BGO2_Ge->Write();

   OutFile->Close();
}

void NuBall_ana::incDtBgoGe (int thisBgoLabel, float thisBgoTime, int thisGeLabel, float thisGeTime)
{
   long long dt;
   dt = thisGeTime - thisBgoTime;
   if (thisBgoLabel == 6) {
      if (thisGeLabel == 10 )
          dt1_BGO1_Ge->Fill( dt, 0);
      if (thisGeLabel == 11 )
          dt1_BGO1_Ge->Fill( dt, 1);
      if (thisGeLabel == 12 )
          dt1_BGO1_Ge->Fill( dt, 2);
      if (thisGeLabel == 13 )
          dt1_BGO1_Ge->Fill( dt, 3);
   }
   if (thisBgoLabel == 7) {
      if (thisGeLabel == 10 )
          dt1_BGO2_Ge->Fill( dt, 0);
      if (thisGeLabel == 11 )
          dt1_BGO2_Ge->Fill( dt, 1);
      if (thisGeLabel == 12 )
          dt1_BGO2_Ge->Fill( dt, 2);
      if (thisGeLabel == 13 )
          dt1_BGO2_Ge->Fill( dt, 3);
   }

   if (thisBgoLabel == 14) {
      if (thisGeLabel == 16 )
          dt2_BGO1_Ge->Fill( dt, 0);
      if (thisGeLabel == 17 )
          dt2_BGO1_Ge->Fill( dt, 1);
      if (thisGeLabel == 18 )
          dt2_BGO1_Ge->Fill( dt, 2);
      if (thisGeLabel == 19 )
          dt2_BGO1_Ge->Fill( dt, 3);
   }
   if (thisBgoLabel == 15) {
      if (thisGeLabel == 16 )
          dt2_BGO2_Ge->Fill( dt, 0);
      if (thisGeLabel == 17 )
          dt2_BGO2_Ge->Fill( dt, 1);
      if (thisGeLabel == 18 )
          dt2_BGO2_Ge->Fill( dt, 2);
      if (thisGeLabel == 19 )
          dt2_BGO2_Ge->Fill( dt, 3);
   }

   if (thisBgoLabel == 20) {
      if (thisGeLabel == 22 )
          dt3_BGO1_Ge->Fill( dt, 0);
      if (thisGeLabel == 23 )
          dt3_BGO1_Ge->Fill( dt, 1);
      if (thisGeLabel == 24 )
          dt3_BGO1_Ge->Fill( dt, 2);
      if (thisGeLabel == 25 )
          dt3_BGO1_Ge->Fill( dt, 3);
   }
   if (thisBgoLabel == 21) {
      if (thisGeLabel == 22 )
          dt3_BGO2_Ge->Fill( dt, 0);
      if (thisGeLabel == 23 )
          dt3_BGO2_Ge->Fill( dt, 1);
      if (thisGeLabel == 24 )
          dt3_BGO2_Ge->Fill( dt, 2);
      if (thisGeLabel == 25 )
          dt3_BGO2_Ge->Fill( dt, 3);
   }

   if (thisBgoLabel == 26) {
      if (thisGeLabel == 28 )
          dt4_BGO1_Ge->Fill( dt, 0);
      if (thisGeLabel == 29 )
          dt4_BGO1_Ge->Fill( dt, 1);
      if (thisGeLabel == 30 )
          dt4_BGO1_Ge->Fill( dt, 2);
      if (thisGeLabel == 31 )
          dt4_BGO1_Ge->Fill( dt, 3);
   }
   if (thisBgoLabel == 27) {
      if (thisGeLabel == 28 )
          dt4_BGO2_Ge->Fill( dt, 0);
      if (thisGeLabel == 29 )
          dt4_BGO2_Ge->Fill( dt, 1);
      if (thisGeLabel == 30 )
          dt4_BGO2_Ge->Fill( dt, 2);
      if (thisGeLabel == 31 )
          dt4_BGO2_Ge->Fill( dt, 3);
   }

}
