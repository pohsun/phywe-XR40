void Draw_HR_xraymap(char fname[100]="pxar.root", char dir[] = "HighRate", char testName[]="highRate_xraymap", int binMaximum=500){
  gStyle->SetOptStat(0);
  TCanvas *c1 = new TCanvas("c1","",1800, 450);
  char module[10];

  printf(" open file %s \n", fname);
  TFile *f = new TFile(fname);
  
  TH2D *h_map = new TH2D("h_map","",416, 0, 416, 160, 0, 160);//52*8 x 80*2
  TH2D *h2 = 0;

  char hname[200];
  int nhits;
  int npix_null;
  int npix_noisy;

  for(int iroc=0; iroc<16; iroc++) {
    sprintf(hname,"%s/%s_C%d_V0", dir, testName, iroc);
    printf(" fetch histogram %s  ", hname);
    h2 = (TH2D*)f->Get(hname);
    h2->Draw("col");
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
	h_map->SetBinContent( xx+1, yy+1, bincount);
      }
    }
  }

  printf(" nhits in the module %d \n", nhits);
  
  h_map->SetNdivisions(1,"XY");
  h_map->SetXTitle("Col");
  h_map->SetYTitle("Row");
  h_map->SetMaximum(binMaximum);
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

  //c1->Print(TString::Format("%s_%s_module.pdf",dir,testName));
}
    
