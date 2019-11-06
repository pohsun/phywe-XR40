void Draw_xraycal(){
  TFile *fZn = new TFile("pxar_Zn.root");
  TFile *fMo = new TFile("pxar_Mo.root");
  TFile *fAg = new TFile("pxar_Ag.root");
  TFile *fSn = new TFile("pxar_Sn.root");

  char hname[100];
  TF1 *f1 = new TF1("f1","gaus",0, 300);
  
  float nele[4]={8639./3.62, 17479./3.62, 22163./3.62, 25271./3.62};
  float vcal[16][4];
  float nele_err[4] = {8639./3.62*0.01, 17479./3.62*0.01, 22163./3.62*0.01, 25271./3.62*0.01};
  float vcal_err[4] = {0.01, 0.01, 0.03, 0.05};
  //float vcal_err[4] = {0.02, 0.02, 0.02, 0.02};
  TCanvas *c1 = new TCanvas("c1","",800, 800);

  c1->Draw();
  for(int iroc=0; iroc<16; iroc++){
    sprintf(hname,"Xray/q_Zn_C%d_V0",iroc);
    TH1D *hZn = (TH1D*)fZn->Get(hname);
    sprintf(hname,"Xray/q_Mo_C%d_V0",iroc);
    TH1D *hMo = (TH1D*)fMo->Get(hname);
    sprintf(hname,"Xray/q_Ag_C%d_V0",iroc);
    TH1D *hAg = (TH1D*)fAg->Get(hname);
    sprintf(hname,"Xray/q_Sn_C%d_V0",iroc);
    TH1D *hSn = (TH1D*)fSn->Get(hname);
    c1->Clear();
    c1->Divide(2,2);
    //fit Zn
    c1->cd(1);
    hZn->GetXaxis()->SetRangeUser(0., 250);
    f1->SetParameters(hZn->GetEntries(), hZn->GetBinCenter(hZn->GetMaximumBin()), 10.);
    hZn->Fit(f1,"0q","",hZn->GetBinCenter(hZn->GetMaximumBin())-15.,hZn->GetBinCenter(hZn->GetMaximumBin())+15.);
    hZn->Fit(f1,"0q","",f1->GetParameter(1)-1.5*f1->GetParameter(2), f1->GetParameter(1)+2.*f1->GetParameter(2));
    hZn->Fit(f1,"q","",f1->GetParameter(1)-1.5*f1->GetParameter(2), f1->GetParameter(1)+2.*f1->GetParameter(2));
    vcal[iroc][0]=f1->GetParameter(1);

    //fit Mo
    c1->cd(2);
    hMo->GetXaxis()->SetRangeUser(0., 250);
    f1->SetParameters(hMo->GetEntries(), hMo->GetBinCenter(hMo->GetMaximumBin()), 10.);
    hMo->Fit(f1,"0q","",hMo->GetBinCenter(hMo->GetMaximumBin())-15.,hMo->GetBinCenter(hMo->GetMaximumBin())+15.);
    hMo->Fit(f1,"0q","",f1->GetParameter(1)-1.5*f1->GetParameter(2), f1->GetParameter(1)+2.*f1->GetParameter(2));
    hMo->Fit(f1,"q","",f1->GetParameter(1)-1.*f1->GetParameter(2), f1->GetParameter(1)+2.*f1->GetParameter(2));
    vcal[iroc][1]=f1->GetParameter(1);

    //fit Ag
    c1->cd(3);
    hAg->GetXaxis()->SetRangeUser(0., 250);
    if(hAg->GetBinCenter(hAg->GetMaximumBin())>50.)
      f1->SetParameters(hAg->GetEntries(), hAg->GetBinCenter(hAg->GetMaximumBin()), 10.);
    else       f1->SetParameters(hAg->GetEntries(), 100., 10.);
    hAg->Fit(f1,"0q","",f1->GetParameter(1)-15.,f1->GetParameter(1)+15.);
    hAg->Fit(f1,"0q","",f1->GetParameter(1)-1.5*f1->GetParameter(2), f1->GetParameter(1)+2.*f1->GetParameter(2));
    hAg->Fit(f1,"q","",f1->GetParameter(1)-1.*f1->GetParameter(2), f1->GetParameter(1)+2.*f1->GetParameter(2));
    vcal[iroc][2]=f1->GetParameter(1);


    //fit sn
    c1->cd(4);
    hSn->GetXaxis()->SetRangeUser(0., 250);
    if(hSn->GetBinCenter(hSn->GetMaximumBin())>100.){
      f1->SetParameters(hSn->GetEntries(), hSn->GetBinCenter(hSn->GetMaximumBin()), 30.);
    }else{ 
      f1->SetParameters(hSn->GetEntries(), 140., 30.);
      hSn->Fit(f1,"0q","",100., 170.);
    }

    hSn->Fit(f1,"0q","",f1->GetParameter(1)-1.5*f1->GetParameter(2), f1->GetParameter(1)+2.*f1->GetParameter(2));
    hSn->Fit(f1,"0q","",f1->GetParameter(1)-1.5*f1->GetParameter(2), f1->GetParameter(1)+2.*f1->GetParameter(2));
    hSn->Fit(f1,"0q","",f1->GetParameter(1)-1.*f1->GetParameter(2), f1->GetParameter(1)+2.*f1->GetParameter(2));
    hSn->Fit(f1,"q","",f1->GetParameter(1)-0.8*f1->GetParameter(2), f1->GetParameter(1)+2.*f1->GetParameter(2));
    vcal[iroc][3]=f1->GetParameter(1);

    
    sprintf(hname,"xray_roc_%d.pdf",iroc);
    c1->SaveAs(hname);
    
  }

  TH1F *hs = new TH1F("hs","slope",100, 0., 100);
  TH1F *ho = new TH1F("ho","offset",100, -2000., 2000);

  TCanvas *c3 = new TCanvas("c3","",1200,600);
  c3->Divide(8,2);  

  TGraphErrors *tge;
  TH2F *h2 = new TH2F("h2","",100, 0, 200, 100, 0., 1e4);
  h2->SetNdivisions(505,"XY");
  h2->SetYTitle("Charge (ne)");
  h2->SetXTitle("Vcal");

  TF1 *f2 = new TF1("f2","pol1",40, 180);
  f2->SetLineColor(4);
  f2->SetLineWidth(2.);
  f2->SetNpx(10000);


  for(int iroc=0; iroc<16; iroc++){
    if(iroc<8) c3->cd(8-iroc);
    else c3->cd(iroc+1);
    float vcal4[4] = {vcal[iroc][0], vcal[iroc][1], vcal[iroc][2], vcal[iroc][3]};
    float vcal4_err[4] = {vcal[iroc][0]*vcal_err[0], vcal[iroc][1]*vcal_err[1], vcal[iroc][2]*vcal_err[2], vcal[iroc][3]*vcal_err[3]};
    tge = new TGraphErrors(4, vcal4, nele, vcal4_err, nele_err);
    h2->Draw();
    tge->SetMarkerStyle(8);
    tge->Fit(f2,"qF");
    tge->Draw("pe same");
    //f2->Update();
    //f2->Draw("same");
    hs->Fill(f2->GetParameter(1));
    ho->Fill(f2->GetParameter(0));
    printf("iroc %d, slope %.3f, offset %.2f \n", iroc, f2->GetParameter(1), f2->GetParameter(0));
  }
  TCanvas *c2 = new TCanvas("c2","",800,400);
  c2->Divide(2,1);
  c2->Draw();
  gStyle->SetOptStat(1);
  c2->cd(1);
  hs->Draw();
  c2->cd(2);
  ho->Draw();

  TFile *fout = new TFile("xray_slope.root","recreate");
  hs->Write();
  ho->Write();
  fout->Close();


}
