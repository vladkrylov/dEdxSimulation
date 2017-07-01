import ROOT as r
from array import array

def plot_event(tfile, evid):
    tree_name = "detectorTree"
    tree = tfile.Get(tree_name)
    
    PosX = r.vector('double')()
    PosY = r.vector('double')()
    
    tree.SetBranchAddress("PosX", PosX)
    tree.SetBranchAddress("PosY", PosY)
    
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
    g = r.TGraph()
    n_points = 0
    
    # fill the Graph
    for iev in range(n_events):
        tree.GetEntry(iev)
        n_hits = PosX.size()
        for ihit in range(n_hits):
            g.SetPoint(n_points, PosX[ihit], PosY[ihit])
            n_points += 1
    
    g.Draw("A*")
    g.SetTitle("")
    g.GetXaxis().SetTitle("X, cm")
    g.GetYaxis().SetTitle("Y, cm")
    c1.SetGrid()
    
    raw_input('Press Enter to exit')
    
    
if __name__ == "__main__":
    tfile = r.TFile("../build/photoconversion.root")
    evid = 0
    plot_event(tfile, evid)
