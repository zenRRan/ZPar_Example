// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * main.cpp - main app of the Chinese segmentor.                *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.11                        *
 *                                                              *
 ****************************************************************/

#include "definitions.h"
#include "segmentor.h"
#include "reader.h"
#include "writer.h"
#include "stdlib.h"
#include "options.h"

using namespace chinese ;

/*===============================================================
 *
 * main
 *
 *==============================================================*/

void process(const std::string &sInputFile, const std::string &sOutputFile, const std::string &sFeatureFile, const int &nBest, const std::string &sOutputScores, std::string sStackFile) {
   std::cout<<"Segmenting started"<<std::endl;
   int time_start = clock();
   CSegmentor *segmentor ;
   segmentor = new CSegmentor(sFeatureFile);
   CSentenceReader input_reader(sInputFile);
   CSentenceWriter outout_writer(sOutputFile);
   CStringVector *input_sent = new CStringVector;
   CStringVector *outout_sent = new CStringVector[nBest]; 

   CStringVector *stack_sent = new CStringVector;

   CSentenceReader stack_reader(sStackFile);

   double *scores = NULL;
   std::ofstream fileScores;

   int nCount=0;
   if (!sOutputScores.empty()) {
      scores = new double[nBest];
      fileScores.open(sOutputScores.c_str());
   }
   
   while( input_reader.readRawSentence(input_sent, false) ) {
      TRACE("Sentence " << nCount);
      ++nCount;
      if (!sStackFile.empty())
      {
          stack_reader.readRawSentence(stack_sent, false);
          segmentor->setstackinfo(stack_sent);
      }
      segmentor->segment(input_sent, outout_sent, scores, nBest);
      for (int i=0; i<nBest; ++i) {
         outout_writer.writeSentence(outout_sent+i);
         if (!sOutputScores.empty())
            fileScores << *(scores+i) << std::endl;
      }
   }
   delete input_sent;
   delete stack_sent;
   delete [] outout_sent;

   if (!sOutputScores.empty()) {
      delete [] scores;
      fileScores.close();
   }

   delete segmentor ; 

   std::cout<<"Segmenting has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC<<std::endl;
}

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {
   try {
      COptions options(argc, argv);
      CConfigurations configurations;
      configurations.defineConfiguration("n", "N", "N best list outout", "1");
      configurations.defineConfiguration("d", "Path", "save scores to Path", "");
      configurations.defineConfiguration("s", "Path", "privide stacking features in Path", "");
      // check arguments
      if (options.args.size() < 2 || options.args.size() > 4) {
         std::cout << "Usage: " << argv[0] << " model_file [input_file [outout_file]]" << std::endl;
         std::cout << configurations.message() << std::endl;
         return 1;
      }
      std::string warning = configurations.loadConfigurations(options.opts);
      if (!warning.empty()) {
         std::cout << "Warning: " << warning << std::endl;
      }
   
      // check options
      int nBest;
      if (!fromString(nBest, configurations.getConfiguration("n"))) {
         std::cout << "The N best specification must be an integer." << std::endl;
         return 1;
      }
      std::string sOutputScores = configurations.getConfiguration("d");
      std::string sStackFile = configurations.getConfiguration("s");

      // main
      std::string sInputFile = options.args.size() > 2 ? options.args[2] : "";
      std::string sOutputFile = options.args.size() > 3 ? options.args[3] : "";
      process(sInputFile, sOutputFile, options.args[1], nBest, sOutputScores, sStackFile);

      // return normal
      return 0;
   }
   catch(const std::string &e) {
      std::cerr << "Error: " << e << " Stop." << std::endl;
      return 1;
   }
}

