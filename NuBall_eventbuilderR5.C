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

   sprintf(leafs, "GeTime[%d]/L", MAX_ITEMS);
   coinc_tree->Branch("GeTime", &coinc_GeTime, leafs);
   sprintf(leafs, "BGOTime[%d]/L", MAX_ITEMS);
   coinc_tree->Branch("BGOTime", &coinc_BGOTime, leafs);
   sprintf(leafs, "GeLabel[%d]/I", MAX_ITEMS);
   coinc_tree->Branch("GeLabel", &coinc_GeLabel, leafs);
   sprintf(leafs, "BGOLabel[%d]/I", MAX_ITEMS);
   coinc_tree->Branch("BGOLabel", &coinc_BGOLabel, leafs);
   sprintf(leafs, "GeNrj[%d]/I", MAX_ITEMS);
   coinc_tree->Branch("GeNrj", &coinc_GeNrj, leafs);
   sprintf(leafs, "BGONrj[%d]/I", MAX_ITEMS);
   coinc_tree->Branch("BGONrj", &coinc_BGONrj, leafs);

   coinc_tree->Branch("mult", &coinc_mult, "mult/I");
   coinc_tree->Branch("mult_bgo", &coinc_BGOmult, "mult_bgo/I");
   coinc_tree->Branch("mult_ge", &coinc_Gemult, "mult_ge/I");
   coinc_tree->Branch("has_ref", &coinc_has_ref, "has_ref/O");

   hdt = new TH2D("hdt", "time diff, ref 28", 2048,-8192,8192, 100,0,100);
   hGeGe =  new TH2D("hGeGe", "gamma-gamma coinc mat", 8192,0,8192,8192,0,8192);
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
        //don't care for now if a Ge or BGO starts an event.
        T0 = time;
        if (isBGO(label)) {
           coinc_BGOTime[0] = time;
           coinc_BGONrj[0] = nrjCal(label, nrj);
           coinc_BGOLabel[0] = label;
           coinc_BGOmult++;
        } else {
           coinc_GeTime[0] = time;
           coinc_GeNrj[0] = nrjCal(label, nrj);
           coinc_GeLabel[0] = label;
           coinc_Gemult++;
        }
        coinc_mult++;
   } else {
     dT = time - T0;
     if (dT < 0) {
      printf("WARNING: event not time sorted - skipping - (entry %lld, dT = %lld - %lld = %lld\n", entry, time, T0, dT);
      return kTRUE;
     }

     if (dT < COINC_WIDTH) {
        if (isBGO(label)) {
           coinc_BGOTime[coinc_BGOmult] = time;
           coinc_BGONrj[coinc_BGOmult] = nrjCal(label, nrj);
           coinc_BGOLabel[coinc_BGOmult] = label;
           coinc_BGOmult++;
        } else {
           coinc_GeTime[coinc_Gemult] = time;
           coinc_GeNrj[coinc_Gemult] = nrjCal(label, nrj);
           coinc_GeLabel[coinc_Gemult] = label;
           coinc_Gemult++;
        }

        coinc_mult++;
        if (coinc_BGOmult >= MAX_ITEMS || coinc_Gemult >= MAX_ITEMS) {
           printf("WARNING: Event exceeds limit of data items per event. Ge:%d, BGO:%d\nstarting next event.\n", coinc_Gemult, coinc_BGOmult);
           reset_coinc();
           return kTRUE;
        }
     } else {
        //write to tree here
        if (coinc_Gemult > 1)
            nCoincidences++;
//        printCurrBranch();
        coinc_tree->Fill();
        coinc_entry++;
        fillHistograms();
//        printCoinc();
        reset_coinc();

        //don't care for now if a Ge or BGO starts an event.
        T0 = time;
        if (isBGO(label)) {
           coinc_BGOTime[0] = time;
           coinc_BGONrj[0] = nrjCal(label, nrj);
           coinc_BGOLabel[0] = label;
           coinc_BGOmult++;
        } else {
           coinc_GeTime[0] = time;
           coinc_GeNrj[0] = nrjCal(label, nrj);
           coinc_GeLabel[0] = label;
           coinc_Gemult++;
        }

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
   hGeGe->Write();

   outfile->Close();
   printf("Finished.\nFound %lld coincidences.\nWrote %lld entries to the new TTree.\n", nCoincidences, coinc_entry);
}


void NuBall_eventbuilderR5::printCoinc()
{
   if (coinc_mult < 2)
      return;
   int i = 0;
   printf("+++Found coincidence %lld:\n", nCoincidences);
   for (i=0; i<coinc_Gemult; i++) {
      printf("Ge:  label%d - energy%d - time %lld\n", coinc_GeLabel[i], coinc_GeNrj[i], coinc_GeTime[i]);
   }
   for (i=0; i<coinc_BGOmult; i++) {
      printf("Ge:  label%d - energy%d - time %lld\n", coinc_BGOLabel[i], coinc_BGONrj[i], coinc_BGOTime[i]);
   }
   printf("**multiplicities: tot%d, bgo%d, ge%d\n", coinc_mult, coinc_BGOmult, coinc_Gemult);
   printf("//\n");
   return;
}

void NuBall_eventbuilderR5::printCurrBranch()
{
   int i = 0;
   printf("+++Current Branch content %lld:\n", coinc_entry);
   for (i=0; i<coinc_mult; i++) {
      printf("Ge:  label%d - energy%d - time %lld\n", coinc_GeLabel[i], coinc_GeNrj[i], coinc_GeTime[i]);
   }
   for (i=0; i<coinc_mult; i++) {
      printf("BGO:  label%d - energy%d - time %lld\n", coinc_BGOLabel[i], coinc_BGONrj[i], coinc_BGOTime[i]);
   }
   printf("**multiplicities: tot%d, bgo%d, ge%d\n", coinc_mult, coinc_BGOmult, coinc_Gemult);
   printf("//\n");
   return;
}

void NuBall_eventbuilderR5::fillHistograms()
{
   int i = 0;
   int j = 0;
   int ref_pos = -1;
   long long ref_GeTime = 0;

   for (i=0; i<coinc_Gemult; i++) {
      hg->Fill(coinc_GeNrj[i], coinc_GeLabel[i]);
      //check for the reference detector
      if (coinc_GeLabel[i] == T_REF) {
         ref_pos = coinc_GeLabel[i];
         ref_GeTime = coinc_GeTime[i];
         coinc_has_ref = 1;
      }
   for (i=0; i<coinc_BGOmult; i++) {
      hg->Fill(coinc_BGONrj[i], coinc_BGOLabel[i]);
   }


   }
   for (i=0; i<coinc_Gemult; i++) {
      //increment gg matrix
      for (j=i+1; j<coinc_Gemult; j++) {
         if (coinc_GeLabel[i] != coinc_GeLabel[j]) {
            hGeGe->Fill(coinc_GeNrj[i], coinc_GeNrj[j]);
            hGeGe->Fill(coinc_GeNrj[j], coinc_GeNrj[i]);
         }
      }
   }
   
   //make dt matrix rel to reference detector
   if (ref_pos > -1) {
      for (i=0; i<coinc_Gemult; i++) {
         if (coinc_GeLabel[i] == ref_pos)
            continue;
         hdt->Fill((Long64_t) (coinc_GeTime[i] - ref_GeTime), coinc_GeLabel[i]);
      }
      for (i=0; i<coinc_BGOmult; i++) {
         hdt->Fill((Long64_t) (coinc_BGOTime[i] - ref_GeTime), coinc_BGOLabel[i]);
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

bool NuBall_eventbuilderR5::isBGO(int this_label)
{
      if (
         this_label == 6 || // these are the BGOs in the mini setup
         this_label == 7 ||
         this_label == 14 ||
         this_label == 15 ||
         this_label == 20 ||
         this_label == 21 ||
         this_label == 26 ||
         this_label == 27 
      ) {
         return 1;
      } else {
         return 0;
      }
}


















