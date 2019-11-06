{
  gROOT->Reset();

  //gROOT->ProcessLine(".L ./setTDRStyle.C");
  //gROOT->ProcessLine("setTDRStyle()");

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);
  gStyle->SetFitFormat("3.3f");
  gStyle->SetPadColor(0);
  gStyle->SetPalette(1);
  TGaxis::SetMaxDigits(4);

  

  //  gSystem->Load("libFWCoreFWLite.so");
  //  AutoLibraryLoader::enable();

}
