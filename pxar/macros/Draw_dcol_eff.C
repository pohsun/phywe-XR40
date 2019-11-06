void Draw_dcol_eff(){
  char txt[100];
  
  TH2F *h2_eff_flux = new TH2F("h2_eff_flux","eff vs flux", 250, 0., 250., 105., 0., 1.05);
  
  TH2D* h2_xraymap_1 = new TH2D();
  TH2D* h2_calmap_1 = new TH2D();

  float flux[5][16][26];
  float cal[5][16][26];
  float flux1d[2080];
  float cal1d[2080];
  float pix1d[2080];
  int npoint=0;

  for(int ifile=0; ifile<2; ifile++){
    int tmp=10;
    if(ifile==0) tmp=10;
    if(ifile==1) tmp=20;
    if(ifile==2) tmp=30;
    if(ifile==3) tmp=40;
    if(ifile==4) tmp=50;
    sprintf(txt,"pxar_HR_current_%d.root", tmp);
    TFile *f = new TFile(txt);
    
    float flux_roc=0;
    float cal_roc=0;
    for(int iroc=0; iroc<16; iroc++){
      //int nhit_roc=0;
      flux_roc=0;
      cal_roc=0;
      sprintf(txt,"HighRate/highRate_xraymap_C%d_V0",iroc);
      h2_xraymap = (TH2D*)f->Get(txt);
      sprintf(txt,"HighRate/highRate_calmap_C%d_V0",iroc);
      h2_calmap = (TH2D*)f->Get(txt);
      
      for(int dcol=0; dcol<26; dcol++){
	float nhit1=0;
	float ncal=0;
	float npix=0;
	for(int ix=0; ix<2; ix++){
	  for(int iy=0; iy<80; iy++){
	    int binx = dcol*2+ix+1;
	    int biny = iy+1;
	    nhit1 += h2_xraymap->GetBinContent(binx,biny);
	    float tmpcal = h2_calmap->GetBinContent(binx,biny);
	    if(tmpcal >0 ) {
	      ncal += tmpcal;
	      npix++;	      
	    }
	  }
	}
	float ff=0.;
	if(dcol==0 || dcol==51) ff= nhit1/416000./0.025/0.015/3./0.01/82.;
	else ff= nhit1/416000./0.025/0.015/2./0.01/82.;
	flux[ifile][iroc][dcol]=ff;
	cal[ifile][iroc][dcol] = ncal/npix;	
	//printf(" dcol %2d, flux %.2f \n", dcol, ff);
	//printf("dcol %2d, cal %.2f \n", dcol, ncal/npix);
	
	flux1d[npoint] = ff;
	cal1d[npoint] = ncal;
	pix1d[npoint] = npix;
	npoint++;

	flux_roc += ff;
	cal_roc += ncal;
      }
      printf("roc flux %.2f , cal %.2f\n", flux_roc/26., cal_roc/26./npix/100.);

    }
  }

  printf("npoint %d \n", npoint);
  TH1F *h1_den = new TH1F("h1_den","h1 den", 250, 0., 250);
  TH1F *h1_num = new TH1F("h1_num","h1 num", 250, 0., 250);
  for(int ii=0; ii<npoint; ii++){
    h1_den->SetBinContent( flux1d[ii]+1, pix1d[ii]*100.);
    h1_num->SetBinContent( flux1d[ii]+1, cal1d[ii]);
    //printf("npoint %d, pix %.2f, cal %.2f \n", ii, pix1d[ii], cal1d[ii]);
  }
  TGraphAsymmErrors *tgrs = new TGraphAsymmErrors();
  tgrs->BayesDivide(h1_num, h1_den);

  h2_eff_flux->GetYaxis()->SetRangeUser(0.5, 1.02);
  h2_eff_flux->SetXTitle("flux (MHz/cm^{2})");
  h2_eff_flux->Draw();
  tgrs->SetMarkerStyle(8);
  tgrs->Draw("pe same");


}

void Draw_dcol_uni(){
  TFile *f1 = new TFile("pxar_HR_current_20.root");
  TFile *f2 = new TFile("pxar_HR_current_40.root");

  TH2D* h2_xraymap_1 = new TH2D();
  TH2D* h2_xraymap_2 = new TH2D();
  TH1D* h_dcol_ratio[16];
  char txt[100];
  float ratiomax=0.; float ratiomin=10.;
  for(int iroc=0; iroc<16; iroc++){
    int nhit1_roc=0;
    int nhit2_roc=0;
    sprintf(txt,"HighRate/highRate_xraymap_C%d_V0",iroc);
    //printf(txt);
    h2_xraymap_1 = (TH2D*)f1->Get(txt);
    h2_xraymap_2 = (TH2D*)f2->Get(txt);
    sprintf(txt,"hdcol_ratio_C%d",iroc);
    h_dcol_ratio[iroc] = new TH1D(txt,txt,26, 0., 52);
    h_dcol_ratio[iroc]->SetMinimum(0.);
    //h_dcol_ratio[iroc]->SetMarkerStyle(8);
    //h2_xraymap_1->Draw("colz");
    for(int dcol=0; dcol<26; dcol++){
      float nhit1=0;
      float nhit2=0;

      for(int ix=0; ix<2; ix++){
	for(int iy=0; iy<80; iy++){
	  int binx = dcol*2+ix+1;
	  int biny = iy+1;
	  nhit1 += h2_xraymap_1->GetBinContent(binx,biny);
	  nhit2 += h2_xraymap_2->GetBinContent(binx,biny);
	}
      }
      //printf("dcol %d, entries %d , %d\n", dcol, nhit1, nhit2);      
      float ratio = 0;
      if(nhit1>0) {
	ratio = nhit2/nhit1;
	h_dcol_ratio[iroc]->SetBinContent(dcol+1, ratio);
	float err = TMath::Sqrt(nhit1)/nhit1*ratio;
	h_dcol_ratio[iroc]->SetBinError(dcol+1, err);
      }
      //if(ratio > ratiomax) ratiomax=ratio;
      nhit1_roc += nhit1;
      nhit2_roc += nhit2;
    }
    float flux1 = nhit1_roc/416000./0.025/0.015/54./0.01/82.;
    float flux2 = nhit2_roc/416000./0.025/0.015/54./0.01/82.;
    printf(" ROC %2d, hits %8d, flux %6.2f MHz/cm2, flux ratio %.2f \n", iroc, nhit2_roc, flux2, flux2/flux1);    

    for(int dcol=0; dcol<26; dcol++){
      h_dcol_ratio[iroc]->SetBinContent(dcol+1, h_dcol_ratio[iroc]->GetBinContent(dcol+1)/2.);
    }    
    if(h_dcol_ratio[iroc]->GetMaximum()>ratiomax) ratiomax = h_dcol_ratio[iroc]->GetMaximum();
    //if(h_dcol_ratio[iroc]->GetMinimum()<ratiomin) ratiomin = h_dcol_ratio[iroc]->GetMinimum();
  }
  //h_dcol_ratio[0]->Draw("pe");
  TCanvas *c4 = new TCanvas("c4","",1200, 600);
  c4->Divide(8,2);
  for(int ii=0; ii<16; ii++){
    if(ii<8)	c4->cd(8-ii);
    else c4->cd(ii+1);
    gPad->SetGridy();
    h_dcol_ratio[ii]->GetYaxis()->SetRangeUser(ratiomin*0.9, ratiomax*1.1);
    h_dcol_ratio[ii]->Draw("e");
  }


}
