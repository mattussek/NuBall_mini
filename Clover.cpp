#include "Clover.h"

Clover::Clover(int ring, int number, int GeA, int GeB, int GeC, int GeD, int Bgo1, int Bgo2, bool addback) {
      ringNr = ring;
      detNr = number;
      GeLabel[0] = GeA;
      GeLabel[1] = GeB;
      GeLabel[2] = GeC;
      GeLabel[3] = GeD;
      BgoLabel[0] = Bgo1;
      BgoLabel[1] = Bgo2;
      useAddBack = addback;
      char histname [200];

      sprintf(histname, "Ge_CompSup_det%d", number);
      hGe_CompSup = TH1D (histname, "Ge Compton suppressed (one det)",8000,0,4000);
      sprintf(histname, "Ge_BGOveto_det%d", number);
      hGe_BgoVeto = TH1D(histname,"Ge vetoed by BGO (one det)",8000,0,4000) ;
      sprintf(histname, "Ge_det%d", number);
      hGe = TH1D(histname,"Ge (one det)",8000,0,4000) ;      
      sprintf(histname, "Ge_tot_det%d", number);
      hGe_single = TH1D(histname,"Ge total (one det)",8000,0,4000);

      sprintf(histname, "Ge_mult_det%d", number);
      hGeMult = TH1D(histname, "Ge multiplicity (one det)", 100,0,100);

      sprintf(histname, "Bgo_tot_det%d", number);
      hBgo_single = TH1D(histname,"Bgo total",8000,0,4000);
      sprintf(histname, "GeBgo_det%d", number);
      hGeBgo = TH2D(histname,"Ge-BGO coincidenc matrix (one det)",
                           8000,0,4000, 8000,0,4000);
      sprintf(histname, "subdet_det%d", number);
      hsubdet = TH2D(histname,"Energy spectra (one det)",
                           8000,0,4000, 6,0,6);
#if MAKE_BGO_GE_MULT
      sprintf(histname, "hBgoGeMult_det%d", number);
      hBgoGeMult = TH2D (histname, "Bgo-Germanium multiplicity", 10,0,10, 10,0,10);
#endif
      int i,j;
      for (i =0; i<4; i++) {
         sprintf(histname, "hAddBackdt_%d_%d_det%d", i, (i+1)%4, detNr);
         hAddBackdt[i] = TH1D(histname, "addback dt", 500,-2000,2000);
      }
      sprintf(histname, "hGeBgodt_det%d", number);
      hGeBgodt = TH1D(histname,"dt Ge-BGO (one det)", 500,-2000,2000);

      sprintf(histname, "hdt_det%d", number);
      hdt = TH1D(histname,"dt Ge-Ge (one det)", 500,-2000,2000);
      
      sprintf(histname, "hGeGe_det%d", number);
      hGeGe = TH2D(histname,"Ge-Ge coinc. (one det)",
                           4000,0,2000, 4000,0,2000);
      
      hasGe=1;//set to 1 in order for first reset to work.
      reset();

}


void Clover::reset()
{
   if (hasGe || hasBgo) {
      int i;
      hasGe = 0;
      hasBgo = 0;
      isGood = 0;
      hasVeto = 0;
      CloverNr = -1;
      for (i=0; i<4; i++) {
         GeTime[i]  = -10000;
         GeEnergy[i] = -1;
      }
      for (i=0; i<2; i++) {
         BgoTime[i] = -10000;
         BgoEnergy[i] = -1;
      }
      CloverEnergy = -1;
      CloverTime = -10000;
      dt = -10000;
   }
   return;
}

void Clover::WriteHistograms(TFile *thisFile)
{
   thisFile->cd();
   char dirname[200];
   sprintf(dirname, "Clover%02d_hist", detNr);
   thisFile->mkdir(dirname);
   thisFile->cd(dirname);
   printf("writing clover%02d hists\n", detNr);
   hGe_CompSup.Write();
   hGe_BgoVeto.Write();
   hGe_single.Write();
   hBgo_single.Write();
   hsubdet.Write();
   hGeMult.Write();
   hGe.Add(&hGe_CompSup, 1);
   hGe.Add(&hGe_BgoVeto, 1);
   hGe.Write();

   int i,j;
   for (i=0; i<4; i++) {
      hAddBackdt[i].Write();
   }
   hGeBgo.Write();
   hGeBgodt.Write();
   hdt.Write();
   hGeGe.Write();
#if MAKE_BGO_GE_MULT
   hBgoGeMult.Write();
#endif
   thisFile->cd();
}

bool Clover::ProcessGe(int this_label, Double_t this_energy, Long64_t this_time) 
{
   int i;
   for (i=0; i<4; i++) {
      if (this_label == GeLabel[i] && this_energy > 0) {
         GeEnergy[i] = this_energy;
         GeTime[i] = this_time;
         hsubdet.Fill(this_energy, i);
         hGe_single.Fill(this_energy);
         hasGe++;
         return 1;
      }
   }
   return 0;
}

bool Clover::ProcessBgo(int this_label, Double_t this_energy, Long64_t this_time)
{
   int i;
   for (i=0; i<2; i++) {
      if (this_label == BgoLabel[i] && this_energy > 0) {
         BgoEnergy[i] = this_energy;
         BgoTime[i] = this_time;
         hsubdet.Fill(this_energy, i+4); //bgo offset by four in subdet TH2
         hBgo_single.Fill(this_energy);
         hasBgo++;
         return 1;
      }
   }
   return 0;
}


