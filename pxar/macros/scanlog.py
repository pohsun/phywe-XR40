#!/usr/bin/env python
from ROOT import TH2F, gStyle, gPad, TLatex, TLine
import sys
from array import array


# 
if len(sys.argv)<3:
    print "Usage: python scanlog.py <logfile> <title>"
    sys.exit(1)
    
f=sys.argv[1]
t=sys.argv[2]

p=[[0]*8 for i in range(8)]
pr00=[[0]*8 for i in range(8)]
pr10=[[0]*8 for i in range(8)]
pr01=[[0]*8 for i in range(8)]
pr11=[[0]*8 for i in range(8)]


MA10=TH2F("MA10","",8*5, 0. ,8., 8*5, 0.,8.)
MA00=TH2F("MA00","",8*5, 0. ,8., 8*5, 0.,8.)
MA11=TH2F("MA10","",8*5, 0. ,8., 8*5, 0.,8.)
MA01=TH2F("MA00","",8*5, 0. ,8., 8*5, 0.,8.)

MB=TH2F("MB","",8*10, 0. ,8., 8*10, 0.,8.)
MNTP = TH2F("MNTP","",8,0., 8., 8, 0., 8.)

MA10.SetMinimum(0)
MA10.SetMaximum(4)
MA00.SetMinimum(0)
MA00.SetMaximum(4)

for l in open(f).readlines():
    
    if l.startswith("'ht/delay"):
        ht=int(l[10])
        tok=int(l[21])
        rd=int(l[-2])
        ntp=False
    elif l.startswith("'no token pass'"):
        nt, tok, rd =-1,-1,-1
        ntp=True
        
    elif l[4]==":":
        d400=int(l[2])
        dx,dy =[(0,0),(2,0),(4,0),(1,2),(3,2),(0,4),(2,4),(4,4)][d400]
        for d160 in range(8):
            r=l[8+3*d160]
            bx=2+d160*5+d400%3
            by=2+rd*5+ int(d400/3)
            cx=3+d160*10+dx
            cy=3+rd*10+ dy
            if r=="+":
                p[d400][d160]+=1
                if ht==0 and tok==0:
                    pr00[rd][d160]+=1
                    MA00.SetBinContent(bx,by,MA00.GetBinContent(bx,by)+1)
                    MB.SetBinContent(cx  ,cy  ,MB.GetBinContent(cx  ,cy  )+1)
                    MB.SetBinContent(cx+1,cy  ,MB.GetBinContent(cx+1,cy  )+1)
                    MB.SetBinContent(cx  ,cy+1,MB.GetBinContent(cx  ,cy+1)+1)
                    MB.SetBinContent(cx+1,cy+1,MB.GetBinContent(cx+1,cy+1)+1)
                elif ht==1 and tok==0:
                    pr10[rd][d160]+=1
                    MA00.SetBinContent(bx,by,MA00.GetBinContent(bx,by)+2)
                    MB.SetBinContent(cx  ,cy  ,MB.GetBinContent(cx  ,cy  )+2)
                    MB.SetBinContent(cx+1,cy  ,MB.GetBinContent(cx+1,cy  )+2)
                    MB.SetBinContent(cx  ,cy+1,MB.GetBinContent(cx  ,cy+1)+2)
                    MB.SetBinContent(cx+1,cy+1,MB.GetBinContent(cx+1,cy+1)+2)

                elif ht==0 and tok==1:
                    pr01[rd][d160]+=1
                    MA01.SetBinContent(bx,by,MA00.GetBinContent(bx,by)+1)
                elif ht==1 and tok==1:
                    pr11[rd][d160]+=1
                    MA11.SetBinContent(bx,by,MA10.GetBinContent(bx,by)+2)
                elif ntp:
                    MNTP.SetBinContent(d160+1, d400+1, 1)

for d400 in range(8):
    for d160 in range(8):
        print "%5d [%1d]"%(p[d400][d160],(2*(d400-d160)+20)%5),
    print
    
    
pm=[0,0,0,0,0]
for m in range(5):
    for d400 in range(8):
        for d160 in range(8):
            if ((2*(d400-d160)+20)%5) == m:
                pm[m] += p[d400][d160]
    print "%1d)  %3.1f ns   %3d"%(m, m*0.5, pm[m])
                
                
gStyle.SetOptStat(0)

