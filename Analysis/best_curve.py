import ROOT as r

from SystemOfUnits import *
from fig2LaTeX import halfwidth, fullwidth 
from __builtin__ import raw_input
from array import array
from subprocess import call

def get_G4_distribution(tfile, ttree_name):
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
    nbins = 100
#     h_min = min(distribution)
#     h_max = max(distribution)
    h_min = 0
    h_max = 50
    h = r.TH1F(name, name, nbins, h_min, h_max)
    for x in distribution:
        h.Fill(x)
    
    # attributes
    if color == 5:
        color = r.kYellow + 1
    h.SetLineColor(color)
    h.SetLineWidth(line_width)
    
    return h.Clone()


def metric_mean(h1, h2):
    return r.TMath.Abs(h1.GetMean() - h2.GetMean())


def metric_mean2(h1, h2):
    return metric_mean(h1, h2)**2


def metric_bins_ydiff(h1, h2):
    m = 0
    nbins = h1.GetNbinsX()
    for i in range(nbins):
        m += r.TMath.Abs(h1.GetBinContent(i) - h2.GetBinContent(i))
    return m


def metric_chi2(h1, h2):
    return 1 - h1.Chi2Test(h2)

        
def metric_dKolmogorov(d1, d2):
    a1 = array('d', sorted(d1))
    a2 = array('d', sorted(d2))
    return r.TMath.KolmogorovTest(len(d1), a1, len(d2), a2, "")


def metric_hKolmogorov(h1, h2):
    return h1.KolmogorovTest(h2)


def metric_bins_ydiff2(h1, h2):
    m = 0
    nbins = h1.GetNbinsX()
    for i in range(nbins):
        m += r.TMath.Abs(h1.GetBinContent(i) - h2.GetBinContent(i))**2
    return m


def find_best_index(current_hist, other_hists, metric_function):
    metric = None
    best_index = None
    for i in range(len(other_hists)):
        h = other_hists[i]
        new_metric = metric_function(h, current_hist)
        if metric is None or new_metric < metric:
            metric = new_metric
            best_index = i
    return best_index
 

def plot_comparison(G4_res_file, heed_res_file, heed_tree_name, Eprim):
    f = r.TFile(G4_res_file)
    ttree_names = [t.GetName() for t in f.GetListOfKeys() if t.GetName().startswith("Lcut=") and t.GetName().endswith("eV")]
    trees_for_plot = [tn for tn in ttree_names if int(tn[5:-2]) in [1, 2, 5, 10, 15, 20, 25, 50]]
    ttree_names = trees_for_plot
    
    G4hists = []
    G4dists = []
    for i in range(len(ttree_names)):
        d = get_G4_distribution(f, ttree_names[i])
        h_name = "Low cut = %s eV" % ttree_names[i][5:-2]
        h = get_hist(d, name=h_name, color=i+2)
        G4dists.append(d)
        G4hists.append(h)
    
    f_heed = r.TFile(heed_res_file)
    Heed_dist = get_G4_distribution(f_heed, heed_tree_name)
    Heed_hist = get_hist(Heed_dist, name="HEED", color=r.kBlack)
    
#     print "\n--- %s ---" % heed_tree_name
#     print find_best_index(Heed_hist, G4hists, metric_mean2)
    best_curve = find_best_index(Heed_hist, G4hists, metric_bins_ydiff2)
#     print find_best_index(Heed_hist, G4hists, metric_chi2)
     
    hists = G4hists + [Heed_hist]
    c1, hists[0] = fullwidth(hists[0])
     
    # Titles
    hists[0].Draw()
    hists[0].SetTitle("E primaries = %.1f MeV, best cut = %s eV" % (Eprim, ttree_names[best_curve][5:-2]))
    hists[0].GetXaxis().SetTitle("Deposit energy, keV")
    hists[0].GetYaxis().SetTitle("Counts / %.2f keV" % hists[0].GetBinWidth(0))
#     hists[0].SetMaximum(200);
    
    yhmax = hists[0].GetMaximum()
    for i in range(1, len(hists)):
        hists[i].Draw("same")
        if yhmax < hists[i].GetMaximum():
            yhmax = hists[i].GetMaximum()
            
    hists[0].SetMaximum((int(yhmax) / 10 + 1) * 10);
     
    c1.SetGrid()
#     c1.SetLogy()
     
    # Legend
    legend = r.TLegend(0.65,0.5,0.9,0.9);
    for h in hists:
        legend.AddEntry(h, h.GetName(), "l");
    legend.Draw();
     
#     c1, hists[0] = halfpage(hists[0])
     
    r.gStyle.SetOptStat(0)
#     hists[0].SetTitle("Energy deposit by 3 MeV electrons in 56 mm of He/isoButane 80/20")
    c1.Update()
    c1.Print("Eprim=%.1fMeV.pdf" % Eprim, "pdf")
    
#     raw_input("Press ENTER to stop the script")
    
if __name__ == "__main__":
    Heed_results_file = "/home/vlad/Thesis/Meetings/2Doro/1MeV_picture/Heed/ArCO2_70_30.root"
#     Heed_results_file = "/home/vlad/Thesis/Meetings/2Doro/1MeV_picture/Heed/HeiBtn_80_20.root"
    energies = [0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0]
    for i in range(len(energies)):
        kinetic_energy = energies[i]
        Geant4_results_file = "../Geant4/Results/ArCO2_70_30_E=%sMeV_cut_scan.root" % str(kinetic_energy)
#         Geant4_results_file = "../Geant4/Results/HeiBut_80_20_E=%sMeV_cut_scan.root" % str(kinetic_energy)
        heed_tree_name = "E=%.3fMeV" % (kinetic_energy + 0.511)
        plot_comparison(Geant4_results_file, Heed_results_file, heed_tree_name, Eprim=kinetic_energy)

#     call("pdfunite Eprim=*MeV.pdf ArCO2_70_30_Eprim_scan.pdf".split())
#     call("pdfunite Eprim=*MeV.pdf HeiBtn_80_20_Eprim_scan.pdf".split())
#     call("rm -f Eprim=*MeV.pdf".split())


