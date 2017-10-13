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
   char histname[200];
   int i;
   
   dt = -10000;

   sprintf(outname, "anaout.root");
//   printf("%s.ana.root", fReader.GetTree()->GetTitle());
   OutFile = new TFile(outname,"recreate");
   OutFile->cd();

   Ge_sum = new TH1D("Ge_sum","Germanium Energy Sum",8000,0,4000);
   BGO_sum = new TH1D("BGO_sum","BGO Energy Sum",8000,0,4000);
   Ge_CompSupSum = new TH2D("Ge_CompSup","Ge Compton suppressed",8000,0,4000,50,0,50) ;
   Ge_BGOvetoSum = new TH2D("Ge_BGOveto","Ge vetoed by BGO",8000,0,4000,50,0,50);

   Ge_AddSum = new TH1D("Ge_AddSum", "Ge add-back sum", 8000,0,4000);
   Ge_CompSupAddSum = new TH1D("Ge_CompSupAddSum", "Ge Compton suppressed add-back sum", 8000,0,4000);

   GeGe = new TH2D("GeGe","Ge-Ge coincidence matrix - no add-back", 4000,0,2000,4000,0,2000);
   GeGe_Add = new TH2D("GeGe_Add","Ge-Ge coincidence matrix - add-back", 4000,0,2000,4000,0,2000);

   dt1_BGO1_Ge = new TH2D ("dt1_BGO1_Ge", "Det1, BGO-Ge time difference", 512, -2048, 2048, 4,0,4);
   dt1_BGO2_Ge = new TH2D ("dt1_BGO2_Ge", "Det1, BGO-Ge time difference", 512, -2048, 2048, 4,0,4);
   dt2_BGO1_Ge = new TH2D ("dt2_BGO1_Ge", "Det2, BGO-Ge time difference", 512, -2048, 2048, 4,0,4);
   dt2_BGO2_Ge = new TH2D ("dt2_BGO2_Ge", "Det2, BGO-Ge time difference", 512, -2048, 2048, 4,0,4);
   dt3_BGO1_Ge = new TH2D ("dt3_BGO1_Ge", "Det3, BGO-Ge time difference", 512, -2048, 2048, 4,0,4);
   dt3_BGO2_Ge = new TH2D ("dt3_BGO2_Ge", "Det3, BGO-Ge time difference", 512, -2048, 2048, 4,0,4);
   dt4_BGO1_Ge = new TH2D ("dt4_BGO1_Ge", "Det4, BGO-Ge time difference", 512, -2048, 2048, 4,0,4);
   dt4_BGO2_Ge = new TH2D ("dt4_BGO2_Ge", "Det4, BGO-Ge time difference", 512, -2048, 2048, 4,0,4);


   clover[0] = new Clover(0,  9,10,11,12,  5, 6);
   clover[1] = new Clover(1, 15,16,17,18, 13,14);
   clover[2] = new Clover(2, 21,22,23,24, 19,20);
   clover[3] = new Clover(3, 27,28,29,30, 25,26);
   clover[4] = new Clover(4, 33,34,35,36, 31,32);//no BGO shield!
   

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


   int i,j, det;
   int bgoCount= *mult_bgo, geCount = *mult_ge;
   reset();
   for( i=0; i<*mult_bgo; i++)  {
      if (BGONrj[i] < HIST_MIN)
         continue;
      BGO_sum->Fill(BGONrj[i]);
      for (det=0; det<5; det++) {
         if (clover[det]->ProcessBgo(BGOLabel[i],  BGONrj[i], BGOTime[i]))
            break;
      } 
   }

   for( i=0; i<*mult_ge; i++)  {
      if (GeNrj[i] < HIST_MIN)
         continue;
      Ge_sum->Fill(GeNrj[i]);
      for (det=0; det<5; det++) {
         if (clover[det]->ProcessGe(GeLabel[i], GeNrj[i], GeTime[i]))
            break;
      }
   }
      
   for (det=0; det<5; det++) {
      clover[det]->AnalyseEvent();
   }

   int det2;
   for (det=0; det<4; det++) {
      for (det2=det+1; det2<4; det2++){ //only till 4, don't include the one without BGO
         dt = clover[det]->CloverTime - clover[det2]->CloverTime;
         if (clover[det]->hasGe && clover[det2]->hasGe
         && GEGE_WINDOW_LOW < dt && GEGE_WINDOW_HI > dt) {
            if(!clover[det]->didAddBack && !clover[det2]->didAddBack) {
               GeGe->Fill(clover[det]->CloverEnergy, clover[det2]->CloverEnergy);
               GeGe->Fill(clover[det2]->CloverEnergy, clover[det]->CloverEnergy);
            }
               GeGe_Add->Fill(clover[det]->CloverEnergy, clover[det2]->CloverEnergy);
               GeGe_Add->Fill(clover[det2]->CloverEnergy, clover[det]->CloverEnergy);
         }         
      }
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
   int i;
   Ge_sum->Write();
   BGO_sum->Write();

   Ge_CompSupSum->Write();
   Ge_BGOvetoSum->Write();

   Ge_AddSum->Write();
   Ge_CompSupAddSum->Write();

   GeGe->Write();
   GeGe_Add->Write();

   dt1_BGO1_Ge->Write();
   dt1_BGO2_Ge->Write();
   dt2_BGO1_Ge->Write();
   dt2_BGO2_Ge->Write();
   dt3_BGO1_Ge->Write();
   dt3_BGO2_Ge->Write();
   dt4_BGO1_Ge->Write();
   dt4_BGO2_Ge->Write();

   for (i=0; i<5; i++) {
      clover[i]->WriteHistograms();

   }

   OutFile->Close();
}


void NuBall_ana::reset ()
{
   int i;
   for (i=0; i<5; i++) {
      clover[i]->reset();
   }
   dt = -10000;
}


























