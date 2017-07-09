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
        hN = tfile.Get("%s/NePerTrack" % d)
        hN.SetNameTitle("%d_" % counter, d)
        counter += 1
        NeFitResults[energy] = landau_fit(hN)
        
        hE = tfile.Get("%s/dEPerTrack" % d)
        hE.SetNameTitle("%d_" % counter, d)
        counter += 1
        dEFitResults[energy] = landau_fit(hE)
        
    mpv_ratios = [NeFitResults[e][0]/dEFitResults[e][0] for e in energies]
    sgm_ratios = [NeFitResults[e][1]/dEFitResults[e][1] for e in energies]
    
    c1 = r.TCanvas("c", "c", 50, 50, 900, 600)
    
    gr1 = r.TGraph(n_points, array('d', energies), array('d', mpv_ratios))
    gr1.Draw("A*")
    gr1.SetTitle("N_e to eV conversion coefficient (MPV ratios)")
    gr1.GetXaxis().SetTitle("Energy, MeV")
    c1.SetGrid()
    c1.Print("%d_Means.pdf" % counter)
    counter += 1
    
    gr2 = r.TGraph(n_points, array('d', energies), array('d', sgm_ratios))
    gr2.Draw("A*")
    gr2.SetTitle("N_e to eV conversion coefficient (Sigma ratios)")
    gr2.GetXaxis().SetTitle("Energy, MeV")
    c1.SetGrid()
    c1.Print("%d_Sigmas.pdf" % counter)
    counter += 1
    
    for i in range(n_points):
        print "%.3f\t%.5f" % (energies[i], mpv_ratios[i])
    
    print_and_clean()
    

def set_fit_box(hist, canvas):
    r.gStyle.SetOptStat("emr");
    r.gStyle.SetOptFit(1101);
    
    ps = canvas.GetPrimitive("stats")
    ps.SetX1NDC(0.6)
    ps.SetX2NDC(0.9)
    ps.SetY1NDC(0.6)
    ps.SetY2NDC(0.9)
    
    canvas.Modified()


def landau_fit(hist):
    c1 = r.TCanvas("c", "c", 50, 50, 900, 600)

    func = "landau"
    hist.Fit(func)
    fit_func = hist.GetFunction(func)
    hist.Draw()

    set_fit_box(hist, c1)
    c1.Print("%s.pdf" % hist.GetName())
    
    return fit_func.GetParameter("MPV"), fit_func.GetParameter("Sigma")
    

def get_energy(dir_name):
    """ Returns the 1st float in the passed string
    """
    return float(re.findall(r"[-+]?\d*\.\d+|\d+", dir_name)[0])


def print_and_clean():
    out_files = [f for f in os.listdir(".") if f.endswith('.pdf') and f[0].isdigit()]
    call(['pdfunite'] + out_files + ['proportionality.pdf'])

if __name__ == "__main__":
    tfile = r.TFile("../Results/UnitsConversion.root")
    plot(tfile)

