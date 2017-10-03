#define NuBall_anaR5_cxx
// The class definition in NuBall_anaR5.h has been generated automatically
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
// Root > T->Process("NuBall_anaR5.C")
// Root > T->Process("NuBall_anaR5.C","some options")
// Root > T->Process("NuBall_anaR5.C+")
//

#include "NuBall_anaR5.h"
#include <TH2.h>
#include <TStyle.h>


void NuBall_anaR5::Begin(TTree * /*tree*/)
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

   Ge_sum = new TH1D("Ge_sum","Germanium Energy Sum",8000,0,4000);
   BGO_sum = new TH1D("BGO_sum","BGO Energy Sum",8000,0,4000);
   Energy = new TH2D("Energy","Energy Singles",8000,0,4000,50,0,50);
   Ge_CompSup = new TH2D("Ge_CompSup","Ge Compton suppressed",8000,0,4000,50,0,50) ;
   Ge_BGOveto = new TH2D("Ge_BGOveto","Ge vetoed by BGO",8000,0,4000,50,0,50);
   GeBGO = new TH2D("GeBGO","Ge-BGO coincidenc matrix (only Ge-BGO from one detector)",8000,0,4000, 8000,0,4000);


   dt1_BGO1_Ge = new TH2D ("dt1_BGO1_Ge", "Det1, BGO-Ge time difference", 5000,-2500,2500, 4,0,4);
   dt1_BGO2_Ge = new TH2D ("dt1_BGO2_Ge", "Det1, BGO-Ge time difference", 5000,-2500,2500, 4,0,4);
   dt2_BGO1_Ge = new TH2D ("dt2_BGO1_Ge", "Det1, BGO-Ge time difference", 5000,-2500,2500, 4,0,4);
   dt2_BGO2_Ge = new TH2D ("dt2_BGO2_Ge", "Det1, BGO-Ge time difference", 5000,-2500,2500, 4,0,4);
   dt3_BGO1_Ge = new TH2D ("dt3_BGO1_Ge", "Det1, BGO-Ge time difference", 5000,-2500,2500, 4,0,4);
   dt3_BGO2_Ge = new TH2D ("dt3_BGO2_Ge", "Det1, BGO-Ge time difference", 5000,-2500,2500, 4,0,4);
   dt4_BGO1_Ge = new TH2D ("dt4_BGO1_Ge", "Det1, BGO-Ge time difference", 5000,-2500,2500, 4,0,4);
   dt4_BGO2_Ge = new TH2D ("dt4_BGO2_Ge", "Det1, BGO-Ge time difference", 5000,-2500,2500, 4,0,4);

   

   printf("starting...\n");
}

void NuBall_anaR5::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t NuBall_anaR5::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either NuBall_anaR5::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   NuBall_anaR5::GetEntry(entry);

   int i,j;
   reset();
   for( i=0; i<mult_bgo; i++)  {
      if (BGONrj[i] < HIST_MIN)
         continue;
      BGO_sum->Fill(BGONrj[i]);
      Energy->Fill(BGONrj[i],BGOLabel[i]);
      for (j=0; j<mult_ge; j++) {
         if (GeNrj[i] < HIST_MIN)
            continue;
         processBgoGe(i,j);
      }      
   }

   for( i=0; i<mult_ge; i++)  {
      if (GeNrj[i] < HIST_MIN)
         continue;
      Ge_sum->Fill(GeNrj[i]);
      Energy->Fill(GeNrj[i],GeLabel[i]);
      if (BGOveto[i] == 0) {
         Ge_CompSup->Fill(GeNrj[i], GeLabel[i]);
      } else {
         Ge_BGOveto->Fill(GeNrj[i], GeLabel[i]);
      }
   }

   return kTRUE;
}

void NuBall_anaR5::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void NuBall_anaR5::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
   Ge_sum->Write();
   BGO_sum->Write();
   Energy->Write();
   Ge_CompSup->Write();
   Ge_BGOveto->Write();
   GeBGO->Write();

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

