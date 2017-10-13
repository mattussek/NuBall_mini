#include "Clover.h"

Clover::Clover(int number, int GeA, int GeB, int GeC, int GeD, int Bgo1, int Bgo2, bool addback = 1) {
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
      hGe_CompSup = TH1D (histname, "Ge Compton suppressed",8000,0,4000);
      sprintf(histname, "Ge_BGOveto_det%d", number);
      hGe_BgoVeto = TH1D(histname,"Ge vetoed by BGO",8000,0,4000) ;
      sprintf(histname, "Ge_single_det%d", number);
      hGe_single = TH1D(histname,"Ge single",8000,0,4000);
      sprintf(histname, "Ge_noAdd_det%d", number);
      hGe_noAdd = TH1D(histname,"Ge no add-back",8000,0,4000);
      sprintf(histname, "Ge_Add_det%d", number);
      hGe_Add = TH1D(histname,"Ge add-back",8000,0,4000);
      sprintf(histname, "Ge_CompSupAdd_det%d", number);
      hGe_CompSupAdd = TH1D(histname,"Ge Compton suppressed add-back",8000,0,4000);
      sprintf(histname, "Bgo_single_det%d", number);
      hBgo_single = TH1D(histname,"Bgo single",8000,0,4000);
      sprintf(histname, "GeBgo_det%d", number);
      hGeBgo = TH2D(histname,"Ge-BGO coincidenc matrix (one det)",
                           8000,0,4000, 8000,0,4000);
      sprintf(histname, "subdet_det%d", number);
      hsubdet = TH2D(histname,"Energy spectra (one det)",
                           8000,0,4000, 6,0,6);

      hasGe=1;//set to 1 in order for first reset to work.
      reset();

}


void Clover::reset()
{
   if (hasGe || hasBgo) {
      int i;
      hasGe = 0;
      hasBgo = 0;
      didAddBack = 0;
      hasVeto = 0;
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
   }
   return;
}

void Clover::WriteHistograms()
{
   hGe_CompSup.Write();
   hGe_BgoVeto.Write();
   hGe_single.Write();
   hGe_Add.Write();
   hGe_CompSupAdd.Write();
   hBgo_single.Write();
   hsubdet.Write();
   hGeBgo.Write();
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
   int i;
   if (hasGe < 1) {
      CloverEnergy = -1;
      CloverTime = -10000;
      return 0;
   }
   for (i=0; i<4; i++) {
      if (GeEnergy[i] > ADDBACK_THRESH) {
         //Do add-back only if exactly two Clover segments fired and
         //the are next neighbours (no diagonal add-back and no triple add-back)
         if (hasGe == 2 && GeEnergy[(i+1)%4] > ADDBACK_THRESH
          && ADDBACK_WINDOW_LOW < dt
          && ADDBACK_WINDOW_HI > dt) {
             CloverEnergy = GeEnergy[(i+1)%4] + GeEnergy[i];
             CloverTime = GeTime[i];
             didAddBack = 1;
             break;
         } else {
             CloverEnergy = GeEnergy[i];
             CloverTime = GeTime[i];
             //having no break here means that in case of a mult>2 event,
             //or if the time doesnt match for a mult==2 event,
             //CloverEnergy and CloverTime will have the value of the
             //Segment with the highest ID when leaving the loop.
         }   
      }
   }
   //have valid CloverTime and CloverEnergy if we get here.
   for (i=0; i<2; i++) {
      if (BgoEnergy[i] > BGO_VETO_THRESH) {
         dt = CloverTime - BgoTime[i];
         if (BGO_GE_WINDOW_LOW < dt && BGO_GE_WINDOW_HI > dt) {
            hGeBgo.Fill(CloverEnergy, BgoEnergy[i]);
            hGe_BgoVeto.Fill(CloverEnergy);
            hasVeto = 1;
         }         
      }
   }
   if (didAddBack) {
      hGe_Add.Fill(CloverEnergy);
      if (!hasVeto) {
         hGe_CompSupAdd.Fill(CloverEnergy);
      }
   } else {
      hGe_noAdd.Fill(CloverEnergy);
      if (!hasVeto) {
         hGe_CompSup.Fill(CloverEnergy);
      } else {
         hGe_BgoVeto.Fill(CloverEnergy);
      }
   }
   return 1;

}

void Clover::PrintSetting()
{
   printf("det%02d, Ge:(%02d, %02d, %02d, %02d) ; BGO(%02d, %02d)\n", detNr,
           GeLabel[0], GeLabel[1], GeLabel[2], GeLabel[3], BgoLabel[0], BgoLabel[1]);
}















