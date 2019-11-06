Double_t PIF_gpTanH( Double_t *x, Double_t *par) {
  return par[3] + par[2] * TMath::TanH(par[0]*x[0] - par[1]);
}
Double_t PIF_err(Double_t *x, Double_t *par) {
  return par[3]*(TMath::Erf((x[0]-par[0])/par[1])+par[2]); 
} 


void Draw_GainPed(int i0=0){
  TFile *f = new TFile("pxar_20150409_124319.root");//pxar.root");
  TH1D *h[16];
  char hname[100];
  int start=i0;
  int end=i0+1;
  if(i0<0) {
    start=0;
    end = 16;
  }
  printf(" start %d, end %d \n", start, end);
  //TF1 *ff = new TF1("ff", PIF_gpTanH, 0., 2000., 4);
  TF1 *ff = new TF1("ff", PIF_err, 0., 2000., 4);
  //TF1 *ff = new TF1("ff","par[3]+par[2]*TMath::TanH(par[0]*x-par[1])", 0, 2000.);
  ff->SetLineColor(6);
  for(int ii=start; ii<end; ii++){
    sprintf(hname,"GainPedestal/gainPedestal_c20_r25_C%d", ii);
    h[ii] = (TH1D*)f->Get(hname);
    if(ii==0 || i0>0) h[ii]->Draw();
    else h[ii]->Draw("same");

    TH1D *h1 = (TH1D*)h[ii]->Clone();
    // for(int ibin=0; ibin<h1->GetNbinsX(); ibin++){
    //   printf("bin %d, content %d \n", ibin+1, h1->GetBinContent(ibin+1));
    // }
     // for(int ibin=0; ibin<h1->GetNbinsX(); ibin++){
     //   if(h1->GetBinError(ibin+1) > 1.) h1->SetBinError(ibin+1, 1.);
     // }

    // h1->SetBinContent(76, 0);
    // h1->SetBinContent(126, 0);
    // h1->SetBinContent(176, 0);
    // h1->SetBinContent(226, 0);
    // h1->SetBinError(76, 0);
    // h1->SetBinError(126, 0);
    // h1->SetBinError(176, 0);
    // h1->SetBinError(226, 0);

    //for tanh
    // ff->SetParameters(0.003, 0.8, 150, 100);
    // ff->SetParameter(0, 1.4e-3);
    // //ff->SetParLimits(0, 1.e-3, 2.e-3);
    // ff->SetParameter(1, 0.8);
    // ff->SetParLimits(1, 0., 20.); 
    // ff->SetParameter(2, 114.6);
    // ff->SetParLimits(2, 0., 2*114.6);    
    // ff->SetParameter(3, 119.4);


    //for err
    double hmax = h1->GetBinContent(h1->GetMaximumBin()); 
    double p0 = h1->GetBinLowEdge(h1->FindFirstBinAbove(0.5*hmax)); // half-point
    double p1 = 250.; // slope
    double p2 = 1.;
    double p3 = 0.5*hmax; // half plateau
    
    ff->SetParameters(p0, p1, p2, p3); 
    ff->SetParLimits(1, 50, 1000.); // keep!
    
    h1->SetMarkerStyle(25);
    h1->Fit(ff,"E");
    ff->Update();
    //h[ii]->Draw();
    //h1->Draw("same");
    h1->Draw();

  }
  
  


}
