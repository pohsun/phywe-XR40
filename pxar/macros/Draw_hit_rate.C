// vim: set sts=4 sw=4 et:

void Draw_hit_rate_current(const int tubeCur = 50){

  char lineBuff[1024];
  char *posBuff = 0;
  double overallEfficiency[16];
  double hitRate[16];
  FILE *log = fopen(TString::Format("pxar_HR_current_%d.log",tubeCur).Data(),"r");
  if (!log){
      printf("INFO\t\t: File \'%s\' not found.\n");
      return;
  }
  while(fgets(lineBuff, 1024, log) != NULL){
      posBuff = strstr(lineBuff, "INFO: Vcal hit overall efficiency (%):  ");
      if (posBuff != NULL){
          posBuff = strtok(posBuff, ":");
          posBuff = strtok(NULL, ":");
          posBuff = strtok(NULL, " ");
          for(int iRoc = 0; iRoc < 16; iRoc++){ 
            overallEfficiency[iRoc] = 0.01*std::atof(posBuff);
            posBuff = strtok(NULL, " ");
            printf("Overall efficiency at ROC%d is %.1f (%%)\n", iRoc, 100*overallEfficiency[iRoc]);
          }
      }

      posBuff = strstr(lineBuff, "INFO: X-ray hit rate [MHz/cm2]:  ");
      if (posBuff != NULL){
          posBuff = strtok(posBuff, ":");
          posBuff = strtok(NULL, ":");
          posBuff = strtok(NULL, " ");
          for(int iRoc = 0; iRoc < 16; iRoc++){ 
            hitRate[iRoc] = std::atof(posBuff);
            posBuff = strtok(NULL, " ");
            printf("Hit rate at ROC%d is %.1f [MHz/cm2]\n", iRoc, hitRate[iRoc]);
          }
      }
      posBuff = 0;
  }
  
  TCanvas *canvas = new TCanvas("canvas","",600,200);
  
  TH2F *h2 =new TH2F("h2","",8 , 0., 8., 2, 0., 2.);
  h2->SetBinContent(8,2,hitRate[ 0]/overallEfficiency[ 0]);
  h2->SetBinContent(7,2,hitRate[ 1]/overallEfficiency[ 1]);
  h2->SetBinContent(6,2,hitRate[ 2]/overallEfficiency[ 2]);
  h2->SetBinContent(5,2,hitRate[ 3]/overallEfficiency[ 3]);
  h2->SetBinContent(4,2,hitRate[ 4]/overallEfficiency[ 4]);
  h2->SetBinContent(3,2,hitRate[ 5]/overallEfficiency[ 5]);
  h2->SetBinContent(2,2,hitRate[ 6]/overallEfficiency[ 6]);
  h2->SetBinContent(1,2,hitRate[ 7]/overallEfficiency[ 7]);
  h2->SetBinContent(1,1,hitRate[ 8]/overallEfficiency[ 8]);
  h2->SetBinContent(2,1,hitRate[ 9]/overallEfficiency[ 9]);
  h2->SetBinContent(3,1,hitRate[10]/overallEfficiency[10]);
  h2->SetBinContent(4,1,hitRate[11]/overallEfficiency[11]);
  h2->SetBinContent(5,1,hitRate[12]/overallEfficiency[12]);
  h2->SetBinContent(6,1,hitRate[13]/overallEfficiency[13]);
  h2->SetBinContent(7,1,hitRate[14]/overallEfficiency[14]);
  h2->SetBinContent(8,1,hitRate[15]/overallEfficiency[15]);
  for(int iRoc = 0; iRoc < 16; iRoc++){ 
      printf("Corrected hit rate at Bin%d is %.1f [MHz/cm2]\n",iRoc, hitRate[iRoc]/overallEfficiency[iRoc]);
  }
  
  h2->SetMarkerSize(4);
  h2->SetMarkerColor(1);
  h2->SetNdivisions(800,"X");
  h2->SetNdivisions(200,"Y");
  //h2->GetXaxis()->SetNdivisions(8,0,0);
  //h2->GetYaxis()->SetNdivisions(2,0,0);
  h2->SetStats(kFALSE);
  h2->Draw("COL TEXT");

  canvas->Print(TString::Format("pxar_HR_current_%d_hitRate.pdf",tubeCur));
  return;
}

void Draw_hit_rate(){
    Draw_hit_rate_current(10);
    Draw_hit_rate_current(20);
    Draw_hit_rate_current(30);
    Draw_hit_rate_current(40);
    Draw_hit_rate_current(50);
    return;
}
