
Double_t effi(Double_t *x, Double_t *par){
  double num=0.;
  //int ntrg = (int)x[0]+0.01;
  double ntrg = x[0];
  //printf("ntrg %f \n", ntrg);
  for (int ii==1; ii<=ntrg; ii++){
    printf("   ii %d, num %f \n", ii, num);
    num += TMath::Power(par[0],(ntrg-1));
  }
  printf(" ntrg %f, num %f \n", ntrg, num);
  return num/ntrg;
}

void Draw_hit_eff(){

  TCanvas *C1 = new TCanvas();
  c1->Draw();
  gPad->SetGridx();
  gPad->SetGridy();

  TTree *tt = new TTree();
  tt->ReadFile("eff_rate.dat","rate/F:eff/F");
  tt->SetMarkerStyle(8);
  
  TH2F *h2 = new TH2F("h2","",100, 0., 350., 100., 70., 102);
  h2->SetXTitle("hit rate (MHz/cm2)");
  h2->SetYTitle("fiducial efficiency (%)");
  h2->SetNdivisions(110,"XY");
  h2->Draw();
  tt->Draw("eff:rate","","pesame");

}
void Draw_hit_eff_current(){

  TCanvas *C1 = new TCanvas();
  c1->Draw();
  gPad->SetGridx();
  gPad->SetGridy();

  TTree *tt = new TTree();
  tt->ReadFile("eff_rate_current.dat","current/I:rate/F:eff/F");
  tt->SetMarkerStyle(8);
  
  TH2F *h2 = new TH2F("h2","",100, 0., 300., 100., 80., 102);
  h2->SetXTitle("hit rate (MHz/cm2)");
  h2->SetYTitle("fiducial efficiency (%)");
  h2->SetNdivisions(110,"XY");
  h2->Draw();
  tt->Draw("eff:rate","current==10","pesame");
  tt->SetMarkerStyle(24);
  tt->SetMarkerColor(2);
  tt->Draw("eff:rate","current==20","pesame");
  tt->SetMarkerStyle(25);
  tt->SetMarkerColor(4);
  tt->Draw("eff:rate","current==30","pesame");
  tt->SetMarkerStyle(26);
  tt->SetMarkerColor(6);
  tt->Draw("eff:rate","current==40","pesame");
  tt->SetMarkerStyle(23);
  tt->SetMarkerColor(3);
  tt->Draw("eff:rate","current==50","pesame");

  // tt->Draw("eff:rate/eff*100.","current==10","pesame");
  // tt->SetMarkerColor(2);
  // tt->Draw("eff:rate/eff*100.","current==20","pesame");
  // tt->SetMarkerColor(4);
  // tt->Draw("eff:rate/eff*100.","current==30","pesame");
  // tt->SetMarkerColor(6);
  // tt->Draw("eff:rate/eff*100.","current==40","pesame");
  // tt->SetMarkerColor(3);
  // tt->Draw("eff:rate/eff*100.","current==50","pesame");
  // tt->SetMarkerColor(7);
  // tt->Draw("eff:rate/eff*100.","current==60","pesame");
  // tt->SetMarkerColor(9);
  // tt->Draw("eff:rate/eff*100.","current==70","pesame");

  // tt->Draw("eff:rate","iroc>=4&&iroc<12","pesame");

  // tt->Draw("eff:rate","iroc<4||iroc>=12","pesame");
  //h2->Draw();
  TLatex *tlx = new TLatex();
  tlx->SetTextSize(0.04);
  tlx->DrawLatex(20, 87, "tube current 0.1mA");
  tlx->SetTextColor(2);
  tlx->DrawLatex(20, 86, "tube current 0.2mA");
  tlx->SetTextColor(4);
  tlx->DrawLatex(20, 85, "tube current 0.3mA");
  tlx->SetTextColor(6);
  tlx->DrawLatex(20, 84, "tube current 0.4mA");
  tlx->SetTextColor(3);
  tlx->DrawLatex(20, 83, "tube current 0.5mA");


}

void Draw_70(){
  float ntrg[4] = {1, 2, 5, 10};
  float eff[4] = { 100., 73.7, 49.4, 37.3};
  float flux[4] = { 203.9, 156.7, 106.5, 80.1};
  
  for(int ii=0; ii<4; ii++){
    flux[ii] = flux[ii]/eff[ii]*100.;
  }
  //TGraph *tg = new TGraph(4, flux, eff);
  TGraph *tg = new TGraph(4, ntrg, eff);
  tg->SetMarkerStyle(8);
  
  TF1 *f1 = new TF1("f1",effi, 0.5, 10.5,1);
  f1->SetParameter(0, 0.8);
  f1->SetParLimits(0, 0., 1.);
  f1->SetNpx(10);
  TCanvas *C1 = new TCanvas();
  c1->Draw();
  gPad->SetGridx();
  gPad->SetGridy();

  //TH2F *h2 = new TH2F("h2","",100, 150., 250., 100., 20., 102);
  TH2F *h2 = new TH2F("h2","",15, 0., 15., 100., 20., 102);
  h2->SetXTitle("hit rate (MHz/cm2)");
  h2->SetYTitle("fiducial efficiency (%)");
  h2->SetNdivisions(110,"XY");
  h2->Draw();

  tg->Draw("p same");
  //tg->Fit(f1);
  f1->Draw();
  TLatex *tlx = new TLatex();
  tlx->SetTextSize(0.04);
  tlx->DrawLatex(flux[0]+5, eff[0]-1, "Ntrg = 1");
  tlx->DrawLatex(flux[1]+5, eff[1]-1, "Ntrg = 2");
  tlx->DrawLatex(flux[2]+5, eff[2]-1, "Ntrg = 5");
  tlx->DrawLatex(flux[3]+5, eff[3]-1, "Ntrg = 10");
}
