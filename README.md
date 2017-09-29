# NuBall_mini
Eventbuilder and sort code for miniNuBall 09.2017


NuBall_ana: intended to do analysis on final event TTree

The follwing event builders work with root trees build by group2tree (faster-code).
Eventually event-building should be done on the raw data directly.

group2tree: First try at converting faster raw data to root. Aim to do event building here.

NuBall_eventbuilderR5: eventbuilder used with root5. Probably not up to date. USE ROOT 6!

NuBallEventBuilder: eventbuilder (root6). This does the basics like producing a g-g matrix and a time difference
                    spectra relative to a reference detector.
