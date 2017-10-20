import ROOT as r

from TFile_io import branch_to_list, get_hist
from SystemOfUnits import *
from fig2LaTeX import halfwidth, fullwidth
from analysis_misc import get_color
from __builtin__ import raw_input

def plot_comparison(G4_res_file, heed_res_file, binning=[], branch_name="dEPerTrack", xtitle=""):
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
        h = get_hist(branch_to_list(f, ttree_names[i], tbranch_name=branch_name)
                     , name=ttree_names[i]
                     , color=get_color(i)
                     , x_min=x_min
                     , x_max=x_max
                     , n_bins=n_bins)
        hists.append(h)
    
    if heed_res_file:
        f_heed = r.TFile(heed_res_file)
        hists.append(get_hist(branch_to_list(f_heed, "E=3.511MeV", tbranch_name=branch_name)
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
    hists[0].GetXaxis().SetTitle(xtitle)
    hists[0].GetYaxis().SetTitle("Counts")
    hists[0].GetYaxis().SetTitleOffset(1.4)
    
    for i in range(1, len(hists)):
        hists[i].Draw("same")
        print hists[i].GetName(), hists[i].GetLineColor()
    
    c1.SetGrid()
#     c1.SetLogy()
    
    # Legend
    legend = r.TLegend(0.50, 0.45, 0.9, 0.9);
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
#     Geant4_results_file = "../Geant4/Results/HeIBut_80_20_models_scan.root"
#     Heed_results_file = ""
#     binning = 0, 35, 35  # x_min, x_max, n_bins
#     branch_name = "NePerTrack_Counted"
#     xtitle = "Number of primary ionization collisions"

#     Geant4_results_file = "../Geant4/Results/HeIBut_80_20_models_scan.root"
#     Heed_results_file = ""
#     binning = 0, 15, 75  # x_min, x_max, n_bins
#     branch_name = "SecTotalEn"
#     xtitle = "Kinetic energy of electrons released in primary ionizations"

    Geant4_results_file = "../Geant4/PAI_HeIBut_80_20_models_scan.root"
    Heed_results_file = ""
    binning = 0, 20, 120  # x_min, x_max, n_bins
    branch_name = "SecTotalEn"
    xtitle = "Kinetic energy of electrons released in primary ionizations [keV]"
    # legend = (0.55, 0.45, 0.9, 0.9)

#     Geant4_results_file = "../Geant4/PAI_HeIBut_80_20_models_scan.root"
#     Heed_results_file = ""
#     binning = 40, 150, 110  # x_min, x_max, n_bins
#     branch_name = "NePerTrack_Counted"
#     xtitle = "Number of primary ionization collisions"
#     # legend = (0.45, 0.45, 0.9, 0.9)
            
    plot_comparison(Geant4_results_file, Heed_results_file, binning=binning, branch_name=branch_name, xtitle=xtitle)