void NuBall_anaR5::processBgoGe (int thisBgoCount, int thisGeCount)
{
   long long dt;
   dt = GeTime[thisGeCount] - BGOTime[thisBgoCount];
   if (BGOLabel[thisBgoCount] == 6) {
      if (GeLabel[thisGeCount] == 10 ){
          dt1_BGO1_Ge->Fill( dt, 0);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 11 ) {
          dt1_BGO1_Ge->Fill( dt, 1);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 12 ) {
          dt1_BGO1_Ge->Fill( dt, 2);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 13 ) {
          dt1_BGO1_Ge->Fill( dt, 3);
          goto increment;
      }
      return;
   }
   if (BGOLabel[thisBgoCount] == 7) { 
      if (GeLabel[thisGeCount] == 10 ) {
          dt1_BGO2_Ge->Fill( dt, 0);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 11 ) {
          dt1_BGO2_Ge->Fill( dt, 1);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 12 ) {
          dt1_BGO2_Ge->Fill( dt, 2);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 13 ) {
          dt1_BGO2_Ge->Fill( dt, 3);
          goto increment;
      }
      return;
   }

   if (BGOLabel[thisBgoCount] == 14) {
      if (GeLabel[thisGeCount] == 16 ) {
          dt2_BGO1_Ge->Fill( dt, 0);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 17 ) {
          dt2_BGO1_Ge->Fill( dt, 1);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 18 ) {
          dt2_BGO1_Ge->Fill( dt, 2);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 19 ) {
          dt2_BGO1_Ge->Fill( dt, 3);
          goto increment;
      }
      return;
   }
   if (BGOLabel[thisBgoCount] == 15) {
      if (GeLabel[thisGeCount] == 16 ) {
          dt2_BGO2_Ge->Fill( dt, 0);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 17 ) {
          dt2_BGO2_Ge->Fill( dt, 1);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 18 ) {
          dt2_BGO2_Ge->Fill( dt, 2);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 19 ) {
          dt2_BGO2_Ge->Fill( dt, 3);
          goto increment;
      }
      return;
   }

   if (BGOLabel[thisBgoCount] == 20) {
      if (GeLabel[thisGeCount] == 22 ) {
          dt3_BGO1_Ge->Fill( dt, 0);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 23 ) {
          dt3_BGO1_Ge->Fill( dt, 1);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 24 ) {
          dt3_BGO1_Ge->Fill( dt, 2);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 25 ) {
          dt3_BGO1_Ge->Fill( dt, 3);
          goto increment;
      }
      return;
   }
   if (BGOLabel[thisBgoCount] == 21) {
      if (GeLabel[thisGeCount] == 22 ) {
          dt3_BGO2_Ge->Fill( dt, 0);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 23 ) {
          dt3_BGO2_Ge->Fill( dt, 1);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 24 ) {
          dt3_BGO2_Ge->Fill( dt, 2);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 25 ) {
          dt3_BGO2_Ge->Fill( dt, 3);
          goto increment;
      }
      return;
   }

   if (BGOLabel[thisBgoCount] == 26) {
      if (GeLabel[thisGeCount] == 28 ) {
          dt4_BGO1_Ge->Fill( dt, 0);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 29 ) {
          dt4_BGO1_Ge->Fill( dt, 1);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 30 ) {
          dt4_BGO1_Ge->Fill( dt, 2);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 31 ) {
          dt4_BGO1_Ge->Fill( dt, 3);
          goto increment;
      }
      return;
   }
   if (BGOLabel[thisBgoCount] == 27) {
      if (GeLabel[thisGeCount] == 28 ) {
          dt4_BGO2_Ge->Fill( dt, 0);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 29 ) {
          dt4_BGO2_Ge->Fill( dt, 1);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 30 ) {
          dt4_BGO2_Ge->Fill( dt, 2);
          goto increment;
      }
      else if (GeLabel[thisGeCount] == 31 ) {
          dt4_BGO2_Ge->Fill( dt, 3);
          goto increment;
      }
      return;
   }
increment:
   {
      BGOveto[thisGeCount] = 1;
      GeBGO->Fill(GeNrj[thisGeCount], BGONrj[thisBgoCount]);
   }
}

void NuBall_anaR5::reset ()
{
   memset(BGOveto, 0, sizeof(Int_t)*MAX_ITEMS);  
}












