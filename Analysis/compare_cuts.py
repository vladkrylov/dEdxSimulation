import ROOT as r

from SystemOfUnits import * 
from __builtin__ import raw_input


def get_G4_resfile(model):
    return "../Geant4/Results/3MeV/G4" + model.lower() + ".root"


def get_G4_distribution(tfile, ttree_name):
    dist = []
    tree = tfile.Get(ttree_name)
    for event in tree:
        dist.append(event.dEPerTrack)
        
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
    h_max = 30
    h = r.TH1F(name, name, nbins, h_min, h_max)
    for x in distribution:
        h.Fill(x)
    
    # attributes
    h.SetLineColor(color)
    h.SetLineWidth(line_width)
    
    return h.Clone()


def plot_comparison():
    c1 = r.TCanvas("c", "c", 50, 50, 900, 600)
    f = r.TFile("../Geant4/Results/cut_scan.root")
    ttree_names = [t.GetName() for t in f.GetListOfKeys() if t.GetName().startswith("Lcut=") and t.GetName().endswith("eV")]
    
    hists = []
    for i in range(len(ttree_names)):
        d = get_G4_distribution(f, ttree_names[i])
        h_name = "Low cut = %s eV" % ttree_names[i][5:-2]
        h = get_hist(d, name=h_name, color=i+2)
        hists.append(h)
     
    hists[0].Draw()
    for i in range(1, len(hists)):
        hists[i].Draw("same")
     
    c1.SetLogy()
     
    raw_input("Press ENTER to stop the script")
    
    
if __name__ == "__main__":
    plot_comparison()



