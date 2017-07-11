import re
import os
import ROOT as r

from array import array
from subprocess import call

def plot(tfile):
    tfile.ls()
    dir_names = [k.GetName() for k in r.gDirectory.GetListOfKeys() if k.IsFolder()]
    n_points = len(dir_names)
    
    energies = []
    NeFitResults = {}
    dEFitResults = {}
    
    counter = 0
    for d in dir_names:
        energy = get_energy(d)
        energies.append(energy)
        
        tfile.cd(d)
        hE = tfile.Get("%s/dEPerTrack" % d)
        hE.SetNameTitle("%d_" % counter, d)
        counter += 1
        dEFitResults[energy] = landau_fit(hE, pdf_out=False)
        
    dEdxs = [dEFitResults[e][0]*1e-3 for e in energies]
    
    c1 = r.TCanvas("c", "c", 50, 50, 900, 600)
    
    gr1 = r.TGraph(n_points, array('d', energies), array('d', dEdxs))
    gr1.Draw("A*")
    gr1.SetTitle("HEED Bethe-Bloch curve")
    gr1.GetXaxis().SetTitle("Energy, MeV")
    gr1.GetYaxis().SetTitle("dE/dx, keV/track (56 mm)")
    c1.SetGrid()
    
    print("=== Results ===")
    print("E, MeV\tdE/dx, keV/track")
    for i in range(n_points):
        print("%.2f\t%.2f" % (energies[i], dEdxs[i]))
    
    raw_input('Press Enter to exit')
    

def set_fit_box(hist, canvas):
    r.gStyle.SetOptStat("emr");
    r.gStyle.SetOptFit(1101);
    
    ps = canvas.GetPrimitive("stats")
    ps.SetX1NDC(0.6)
    ps.SetX2NDC(0.9)
    ps.SetY1NDC(0.6)
    ps.SetY2NDC(0.9)
    
    canvas.Modified()


def landau_fit(hist, pdf_out=False):
    c1 = r.TCanvas("c", "c", 50, 50, 900, 600)

    func = "landau"
    hist.Fit(func)
    fit_func = hist.GetFunction(func)

    if pdf_out:
        hist.Draw()
        set_fit_box(hist, c1)
        c1.Print("%s.pdf" % hist.GetName())
    
    return fit_func.GetParameter("MPV"), fit_func.GetParameter("Sigma")
    

def get_energy(dir_name):
    """ Returns the 1st float in the passed string
    """
    return float(re.findall(r"[-+]?\d*\.\d+|\d+", dir_name)[0])


if __name__ == "__main__":
    tfile = r.TFile("../Results/UnitsConversion.root")
    plot(tfile)


