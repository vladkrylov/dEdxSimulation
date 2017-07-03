import ROOT as r
from array import array

def plot_event(tfile, evid):
    tree_name = "detectorTree"
    tree = tfile.Get(tree_name)
    
    PosX = r.vector('double')()
    tree.SetBranchAddress("PosX", PosX)
    
    n_events = tree.GetEntriesFast()
    
#     ntotal_hits = 0
#     for iev in range(n_events):
#         tree.GetEntry(iev)
#         ntotal_hits += PosX.size()
#     print("Total number of hits = %d" % ntotal_hits)
#     
#     
#     for iev in range(10):
#         tree.GetEntry(iev)
#         print("=== Event %d ===" % iev)
#         for i in range(PosX.size()):
#             print("%f\t%f" % (PosX[i], PosY[i]))

    c1 = r.TCanvas("Event%d" % evid, "canvas", 50, 50, 900, 600)
    nE_max = 1000
    h = r.TH1I("NElectrons", "NElectrons", nE_max, 0, nE_max)
    n_points = 0
    
    # fill the Graph
    for iev in range(n_events):
        tree.GetEntry(iev)
        n_hits = PosX.size()
        h.Fill(n_hits)
    
    print h.FindLastBinAbove(0)
    nE_max = h.GetXaxis().GetBinCenter(h.FindLastBinAbove(0)) + 1.5
    h.SetBins(int(nE_max), 0, nE_max)
    h.Draw()
    h.SetTitle("")
    h.SetLineWidth(2)
    h.GetXaxis().SetTitle("Number of electrons per track")
    h.GetYaxis().SetTitle("Counts")
    c1.SetGrid()
    
    raw_input('Press Enter to exit')
    
    
if __name__ == "__main__":
    tfile = r.TFile("../build/photoconversion.root")
    evid = 0
    plot_event(tfile, evid)
