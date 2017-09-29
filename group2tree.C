
/*
 *  The file '/usr/share/fasterac/data/smallgrp.fast' is a faster data file
 *  containing four channels of qdc_tdc data. The channels 1 and 3 are systematically
 *  grouped in this file (coinc).
 *  This program reads the data file, put the grouped channels 1 & 3 in a root tree
 *  and saves the given tree in 'group2tree_data.root'.
 *
 *  Rq : channels 2 and 4 are discarded
 *
 */


//  std includes
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//  root includes
#include "TFile.h"
#include "TTree.h"

//  fasterac includes
#include "fasterac/fasterac.h"
#include "fasterac/fast_data.h"
//#include "fasterac/qdc_caras.h"
#include "fasterac/adc.h"
#include "fasterac/utils.h"

using namespace std;


#define DATAFILENAME "60Co_first_mini_nuball_0001.fast"
#define ROOTFILENAME "rootfiles/60Co_first_mini_nuball_0001.root"
#define LABEL1  1
#define LABEL2  2
#define LABEL3  3
#define LABEL4  4
#define LABEL5  5
#define LABEL6  6
#define LABEL7  7
#define LABEL8  8
#define LABEL9  9
#define LABEL10  10
#define LABEL11 11
#define LABEL12 12
#define LABEL13 13
#define LABEL14 14
#define LABEL15 15
#define LABEL16 16
#define LABEL17 17
#define LABEL18 18
#define LABEL19 19
#define LABEL20 20
#define LABEL21 21
#define LABEL22 22
#define LABEL23 23
#define LABEL24 24
#define LABEL25 25
#define LABEL26 26
#define LABEL27 27
#define LABEL28 28
#define LABEL29 29
#define LABEL30 30
#define LABEL31 31
#define LABEL32 32
#define LABEL33 33
#define LABEL34 34
#define LABEL35 35
#define LABEL36 36

int main (int argc, char** argv) {
  
  /************/
  /*  FASTER  */
  /************/
  //  file reader
  faster_file_reader_p   reader;
  //  data
  faster_data_p          data;
  unsigned char          alias;
  unsigned short         label;
  //  group data
  faster_buffer_reader_p group_reader;
  char                   group_buffer [1500];
  unsigned short         lsize;  
  
  faster_data_p          group_data;
  //  qdc tdc data  (from faster group)
  //qdc_t_x1               qdc1;
  //qdc_t_x1               qdc3;
  //qdc_t_x1               qdc4;
  adc_data	adc;

  
  /**********/
  /*  ROOT  */
  /**********/
  
  //  root leaves : channels 1 and 3
  Int_t                 leaf_adc;
  Long64_t	        leaf_t; 
  Int_t                 leaf_label;

  //  root tree
  TTree*                 tree;
  char                   tree_title [256];
  //  root file
  TString                fName = ROOTFILENAME;
  TFile*                 root_file;
  
  
  //  print infos
  printf ("\n");
  printf ("  group2tree :\n");
  printf ("     - read faster file '%s'\n", DATAFILENAME);
  printf ("     - output those data to root file '%s'\n", ROOTFILENAME);
  printf ("\n");
  
  //  open faster file reader
  reader = faster_file_reader_open (DATAFILENAME);
  if (reader == NULL) {
    printf ("error opening file %s\n", DATAFILENAME);
    return EXIT_FAILURE;
  }

  
//  output root file
  root_file = new TFile (fName.Data (), "recreate");
  sprintf (tree_title, "faster to tree root test : DATAFILENAME");
  tree = new TTree ("DataTree", tree_title);
  tree->Branch ("label", &leaf_label, "label/I");
  tree->Branch ("nrj", &leaf_adc, "nrj/I");
  tree->Branch ("time", &leaf_t, "time/L");

  // main loop
  while ((data = faster_file_reader_next (reader)) != NULL) { //  read each data
    alias = faster_data_type_alias (data);
    label = faster_data_label (data);
    faster_data_load (data, &adc);

// Load the info to the leaves
      leaf_adc = adc.measure;
      leaf_label = label;
//      leaf_t = adc.delta_t;
      leaf_t = faster_data_clock_ns(data) ;

	// I send everything to the tree
      if(leaf_adc > 0 && leaf_label < 60) {
	 //if(leaf_label == 22)cout << "Det #: " << (int)leaf_label << "; Nrj = " << (double)leaf_adc << "; Event time = " << (double)leaf_t << endl;
	 tree->Fill ();
      }
}



  //  close files & quit
  faster_file_reader_close (reader);
  root_file->Write ();
  root_file->Close ();
  return EXIT_SUCCESS;
}


