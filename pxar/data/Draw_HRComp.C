#include <TH1D.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TFile.h>
#include <TLine.h>
#include <TMarker.h>
#include <TLatex.h>
#include <TLegend.h>

#include <iostream>
using namespace std;


void Draw_HRComp(){
    //std::printf("num of input varialbes = %d\n",argc);
    //for(int i=0; i<argc; ++i){
    //    std::printf("%d %s\n",i,argv[i+1]);
    //}

    gStyle->SetOptStat(0);
    TCanvas *can = new TCanvas("can","",980,730);

    const int num=4;//four modules
    int tube_current = 50;
    char Modules[num][50]={"M3019_Fulltest","M3021_Fulltest","M3022_dacHi_149","M3024_dacHi_149"};

    /*
    //load log files
    FILE *logfile[num];
    char logname[100];
    int data1, caldel[num][16];
    double data2, eff[num][16];
    for(int i=0; i<num; ++i){
        sprintf(logname,"%s/pxar_HR_current_%d.log",Modules[i],tube_current);
        logfile = fopen(logname,"r");
        for (int j=0; j<datapoint; j++){
            fscanf(logfile,"%d %lf",&data1, &data2);
            calde[i][j]=data1; eff[i][j]=data2;
            printf("%d, %d, %f\n", i, data1, data2);
        }
    }
    */

    //load root files
    TFile *fin[num], *f;
    char fname[100];
    for(int i=0; i<num; ++i){
        sprintf(fname,"%s/pxar_HR_current_%d.root",Modules[i],tube_current);
        fin[i] = new TFile(fname);
    }
    //f = new TFile("commander_Fulltest.root");
    printf("Loading root file is OK!!\n");

    //Max. eff search


    //Draw Plots
    TLine *MarkerlineCB;
    TMarker *MarkerHR[num][16];
    TH1D *hCalDelScan[num][16];
    //TH1D *hCalDelValue = (TH1D*)f->Get("Pretest/pretestCalDel_V0");
    TLatex *tex, *texMarker;
    //int color[num]={kBlue,kGreen+2,kOrange+4,kMagenta+1,kOrange,kRed}, value;
    int color[num]={kBlue,kGreen+2,kMagenta+1,kRed}, caldel[num], eff[num], value;
    char hname[200], oname[100], text[100], valueTex[100];
    printf("Before loop is OK!!\n");

    for(int i=0; i<16; ++i){
        TLegend *leg = new TLegend(0.72,0.7,0.9,0.9);
        for(int j=0; j<num; ++j){
            sprintf(hname,"HighRate/HR_CalDelScan_eff_C%d_V0",i);
            hCalDelScan[j][i] = (TH1D*)fin[j]->Get(hname);
            hCalDelScan[j][i]->SetLineColor(color[j]);
            hCalDelScan[j][i]->SetLineWidth(2);
            leg->AddEntry(hCalDelScan[j][i],Modules[j],"f");
            if(j==0){
                hCalDelScan[j][i]->SetAxisRange(0.,1.05,"Y");
                hCalDelScan[j][i]->Draw();
            }
            else hCalDelScan[j][i]->Draw("same");

            caldel[j] = hCalDelScan[j][i]->GetMaximumBin();
            eff[j] = hCalDelScan[j][i]->GetMaximum();
            MarkerHR[j][i] = new TMarker(caldel[j],eff[j],20);
            MarkerHR[j][i]->SetMarkerColor(color[j]);
            MarkerHR[j][i]->SetMarkerSize(1.5);
            MarkerHR[j][i]->Draw("same");
            sprintf(valueTex,"Eff=%3f%%",eff[j]*100.);
            texMarker = new TLatex(caldel[j]+10,eff[i],valueTex);
            texMarker->SetTextSize(0.040);
            texMarker->SetTextColor(color[j]);
            texMarker->Draw("same");
            
            /*
            //value = hCalDelValue->GetBinContent(i+1);
            MarkerlineCB = new TLine(value,0,value,1.05);
            MarkerlineCB->SetLineColor(kBlack);
            MarkerlineCB->SetLineWidth(3);
            MarkerlineCB->Draw("same");
            sprintf(valueTex,"CalDel(CB)=%d",value);
            tex = new TLatex(13,0.8,valueTex);
            tex->SetTextSize(0.040);
            tex->SetTextColor(kBlack);
            tex->Draw("same");
            */
        }
        leg->Draw("same");
        sprintf(oname,"Comparison/HR_CalDelScan_C%d.png",i);
        can->SaveAs(oname);
    }
}
