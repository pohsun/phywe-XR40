void Draw_module_map(char fname[100]="pxar.root", char dir[100]="Xray", char source[100]="Mo", int createmask=0){

  //create noise mask file
  //int createmask=0;
  // Xray/hMap_Mo
  // HighRate/hitMap_daqbbtest
  
  gStyle->SetOptStat(1);
  TCanvas *c1 = new TCanvas("c1","",1200, 600);
  //char fname[100];
  char module[10];

  printf(" open file %s \n", fname);
  TFile *f = new TFile(fname);
  
  TH2D *h_map = new TH2D("h_map","",416, 0, 416, 160, 0, 160);
  TH1F *h1 = new TH1F("h1","",256, 0., 256);
  TH2D *h2;// = new TH1F();

  char hname[200];
  int nhits;
  int npix_null;
  int npix_noisy;

  if(createmask==1)  printf(" noisy mask list -------- \n");
  if(createmask==-1)  printf(" pixel list sees no xray -------- \n");

  float ntrg;
  if(strcmp(dir,"Xray")==0){
    sprintf(hname,"Xray/ntrig_%s_V0",source);
    TH1D *hntrg = (TH1D*)f->Get(hname);
    ntrg=hntrg->GetBinContent(1);
    printf(" Xray:phrun test with ntrigger sent %.0f \n", ntrg);
  }
  double flux[16];

  for(int iroc=0; iroc<16; iroc++) {
    if(strcmp(dir,"Xray")==0){
      sprintf(hname,"%s/hMap_%s_C%d_V0",dir, source,iroc);
    }else{
      sprintf(hname,"%s/%s_C%d_V0", dir, source, iroc);
    }
    printf(" fetch histogram %s  ", hname);
    h2 = (TH2D*)f->Get(hname);
    h2->Draw("col");
    flux[iroc] = h2->GetEntries()/ntrg/25.e-9/1.e3/0.81/0.82;
    printf(" entries %.0f \n", h2->GetEntries());
    for(int ix=0; ix<52; ix++){
      for(int iy=0; iy<80; iy++){	
	int xx = 52*8-1-ix - 52*iroc;
	int yy = 80*2-1-iy;
	if(iroc>7){
	  xx = ix + 52* (iroc-8);
	  yy = iy;
	}
	float bincount = h2->GetBinContent(ix+1,iy+1);
	nhits += bincount;
	if(bincount<2) npix_null++;
	if(createmask==1 && strcmp(dir,"BumpBonding")==0 ){
	  if(bincount>60) {
	    printf("pix %d %d %d \n", iroc, ix, iy);
	    npix_noisy++;
	  }
	}else	if(createmask==1){
	  if(bincount>1) {
	    printf("pix %d %d %d \n", iroc, ix, iy);
	    npix_noisy++;
	  }
	}

	if(createmask==-1){
	  if(bincount<2) printf("no hit pix %d %d %d \n", iroc, ix, iy);
	}
	h1->Fill(bincount);
	h_map->SetBinContent( xx+1, yy+1, bincount);
      }
    }
  }

  printf(" nhits in the module %d \n", nhits);
  printf(" npixels %d has no hit\n", npix_null);
  printf(" npixels %d is noisy\n", npix_noisy);
  printf(" flux of rocs (kHz/cm2)  %.2f, %.2f, %.2f %.2f %.2f, %.2f, %.2f %.2f\n", flux[0],flux[1],flux[2],flux[3],flux[4],flux[5],flux[6],flux[7]);
  printf("                         %.2f, %.2f, %.2f %.2f %.2f, %.2f, %.2f %.2f\n", flux[8],flux[9],flux[10],flux[11],flux[12],flux[13],flux[14],flux[15]);
  //  if(alivemap==0){
  TF1* f1 = new TF1("f1","gaus",0,100);
  h1->Fit(f1);
  float xmax = f1->GetParameter(1)+7.*f1->GetParameter(2);
  printf(" colz max %.2f \n", xmax);
  // h_map->SetMinimum(0.);
  // h_map->SetMaximum(xmax);
    //  }
  
  h_map->SetNdivisions(1,"XY");
  h_map->SetXTitle("Col");
  h_map->SetYTitle("Row");
  h_map->SetMaximum(5000.);
  h_map->SetLineWidth(0.2);
  h_map->Draw("colz");

  TLine *tl = new TLine();;
  tl->SetLineWidth(0.2);
  tl->SetLineStyle(3);
  for(int ii=0; ii<7; ii++) 
    tl->DrawLine(52*ii+52, 0, 52*ii+52, 160);
  tl->DrawLine(0, 80, 52*8, 80);
  TLatex tlx;
  tlx.SetTextSize(0.03);
  char txt[100];
  for(int ii=0; ii<8; ii++) {
    sprintf(txt,"ROC %d", ii);
    tlx.DrawLatex( 52*8-52*ii-35, 162, txt);

    sprintf(txt,"ROC %d", ii+8);
    tlx.DrawLatex( 17+52*ii, -8, txt);
  }

  TFile *fff = new TFile("fff.root","recreate");
  h_map->Write();
  fff->Close();

  TCanvas *c2 = new TCanvas("c2","");
  h1->Draw();


  TCanvas *c3 = new TCanvas("c3","",1200,600);
  c3->Draw();
  gPad->SetGridx();
  gPad->SetGridy();
  TH1D * h_proj = (TH1D*)h_map->ProjectionX();
  h_proj->SetMinimum(0.);
  h_proj->SetNdivisions(505,"XY");  
  h_proj->Rebin(2);
  h_proj->Draw();
  c3->SaveAs("Xray_rate.C");

  TH1D *hq;
  if(strcmp(dir,"Xray")==0) {
    printf("Draw xray spectrum from %s in vcal unit \n", source);
    
    TCanvas *c4 = new TCanvas("c4","",1200, 600);
    c4->Divide(8,2);
    for(int ii=0; ii<16; ii++){
      if(ii<8)	c4->cd(8-ii);
      else c4->cd(ii+1);
      sprintf(hname,"%s/q_%s_C%d_V0",dir,source,ii);
      hq = (TH1D*)f->Get(hname);
      //hq->GetXaxis()->SetRangeUser(0., 1000.);
      hq->GetXaxis()->SetRangeUser(0., 200.);
      hq->Draw();
    }
  }

  // if(alivemap==0){
  //   sprintf(fname,"bbmap.pdf");
  //   if(moduleID>0) sprintf(fname,"bbmap_m%s.pdf",module);
  // }else{
  //   sprintf(fname,"alivemap.pdf");
  //   if(moduleID>0)     sprintf(fname,"alivemap_m%s.pdf",module);
  // }
  // c1->SaveAs(fname);

  // if(alivemap==0){   Draw_alive(1, moduleID); }
  

}
    
