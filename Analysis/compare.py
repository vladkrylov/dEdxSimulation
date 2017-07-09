import ROOT as r

from SystemOfUnits import * 
from __builtin__ import raw_input

def get_G4_distribution(model):
    f = r.TFile("../Geant4/build/G4EDep.root")
    dist = []
    tree = f.Get("GasDetector")
    for event in tree:
        dist.append(event.EDep)
        
    if len(dist) == 0:
        print "No data found for Geant4 distribution"
        
    return dist


def get_Heed_distribution(model=PAI):
    f = r.TFile("../Heed/Results.root")
    dist = []
    tree = f.Get("E=3MeV")
    for event in tree:
        dist.append(event.dEPerTrack)
        
    if len(dist) == 0:
        print "No data found for Geant4 distribution"
        
    return dist


def get_Degrad_distribution():
    pass


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


def plot_comparison(energy, linlogY=LIN):
    c1 = r.TCanvas("c", "c", 50, 50, 900, 600)
    hists = [get_hist(  get_G4_distribution(PAI), name="h1", color=r.kRed), 
             get_hist(get_Heed_distribution(PAI), name="h2", color=r.kGreen)]
    
    hists[0].Draw()
    for i in range(1, len(hists)):
        hists[i].Draw("same")
    
    if linlogY == LOG:
        c1.Setlogy()
        
    raw_input("Press ENTER to stop the script")
    
    
if __name__ == "__main__":
    plot_comparison(3*MeV)


