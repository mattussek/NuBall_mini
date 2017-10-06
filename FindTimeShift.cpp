#include <TROOT.h>
#include <TH2.h>
#include <TH1.h>


void FindTimeShift (TH2D * thishist, int first, int last, int refdet, int offset = 0) {
   int current;
   long long max, entries;
   FILE *tsfile = fopen("TShifts", "w");
  printf("#Time shifts, reference det%2d\n#Gives the offset with respect to desired value\n#Id   timeshift (ns)\n", refdet);
   TH1D *projX;
   for (current = first; current < last; current++) {
      thishist->ProjectionX("tmp", current+1, current+1);
      projX = (TH1D*) gDirectory->Get("tmp");
      max = projX->GetMaximum();
      entries = projX->GetEntries();
      if (max < 2) {
        if (current == refdet) {
           fprintf(tsfile, "%02d  %d\n", current, 0);
           printf("%02d      reference\n", current);
        } else {
           printf("%02d          empty\n", current);
        }
      } else if ( (double) max/entries > 0.05 ) {
        projX->GetMaximumBin();
       fprintf(tsfile, "%02d  %d\n", current, (int)( projX->GetXaxis()->GetBinLowEdge(projX->GetMaximumBin()) - offset));
       printf("%02d  %13d  (%lf)\n", current, (int)( projX->GetXaxis()->GetBinLowEdge(projX->GetMaximumBin()) - offset), (double) max/entries );
//       printf("%02d  %lf\n", current,(double) max/entries);
      } else {
        printf("%02d  no clear peak  (%lf)\n", current,  (double) max/entries );
      }
      delete projX;
   }

   fclose(tsfile);
}
