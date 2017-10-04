import ROOT as r

from TFile_io import branch_to_list, get_hist
from SystemOfUnits import *
from fig2LaTeX import halfwidth, fullwidth
from analysis_misc import get_color
from __builtin__ import raw_input

def plot_comparison(G4_res_file, heed_res_file, binning=[]):
    if binning:
        x_min, x_max, n_bins = binning
    else:
        x_min = 0.
        x_max = 100.
        n_bins = 100
    
    f = r.TFile(G4_res_file)
    ttree_names = [t.GetName() for t in f.GetListOfKeys() if t.GetClassName() == "TTree"]
    
    hists = []
    for i in range(len(ttree_names)):
        h = get_hist(branch_to_list(f, ttree_names[i], tbranch_name="SecTotalEn")
                     , name=ttree_names[i]
                     , color=i+2
                     , x_min=x_min
                     , x_max=x_max
                     , n_bins=n_bins)
        hists.append(h)
    
    if heed_res_file:
        f_heed = r.TFile(heed_res_file)
        hists.append(get_hist(branch_to_list(f_heed, "E=3.511MeV", tbranch_name="dEPerTrack")
                              , name="HEED"
                              , color=get_color(i)
                              , x_min=x_min
                              , x_max=x_max
                              , n_bins=n_bins)
                     )
    
    c1, hists[0] = halfwidth(hists[0])
    hists.reverse()

    # here the hists are filled, start plotting
    yhmax = hists[0].GetMaximum()
    for i in range(1, len(hists)):
        if yhmax < hists[i].GetMaximum():
            yhmax = hists[i].GetMaximum()
    hists[0].SetMaximum((int(yhmax) / 10 + 1) * 10);
        
    # Titles
    hists[0].Draw()
    hists[0].GetXaxis().SetTitle("Number of secondary electrons")
    hists[0].GetYaxis().SetTitle("Counts")
    
    for i in range(1, len(hists)):
        hists[i].Draw("same")
    
    c1.SetGrid()
    c1.SetLogy()
    
    # Legend
    legend = r.TLegend(0.6,0.35,0.9,0.9);
    for h in hists:
        legend.AddEntry(h, h.GetName(), "l");
    legend.Draw();
    
#     c1, hists[0] = halfpage(hists[0])
    
    r.gStyle.SetOptStat(0)
    hists[0].SetTitle("")
#     hists[0].SetTitle("Energy deposit by 3 MeV electrons in 56 mm of He/isoButane 80/20")
    c1.Update()
    
    raw_input("Press ENTER to stop the script")
     
    
if __name__ == "__main__":
    Geant4_results_file = "../Geant4/Results/HeIBut_80_20_models_scan.root"
    Heed_results_file = ""
    binning = 0, 30, 70  # x_min, x_max, n_bins
        
    plot_comparison(Geant4_results_file, Heed_results_file, binning=binning)



