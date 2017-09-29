#define NuBall_eventbuilderR5_cxx
// The class definition in NuBall_eventbuilderR5.h has been generated automatically
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
// Root > T->Process("NuBall_eventbuilderR5.C")
// Root > T->Process("NuBall_eventbuilderR5.C","some options")
// Root > T->Process("NuBall_eventbuilderR5.C+")
//

#include "NuBall_eventbuilderR5.h"
#include <TH2.h>
#include <TStyle.h>


void NuBall_eventbuilderR5::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   sprintf(CALIB_FILE, "miniNuBall.cal");
   char leafs[300];
   TString option = GetOption();
//   reset_coinc(coinc);
   dT = 0;
   T0 = 0;

//   sprintf(leafs, "fmult/s:gmult/s:dgs[%d]/F:fatima[%d]/F", MaxTreeArraySize, MaxTreeArraySize);
//    spec_tree->Branch("spec", &spec_event, leafs);

   reset_coinc();

   outfile = new TFile("out.root", "recreate");
   outfile->cd();
   coinc_tree = new TTree("coinc", "NuBall coinc tree");
   sprintf(leafs, "time[%d]/L", MAX_ITEMS);
   coinc_tree->Branch("time", &coinc_time, leafs);
   sprintf(leafs, "label[%d]/I", MAX_ITEMS);
   coinc_tree->Branch("label", &coinc_label, leafs);
   sprintf(leafs, "nrj[%d]/I", MAX_ITEMS);
   coinc_tree->Branch("nrj", &coinc_nrj, leafs);

   coinc_tree->Branch("mult", &coinc_mult, "mult/I");
   coinc_tree->Branch("mult_bgo", &coinc_mult_bgo, "mult_bgo/I");
   coinc_tree->Branch("mult_ge", &coinc_mult_ge, "mult_ge/I");
   coinc_tree->Branch("has_ref", &coinc_has_ref, "has_ref/O");

   hdt = new TH2D("hdt", "time diff, ref 28", 2048,-8192,8192, 100,0,100);
   hgg =  new TH2D("hgg", "gamma-gamma coinc mat", 8192,0,8192,8192,0,8192);
   hg =  new TH2D("hg", "gamma single matrices", 8192,0,8192,100,0,100);
   nCoincidences = 0;
   coinc_entry = 0;

   readCalibration();

   printf("starting...\n");

}

void NuBall_eventbuilderR5::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t NuBall_eventbuilderR5::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either NuBall_eventbuilderR5::GetEntry() or TBranch::GetEntry()
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
   NuBall_eventbuilderR5::GetEntry(entry);

   if (label > MAX_LABEL) {
      printf("WARNING: Found label that exceeds limit (label%d) -> skipping event\n", label);
      return kTRUE;
   }

   if (coinc_mult == 0) {
      T0 = time;
      coinc_time[0] = time;
      coinc_nrj[0] = nrjCal(label, nrj);
      coinc_label[0] = label;
      coinc_mult++;

   } else {
     dT = time - T0;
     if (dT < 0) {
      printf("WARNING: event not time sorted - skipping - (entry %lld, dT = %lld - %lld = %lld\n", entry, time, T0, dT);
      return kTRUE;
     }

     if (dT < COINC_WIDTH) {
        coinc_time[coinc_mult] = time;
        coinc_nrj[coinc_mult] = nrjCal(label, nrj);
        coinc_label[coinc_mult] = label;
        coinc_mult++;
     } else {
        //write to tree here
        if (coinc_mult > 1)
            nCoincidences++;
//        printCurrBranch();
        coinc_tree->Fill();
        coinc_entry++;
        fillHistograms();
//        printCoinc();
        reset_coinc();

        T0 = time;
        coinc_time[0] = time;
        coinc_nrj[0] = nrjCal(label, nrj);
        coinc_label[0] = label;
        coinc_mult++;
     }
   }

   return kTRUE;
}

void NuBall_eventbuilderR5::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void NuBall_eventbuilderR5::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
   printf("\n");
   coinc_tree->Write();
   hdt->Write();
   hg->Write();
   hgg->Write();

   outfile->Close();
   printf("Finished.\nFound %lld coincidences.\nWrote %lld entries to the new TTree.\n", nCoincidences, coinc_entry);
}


void NuBall_eventbuilderR5::printCoinc()
{
   if (coinc_mult < 2)
      return;
   int i = 0;
   printf("+++Found coincidence %lld:\n", nCoincidences);
   for (i=0; i<coinc_mult; i++) {
      printf("label%d - energy%d - time %lld\n", coinc_label[i], coinc_nrj[i], coinc_time[i]);
   }
   printf("**multiplicities: tot%d, bgo%d, ge%d\n", coinc_mult, coinc_mult_bgo, coinc_mult_ge);
   printf("//\n");
   return;
}

