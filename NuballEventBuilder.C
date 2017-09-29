#define NuballEventBuilder_cxx
// The class definition in NuballEventBuilder.h has been generated automatically
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
// root> T->Process("NuballEventBuilder.C")
// root> T->Process("NuballEventBuilder.C","some options")
// root> T->Process("NuballEventBuilder.C+")
//


#include "NuballEventBuilder.h"
#include <TStyle.h>

void NuballEventBuilder::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).
   char leafs[300];
   TString option = GetOption();
//   reset_coinc(coinc);
   dT = 0;
   T0 = 0;

//   sprintf(leafs, "fmult/s:gmult/s:dgs[%d]/F:fatima[%d]/F", MaxTreeArraySize, MaxTreeArraySize);
//    spec_tree->Branch("spec", &spec_event, leafs);

   reset_coinc();
   coinc_tree = new TTree("coinc", "NuBall coinc tree");
   sprintf(leafs, "time/D[%d]", MAX_ITEMS);
   coinc_tree->Branch("time", coinc_time, leafs);
   sprintf(leafs, "label/I[%d]", MAX_ITEMS);
   coinc_tree->Branch("label", coinc_label, leafs);
   sprintf(leafs, "nrj/I[%d]", MAX_ITEMS);
   coinc_tree->Branch("nrj", coinc_nrj, leafs);

   coinc_tree->Branch("mult", &coinc_mult, "mult/I");
   coinc_tree->Branch("mult_bgo", &coinc_mult_bgo, "mult_bgo/I");
   coinc_tree->Branch("mult_ge", &coinc_mult_ge, "mult_ge/I");
   coinc_tree->Branch("has_ref", &coinc_has_ref, "has_ref/O");

   hdt = new TH2D("hdt", "time diff, ref 28", 32000,0,32000, 100,0,100);
}

void NuballEventBuilder::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t NuballEventBuilder::Process(Long64_t entry)
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

/*   if (coinc_mult == 0) {
      T0 = *time;
      coinc_time[0] = *time;
      coinc_nrj[0] = *nrj;
      coinc_label[0] = *label;
      coinc_mult++;
   } else {
     dT = *time - T0;
     if (dT < 0) {
        printf("WARNING: event not time sorted (entry %ld, dT = %ld - %ld = %ld\n", entry, *time, T0, dT);
     } else if (dT < COINC_WIDTH) {
        coinc_time[coinc_mult] = *time;
        coinc_nrj[coinc_mult] = *nrj;
        coinc_label[coinc_mult] = *label;
        coinc_mult++;
     } else {
        //write to tree here

        reset_coinc();
        T0 = *time;
        coinc_time[0] = *time;
        coinc_nrj[0] = *nrj;
        coinc_label[0] = *label;
        coinc_mult++;
     }
   }
*/
   printf("entry-%ld -- label: %d, nrj: %d, time: %ld\n", entry, *label, *nrj, *time);

   return kTRUE;
}

void NuballEventBuilder::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void NuballEventBuilder::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   outfile = new TFile("out.root", "recreate");
   outfile->cd();
   coinc_tree->Write();
   outfile->Close();

}
