import ROOT as r

from SystemOfUnits import *
from fig2LaTeX import halfwidth, fullwidth
from TFile_io import branch_to_list, get_hist
from analysis_misc import get_color
from fig2LaTeX import halfwidth, fullwidth
from __builtin__ import raw_input


def plot_comparison(G4_res_file, heed_res_file, n_bins, x_min, x_max):
    f = r.TFile(G4_res_file)
    ttree_names = [t.GetName() for t in f.GetListOfKeys() if t.GetName().startswith("Lcut=") and t.GetName().endswith("eV")]
    
    filtered_names = ["Lcut=%deV" % cut for cut in [1, 2, 5, 10, 20]]
    ttree_names = filter(lambda name: name not in filtered_names, ttree_names)
    
    hists = []
    means_plot_data = [] 
    for i in range(len(ttree_names)):
        d = branch_to_list(f, ttree_names[i], "NePerTrack_Counted")
        h_name = "Low cut = %s eV" % ttree_names[i][5:-2]
        h = get_hist(d, name=h_name, n_bins=n_bins, x_min=x_min, x_max=x_max, color=get_color(i))
        hists.append(h)
        
        means_plot_data.append((int(ttree_names[i][5:-2]), h.GetMean()))
    
    if heed_res_file:
        f_heed = r.TFile(heed_res_file)
        hists.append(get_hist(get_G4_distribution(f_heed, "E=1.511MeV"), name="HEED", n_bins=n_bins, x_min=x_min, x_max=x_max, color=get_color(i)))
    
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
    hists[0].GetXaxis().SetTitle("Number of primary ionizations")
    hists[0].GetYaxis().SetTitle("Counts")
    hists[0].GetYaxis().SetTitleOffset(1.4)
#     hists[0].SetMaximum(90);
    
    for i in range(1, len(hists)):
        hists[i].Draw("same")
    
    c1.SetGrid()
#     c1.SetLogy()
    
    # Legend
    legend = r.TLegend(0.45,0.5,0.9,0.9);
    for h in hists:
        legend.AddEntry(h, h.GetName(), "l");
    legend.Draw();
    
#     c1, hists[0] = halfpage(hists[0])
    
    r.gStyle.SetOptStat(0)
    hists[0].SetTitle("")
    
    yhmax = hists[0].GetMaximum()
    for i in range(1, len(hists)):
        hists[i].Draw("same")
        if yhmax < hists[i].GetMaximum():
            yhmax = hists[i].GetMaximum()
            
    hists[0].SetMaximum((int(yhmax) / 10 + 1) * 10);
    c1.Update()
    
    for cut, mean in means_plot_data:
        print "%d\t%.2f" % (cut, mean)
    print "Heed\t%.2f" % hists[-1].GetMean()
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

#     Geant4_results_file = "../Geant4/Results/HeiBut_80_20_E=1MeV_cut_scan.root"
#     Heed_results_file = "/home/vlad/Thesis/Meetings/2Doro/1MeV_picture/Heed/HeiBtn_80_20.root"

    # panic verification
#     Geant4_results_file = "../Geant4/Results/ArCO2_70_30_d=1cm_E=1MeV_cut_scan_my.root"
#     Heed_results_file = "../Heed/Results/ArCO2_70_30_d=10mm.root"
    
#     Geant4_results_file = "../Geant4/Results/ArCO2_70_30_d=1cm_E=1MeV_cut_scan_SD.root"
#     Heed_results_file = "../Heed/Results/ArCO2_70_30_d=10mm.root"

    Geant4_results_file = "../Geant4/Results/HeIBut_80_20.root"
    Heed_results_file = ""
    x_min=0
    x_max=100
    n_bins=x_max

    plot_comparison(Geant4_results_file, Heed_results_file, n_bins=n_bins, x_min=x_min, x_max=x_max)



