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
   Ge_tot = new TH1D("Ge_tot","Germanium Energy Total",8000,0,4000);
   BGO_tot = new TH1D("BGO_tot","BGO Energy Total",8000,0,4000);
   Ge_CompSupSum = new TH1D("Ge_CompSup","Ge Compton suppressed",8000,0,4000);
   Ge_BGOvetoSum = new TH1D("Ge_BGOveto","Ge vetoed by BGO",8000,0,4000);

   GeGe = new TH2D("GeGe","Ge-Ge coincidence matrix", 4000,0,2000,4000,0,2000);
   GeGe_CompSup = new TH2D("GeGe_CompSup","Ge-Ge coincidence matrix, compton suppressed", 4000,0,2000,4000,0,2000);
   GeGe_dt = new TH1D("GeGe_dt","Ge-Ge dt", 500,-2000,2000);

   if (ADDBACK) {
      printf("AddBack is ON.\n");
   } else {
      printf("AddBack is OFF.\n");
   }   
   
   clover[0] = new Clover(0,  9,10,11,12,  5, 6, ADDBACK);
   clover[1] = new Clover(1, 15,16,17,18, 13,14, ADDBACK);
   clover[2] = new Clover(2, 21,22,23,24, 19,20, ADDBACK);
   clover[3] = new Clover(3, 27,28,29,30, 25,26, ADDBACK);
   clover[4] = new Clover(4, 33,34,35,36, 31,32, ADDBACK);//no BGO shield!
   

   printf("Init done.\nSorting...\n");
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


   int i,j, det, det2;
   int bgoCount= *mult_bgo, geCount = *mult_ge;
   reset();
///Fill the Clover objects with event data
   for( i=0; i<*mult_bgo; i++)  {
      if (BGONrj[i] < HIST_MIN)
         continue;
      BGO_tot->Fill(BGONrj[i]);
      for (det=0; det<NCLOVER; det++) {
         if (clover[det]->ProcessBgo(BGOLabel[i],  BGONrj[i], BGOTime[i]))
            break;
      } 
   }
   for( i=0; i<*mult_ge; i++)  {
      if (GeNrj[i] < HIST_MIN)
         continue;
      Ge_tot->Fill(GeNrj[i]);
      for (det=0; det<NCLOVER; det++) {
         if (clover[det]->ProcessGe(GeLabel[i], GeNrj[i], GeTime[i]))
            break;
      }
   }
///Do compton suppression (and addback if enabled) in the Clover objects
   int tryv;
   for (det=0; det<NCLOVER; det++) {
      tryv = clover[det]->AnalyseEvent();
      if(tryv) {
         Clover_hitpat += 1 << det;
      }
   }
///From here on, use Clover[i].CloverEnergy and CloverTime as energy and time for each Clover detector
///Addback will have been done, if enabled. BGO veto can be checked from Clover[i].hasVeto
///Fill coincidence matrices here.
   for (det=0; det<NCLOVER; det++) {
      if ( clover[det]->CloverEnergy > 0) {
         for (det2=det+1; det2<NCLOVER; det2++) {
            if ( clover[det2]->CloverEnergy > 0) {
               dt = clover[det]->CloverTime - clover[det2]->CloverTime;
               GeGe_dt->Fill(dt);
               if (GEGE_WINDOW_LOW < dt && GEGE_WINDOW_HI > dt) {
                  GeGe->Fill(clover[det]->CloverEnergy, clover[det2]->CloverEnergy);
                  GeGe->Fill(clover[det2]->CloverEnergy, clover[det]->CloverEnergy);
                  if (!clover[det]->hasVeto && !clover[det2]->hasVeto) {
                     GeGe_CompSup->Fill(clover[det]->CloverEnergy, clover[det2]->CloverEnergy);
                     GeGe_CompSup->Fill(clover[det2]->CloverEnergy, clover[det]->CloverEnergy);
                  }
               }
            }
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
   printf("Writing histograms...\n");
   int det;

   Ge_tot->Write();
   BGO_tot->Write();
   
   for (det=0; det<NCLOVER; det++) {
      Ge_sum->Add(&clover[det]->hGe,1);
      Ge_CompSupSum->Add(&clover[det]->hGe_CompSup,1);
      Ge_BGOvetoSum->Add(&clover[det]->hGe_BgoVeto,1);
      clover[det]->WriteHistograms(OutFile);
   }
   Ge_sum->Write();
   Ge_CompSupSum->Write();
   Ge_BGOvetoSum->Write();
   
   GeGe->Write();
   GeGe_CompSup->Write();
   GeGe_dt->Write();

   OutFile->Close();
      printf("Done.\n");
}


void NuBall_ana::reset ()
{
   int i;
   for (i=0; i<NCLOVER; i++) {
      clover[i]->reset();
   }
   dt = -10000;
   Clover_hitpat = 0;
}


