if True:
    M=TH2F("M","# of valid port settings",8, 0. ,8., 8, 0.,8.)
    M.GetXaxis().SetTitle("160 MHz Delay setting")
    M.GetYaxis().SetTitle("400 MHz Delay setting")
    M2=TH2F("M2","",8, 0. ,8., 8, 0.,8.)
    for d400 in range(8):
        for d160 in range(8):
            M.SetBinContent(1+d160, 1+d400, (2*(d400-d160)+20)%5)
            M2.SetBinContent(1+d160,1+d400, p[d400][d160])
             
    M.Draw("colz")  # color indicates delay
    M2.Draw("sametext")
    MNTP.Draw("sameBOX")
    gPad.Update()
    gPad.Print("d160-d400-%s.png"%t)
    raw_input()



if False:
    MR=TH2F("MR","# of valid 400 MHz settings",80, 0. ,8., 80, 0.,8.)
    MR.GetXaxis().SetTitle("160 MHz Delay setting")
    MR.GetYaxis().SetTitle("ROC port Delay setting")
    for rd in range(80):
        for d160 in range(80):
            d = ( 0.1*(rd-d160) + 6.25*100) % 6.25
            if 2.0 <= d<= 5.5:
                MR.SetBinContent(1+d160, 1+rd, 1.)

    MR1=TH2F("MR1","",8, 0. ,8., 8, 0.,8.)
    MR0=TH2F("MR0","",8, 0. ,8., 8, 0.,8.)
    for rd in range(8):
        for d160 in range(8):
            MR1.SetBinContent(1+d160,1+rd, pr10[rd][d160]+0.1*pr11[rd][d160])
            MR0.SetBinContent(1+d160,1+rd, pr00[rd][d160]+0.1*pr01[rd][d160])
            
    MR0.GetXaxis().SetTitle("160 MHz Delay setting")
    MR0.GetYaxis().SetTitle("ROC port Delay setting")


    MR0.SetFillColor(2)
    MR0.SetMarkerColor(2)
    MR1.SetFillColor(4)
    MR1.SetMarkerColor(4)

    #MR.Draw("colz")
    MR0.Draw("text")
    MR1.Draw("sametext")
    
    

    tl=TLatex()
    tl.SetTextSize(0.03)
    tl.DrawLatex(3.0,7.5, "bands: ")
    tl.DrawLatex(3.0,7.1, "(160 MHz - port) modulo 6.25 ns  = 2.0 .. 5.5 ns")
    tl.DrawLatex(3.0,6.5, "Header/Trailer delay 0 = red, 1=blue")
    tl.DrawLatex(3.0,6.1, "[tokendelay=0].[tokendelay=1]")
    lines=[ TLine(4.5, 0., 8.0, 3.5), TLine(0.5, 0., 8., 7.5), 
        TLine(0., 1.5, 6.5, 8.0), TLine(0., 5.5, 2.5, 8) ]
    for l in lines:
        l.Draw()
        
    gPad.Update()
    gPad.Print("roc-d160%s.png"%t)
    raw_input()
 
 
if True:
    MA=TH2F("MA","valid 400 MHz settings %s"%t,80, 0. ,8., 80, 0.,8.)
    MA.GetXaxis().SetTitle("160 MHz Delay setting")
    MA.GetYaxis().SetTitle("ROC port Delay setting")

    gStyle.SetNumberContours(3)    
    gStyle.SetPalette(3, array('i',[0,3,2]));   

    MA.Draw()
    MB.Draw("samecol")
    #MA00.Draw("samecolz")
    #MA10.Draw("samecolz")
    
    

    tl=TLatex()
    tl.SetTextSize(0.03)
    tl.DrawLatex(3.0,7.5, "bands: ")
    tl.DrawLatex(3.0,7.1, "(160 MHz - port) modulo 6.25 ns  = 2.0 .. 5.5 ns")
    tl.DrawLatex(3.0,6.5, "Header/Trailer delay 0 = green, 1=red")
    lines=[ TLine(4.5, 0., 8.0, 3.5), TLine(0.5, 0., 8., 7.5), 
        TLine(0., 1.5, 6.5, 8.0), TLine(0., 5.5, 2.5, 8) ]
    for l in lines:
        l.Draw()
        
    gPad.Update()
    gPad.Print("roc-d160-%s.png"%t)
    raw_input()
 
