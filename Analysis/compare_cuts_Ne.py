import ROOT as r

from TFile_io import branch_to_list, get_hist
from SystemOfUnits import *
from fig2LaTeX import halfwidth, fullwidth 
from __builtin__ import raw_input

def plot_comparison(G4_res_file, heed_res_file, binning=[]):
    if binning:
        x_min, x_max, n_bins = binning
    else:
        x_min = 0.
        x_max = 100.
        n_bins = 100
    
    f = r.TFile(G4_res_file)
    ttree_names = [t.GetName() for t in f.GetListOfKeys() if t.GetName().startswith("Lcut=") and t.GetName().endswith("eV")]
    
    hists = []
    for i in range(len(ttree_names)):
        h = get_hist(branch_to_list(f, ttree_names[i], tbranch_name="NePerTrack_Counted")
                     , name="Low cut = %s eV" % ttree_names[i][5:-2]
                     , color=i+2
                     , x_min=x_min
                     , x_max=x_max
                     , n_bins=n_bins)
        hists.append(h)
    
    f_heed = r.TFile(heed_res_file)
    hists.append(get_hist(branch_to_list(f_heed, "E=3.511MeV", tbranch_name="NePerTrack")
                          , name="HEED"
                          , color=i+5
                          , x_min=x_min
                          , x_max=x_max
                          , n_bins=n_bins)
                 )
    
    c1, hists[0] = fullwidth(hists[0])
    
    # Titles
    hists[0].Draw()
    hists[0].GetXaxis().SetTitle("Number of secondary electrons")
    hists[0].GetYaxis().SetTitle("Counts")
    
    for i in range(1, len(hists)):
        hists[i].Draw("same")
    
    c1.SetGrid()
#     c1.SetLogy()
    
    # Legend
    legend = r.TLegend(0.65,0.5,0.9,0.9);
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
#     Geant4_results_file = "../Geant4/Results/cut_scan.root"
#     Heed_results_file = "../Heed/Results/HeiBtn_80_20.root"
    
#     Geant4_results_file = "../Geant4/Results/ArC02_70_30_cut_scan.root"
#     Heed_results_file = "../Heed/Results/ArCO2_70_30.root"
     
#     Geant4_results_file = "../Geant4/Results/ArC02_70_30_w=28eV_cut_scan.root"
#     Heed_results_file = "../Heed/Results/ArCO2_70_30.root"

#     Geant4_results_file = "/home/vlad/Thesis/Meetings/2Doro/1MeV_picture/Geant4/ArCO2_70_30.root"
#     Heed_results_file = "/home/vlad/Thesis/Meetings/2Doro/1MeV_picture/Heed/ArCO2_70_30.root"

#     Geant4_results_file = "/home/vlad/Thesis/Meetings/2Doro/1MeV_picture/Geant4/HeiBtn_80_20.root"
#     Heed_results_file = "/home/vlad/Thesis/Meetings/2Doro/1MeV_picture/Heed/HeiBtn_80_20.root"

    Geant4_results_file = "/home/vlad/Thesis/Meetings/2Doro/1MeV_picture/Geant4/HeiBtn_80_20.root"
    Heed_results_file = "/home/vlad/Thesis/Meetings/2Doro/1MeV_picture/Heed/HeiBtn_80_20.root"
    binning = 0, 500, 500
        
    plot_comparison(Geant4_results_file, Heed_results_file, binning=binning)