void NuBall_eventbuilderR5::printCurrBranch()
{
   int i = 0;
   printf("+++Current Branch content %lld:\n", coinc_entry);
   for (i=0; i<coinc_mult; i++) {
      printf("label%d - energy%d - time %lld\n", coinc_label[i], coinc_nrj[i], coinc_time[i]);
   }
   printf("**multiplicities: tot%d, bgo%d, ge%d\n", coinc_mult, coinc_mult_bgo, coinc_mult_ge);
   printf("//\n");
   return;
}

void NuBall_eventbuilderR5::fillHistograms()
{
   int i = 0;
   int j = 0;
   int ref_pos = -1;
   long long ref_time = 0;
   bool isBGO[coinc_mult];

   for (i=0; i<coinc_mult; i++) {
      hg->Fill(coinc_nrj[i], coinc_label[i]);
      //check for the reference detector
      if (coinc_label[i] == T_REF) {
         ref_pos = coinc_label[i];
         ref_time = coinc_time[i];
         coinc_has_ref = 1;
      }
      //check detector type
      isBGO[i] = 0;
      if (
         coinc_label[i] == 6 || // these are the BGOs in the mini setup
         coinc_label[i] == 7 ||
         coinc_label[i] == 14 ||
         coinc_label[i] == 15 ||
         coinc_label[i] == 20 ||
         coinc_label[i] == 21 ||
         coinc_label[i] == 26 ||
         coinc_label[i] == 27 
      ) {
         coinc_mult_bgo++;
         isBGO[i] = 1;
      } else {
         coinc_mult_ge++;
      }

   }
   for (i=0; i<coinc_mult; i++) {
      //increment gg matrix
      if (isBGO[i])
         continue;
      for (j=i+1; j<coinc_mult; j++) {
         if (isBGO[j])
            continue;
         if (coinc_label[i] != coinc_label[j]) {
            hgg->Fill(coinc_nrj[i], coinc_nrj[j]);
            hgg->Fill(coinc_nrj[j], coinc_nrj[i]);
         }
      }
   }
   
   //make dt matrix rel to reference detector
   if (ref_pos > -1) {
      for (i=0; i<coinc_mult; i++) {
         if (coinc_label[i] == ref_pos)
            continue;
         hdt->Fill((Long64_t) (coinc_time[i] - ref_time), coinc_label[i]);
      }
   }
   
   return;
}


void NuBall_eventbuilderR5::readCalibration()
{
   	int this_label=0;
        int i;
	float params[2];


	int lnr=0;
	//read the file and fill the arrays
	FILE *calfile = fopen( CALIB_FILE , "r") ;
	printf("\nReading calibration parameters from %s...", CALIB_FILE) ;

        //initialising calParameters to zero.
        for (i=0; i<MAX_LABEL; i++) {
              calParameters[i][0] = 0;
              calParameters[i][1] = 0;
        }

	if(calfile != NULL) //Load the values if file exists
	{
		char line[1000];

                //reading the file line by line, skipping comments (#) and empty lines
                //but exiting if format is otherwise wrong.
		while (fgets(line, sizeof line, calfile))
		{
			lnr++;
			if (*line == '#' || *line == '\n') 
			{
				continue; // ignore comment line (TODO add empty line)
			}
			if (sscanf(line, "%d\t%f\t%f",&this_label,&params[0], &params[1]) == 3) {
				if(label < MAX_LABEL)
				{
				   calParameters[this_label][0] = params[0];
                                   calParameters[this_label][1] = params[1];
				}
			}else{
				printf("FAILED to parse line in cal file %s:\n%s\nEXITING\n", CALIB_FILE, line);
				exit (EXIT_FAILURE);
			}
		}
		printf("OK\n");
	        printf("Calibration parameters read from file %s:\n", CALIB_FILE);
                for (i=0; i<MAX_LABEL; i++) {
                   printf("%02d  %3.6f  %3.6f\n", i, calParameters[i][0], calParameters[i][1]);
                }
                printf("\n");
                return;
        }
	printf("FAILED\nError: File not found: %s\nNothing will be calibrated.\n\n", CALIB_FILE) ;
        return;
}


float  NuBall_eventbuilderR5::nrjCal(int this_label, int this_nrj)
{
	double ene;
//	double random = ((double) rand()) / ((double) RAND_MAX *1);

	ene = calParameters[this_label][0] + calParameters[this_label][1]*this_nrj;
        return (float) ene;
}




