bool Clover::AnalyseEvent()
{
   int i,j;
   hGeMult.Fill(hasGe);
   
   if (hasGe < 1) {
      CloverEnergy = -1;
      CloverTime = -10000;
      return isGood;
   }

   if (hasGe > 1) {
      for (i=0; i<4; i++) {
         if (GeEnergy[i] < 0)
            continue;
         for (j=i+1; j<4; j++) {
            if (GeEnergy[j] < 0)
               continue;
            dt = GeTime[i] - GeTime[(i+1)%4];
            hdt.Fill(dt);
            if(ADDBACK_WINDOW_LOW < dt
                && ADDBACK_WINDOW_HI > dt) {
               hGeGe.Fill(GeEnergy[i], GeEnergy[j]);
               hGeGe.Fill(GeEnergy[j], GeEnergy[i]);
            }
         }
      }
   }
   dt = -10000;
/*#if MAKE_BGO_GE_MULT   
   for (i=0; i<4; i++) {
      if (GeEnergy[i] > 0) {
         for (j=0; j<2; j++) {
            if (BgoEnergy[j] > BGO_VETO_THRESH) {
               hBgoGeMult.Fill(j, i);
            }
         }
      }
   }
#endif */
///IF ADDBACK ENABLED
   if (useAddBack) {
      if (hasGe > 2) { //Skip events with Ge mult>2      
         return 0;  
      }
      //Do AddBack (or find the single gamma if mult==1)
      for (i=0; i<4; i++) {
         if (GeEnergy[i] > 0) {
            if (hasGe == 2) {
               dt = GeTime[i] - GeTime[(i+1)%4];
               if( GeEnergy[i] > ADDBACK_THRESH
               && GeEnergy[(i+1)%4] > ADDBACK_THRESH) {
                     hAddBackdt[i].Fill(dt);
                  if(ADDBACK_WINDOW_LOW < dt
                  && ADDBACK_WINDOW_HI > dt) {
                     CloverEnergy = GeEnergy[(i+1)%4] + GeEnergy[i];
                     CloverTime = GeTime[i];
                     break;
                  }
               } else {
                  i++;
               }
            } else {
               CloverEnergy = GeEnergy[i];
               CloverTime = GeTime[i];
               break;
            }
         }
      }
      for (i=0; i<2; i++) {
         if (BgoEnergy[i] > BGO_VETO_THRESH) {
            dt = CloverTime - BgoTime[i];
            hGeBgodt.Fill(dt);
            for (j=0; j<4; j++) {
               //This assumes that the even crystals are on the side of BGO[0] and the
               //odd ones on the side of BGO[1]. Check this using hBgoGeMult and change
               //in call of constructor if necessary
               if ((j%2) == i && GeEnergy[j] > 0 && BGO_GE_WINDOW_LOW < dt && BGO_GE_WINDOW_HI > dt) {
                  hGeBgo.Fill(CloverEnergy, BgoEnergy[i]);
                  hGe_BgoVeto.Fill(CloverEnergy);
                  hasVeto = 1;
                  hBgoGeMult.Fill(i,j);
                  break;
               }
            }
         }
      }
      if(hasVeto == 0){
         hGe_CompSup.Fill(CloverEnergy);
      }
      return 1;
   }
///END IF ADDBACK ENABLED      
///IF ADDBACK DISABLED     
   if (!useAddBack && hasGe==1) {
      for(j=0; j<4; j++) {
         if (GeEnergy[j] < 0)
            continue;
         CloverTime = GeTime[j];
         CloverEnergy = GeEnergy[j];
         CloverNr = j;
         break;
      }
      for (i=0; i<2; i++) {
         if (BgoEnergy[i] > BGO_VETO_THRESH) {
            dt = CloverTime - BgoTime[i];
            //This assumes that the even crystals are on the side of BGO[0] and the
            //odd ones on the side of BGO[1]. Check this using hBgoGeMult and change
            //in call of constructor if necessary
            if ((CloverNr%2) == i && CloverEnergy > 0
                 && BGO_GE_WINDOW_LOW < dt && BGO_GE_WINDOW_HI > dt) {
               hGeBgo.Fill(CloverEnergy, BgoEnergy[i]);
               hGe_BgoVeto.Fill(CloverEnergy);
               hasVeto = 1;
               hBgoGeMult.Fill(i, CloverNr);
               break;
            }
         }
      }
      if(hasVeto == 0) {
         hGe_CompSup.Fill(CloverEnergy);
      }
      return 1;
   }
///END IF ADDBACK DISABLED
//return 0 if none of the above returns.
return 0;
}

void Clover::PrintSetting()
{
   printf("det%02d, Ge:(%02d, %02d, %02d, %02d) ; BGO(%02d, %02d)", detNr,
           GeLabel[0], GeLabel[1], GeLabel[2], GeLabel[3], BgoLabel[0], BgoLabel[1]);
   printf("AddBack:");
   if(useAddBack == 0) {
      printf("No\n");
   }else{
      printf("Yes\n");
   }
}















