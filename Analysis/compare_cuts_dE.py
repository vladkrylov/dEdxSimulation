import ROOT as r

from SystemOfUnits import *
from fig2LaTeX import halfwidth, fullwidth 
from __builtin__ import raw_input

def get_G4_distribution(tfile, ttree_name):
    dist = []
    tree = tfile.Get(ttree_name)
    counter = 0
    for event in tree:
        counter += 1
        try:
            dist.append(event.IonizEDep)
        except:
            pass
        
    if len(dist) == 0:
        print "No data found for Geant4 distribution"
        
    return dist

def get_Heed_distribution(tfile, ttree_name):
    dist = []
    tree = tfile.Get(ttree_name)
    counter = 0
    for event in tree:
        counter += 1
        try:
            dist.append(event.dEPerTrack)
        except:
            pass
        
    if len(dist) == 0:
        print "No data found for Geant4 distribution"
        
    return dist


def get_hist(distribution,
             name,
             color=r.kRed,
             line_width=2
             ):
    nbins = 200
    h_min = 0
    h_max = 60
    h = r.TH1F(name, name, nbins, h_min, h_max)
    for x in distribution:
        h.Fill(x)
    
    # attributes
    if color == 5:  # bright yellow
        color = r.kYellow + 1
    if color == 10:  # white
        color = r.kMagenta + 2
        
    h.SetLineColor(color)
    h.SetLineWidth(line_width)
    
    return h.Clone()


def plot_comparison(G4_res_file, heed_res_file):
    f = r.TFile(G4_res_file)
    ttree_names = [t.GetName() for t in f.GetListOfKeys() if t.GetName().startswith("Lcut=") and t.GetName().endswith("eV")]
    
    hists = []
    means_plot_data = [] 
    for i in range(len(ttree_names)):
        d = get_G4_distribution(f, ttree_names[i])
        h_name = "Low cut = %s eV" % ttree_names[i][5:-2]
        h = get_hist(d, name=h_name, color=i+2)
        hists.append(h)
        
        means_plot_data.append((int(ttree_names[i][5:-2]), h.GetMean()))
    
    f_heed = r.TFile(heed_res_file)
    hists.append(get_hist(get_Heed_distribution(f_heed, "E=1.511MeV"), name="HEED", color=i+5))
    
    c1, hists[0] = fullwidth(hists[0])
    
    # Titles
    hists[0].Draw()
    hists[0].GetXaxis().SetTitle("Primary ionization energy [keV]")
    hists[0].GetYaxis().SetTitle("Counts / %.2f keV" % hists[0].GetBinWidth(0))
#     hists[0].SetMaximum(90);
    
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

<<<<<<< HEAD
#     # panic verification
=======
    # panic verification
>>>>>>> stash
#     Geant4_results_file = "../Geant4/Results/ArCO2_70_30_d=1cm_E=1MeV_cut_scan_my.root"
#     Heed_results_file = "../Heed/Results/ArCO2_70_30_d=10mm.root"
    
#     Geant4_results_file = "../Geant4/Results/ArCO2_70_30_d=1cm_E=1MeV_cut_scan_SD.root"
#     Heed_results_file = "../Heed/Results/ArCO2_70_30_d=10mm.root"

<<<<<<< HEAD
    # new interface to PAI
    Geant4_results_file = "../Geant4/Results/ArCO2_70_30_d=1cm_E=1MeV_cut_scan_PAI.root"
=======
    Geant4_results_file = "../Geant4/Results/ArCO2_70_30.root"
>>>>>>> stash
    Heed_results_file = "../Heed/Results/ArCO2_70_30_d=10mm.root"

    plot_comparison(Geant4_results_file, Heed_results_file)



