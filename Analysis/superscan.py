import ROOT as r

from SystemOfUnits import *
from fig2LaTeX import halfwidth, fullwidth
from TFile_io import get_branch_mean
from analysis_misc import get_color
from fig2LaTeX import halfwidth, fullwidth

line_colors = {"SecTotalEn": r.kRed, 
               "NePerTrack_Counted": r.kYellow + 3,
               "fEkinLost": r.kBlue,
               "TotalEDep": r.kGreen}

def fmean(l):
    return sum(l) / float(len(l))


def get_line(results, branch_name):
    for b, gr in results:
        if branch_name == b:
            n_points = gr.GetN()
            buffer = gr.GetY()
            return [buffer[i] for i in range(n_points)]
    return None


def add_lines(results):
    if len(results) == 0:
        return results
    
    results_upgr = list(results)
    
    n_points = results_upgr[0][1].GetN()
    xbuffer = results_upgr[0][1].GetX()
    x = [xbuffer[i] for i in range(n_points)]
    
    n_prim_ionsations = get_line(results, "NePerTrack_Counted")
    E_kin_lost = get_line(results, "fEkinLost")
    E_kin_created = get_line(results, "SecTotalEn")
    
    ####
    gr = r.TGraph()
    gr.SetName("Ekin_created - Ekin_lost")
    gr.SetLineWidth(2)
    gr.SetLineColor(r.kMagenta + 1)
    for i in range(n_points):
        gr.SetPoint(i, x[i], E_kin_created[i]-E_kin_lost[i])
    results_upgr.append(("diff1", gr))
    
    ####
    I = 24.6e-3
    gr = r.TGraph()
    gr.SetName("Ekin_created - Ekin_lost + N*I")
    gr.SetLineWidth(2)
    gr.SetLineColor(r.kOrange - 3)
    for i in range(n_points):
        gr.SetPoint(i, x[i], E_kin_created[i]-E_kin_lost[i]+n_prim_ionsations[i]*I)
    results_upgr.append(("diff2", gr))
    
    return results_upgr
    
    
def plot_comparison(G4_res_file):
    f = r.TFile(G4_res_file)
    ttree_names = [t.GetName() for t in f.GetListOfKeys() if t.GetName().startswith("Lcut=") and t.GetName().endswith("eV")]
    
    roi = lambda cut: cut > 10.
#     filtered_names = ["Lcut=%deV" % cut for cut in range(10, 60)]
#     ttree_names = filter(lambda name: name not in filtered_names, ttree_names)

    branch_titles = {"SecTotalEn": "Secondaries kinetic energy [keV]", 
                     "NePerTrack_Counted": "Number of primary ionizations",
                     "fEkinLost": "Lost kinetic energy [keV]",
                     "TotalEDep": "Deposited energy [keV]"}
    results = [(branch, r.TGraph()) for branch in branch_titles.keys()]
    graph_counter = 0
#     yhmax = -100500e9
#     yhmin = +100500e9
    for branch, graph in results:
        graph_counter += 1
        graph.SetName(branch_titles[branch])
        graph.SetLineColor(line_colors.get(branch) or get_color(graph_counter))
        graph.SetLineWidth(2)
        for i in range(len(ttree_names)):
            cut_value = float(ttree_names[i][5:-2])
            if not roi(cut_value):
                continue
            mean_val, _ = get_branch_mean(f, ttree_names[i], branch)
            graph.SetPoint(i, cut_value, mean_val)
            
    results = add_lines(results)
            
    # here the hists are filled, start plotting
    branches_to_draw = ["SecTotalEn"
                        , "fEkinLost"
                        , "TotalEDep"
                        , "diff1"
                        , "diff2"]

    results_to_draw = [res for res in results if res[0] in branches_to_draw]
    graph0 = results_to_draw[0][1]
    c1, graph0 = fullwidth(graph0)

    ymin = r.TMath.MinElement(graph0.GetN(), graph0.GetY())
    ymax = r.TMath.MaxElement(graph0.GetN(), graph0.GetY())
#     graph0.SetMinimum(0.9*ymin)
    
    # Titles
    graph0.Draw()
    graph0.GetXaxis().SetTitle("Lower production threshold, eV")
#     graph0.GetYaxis().SetTitle("Secondaries kinetic energy [keV]")
#     graph0.GetYaxis().SetTitleOffset(1.1)
    
    for _, graph in results_to_draw[1:]:
        graph.Draw("same")
        ymin = min([ymin, r.TMath.MinElement(graph.GetN(), graph.GetY())])
        ymax = max([ymin, r.TMath.MaxElement(graph.GetN(), graph.GetY())])
        print(graph.GetName(), graph.GetLineColor())
        
    graph0.SetMinimum(0)
    graph0.SetMaximum(1.1*ymax)
    
    c1.SetGrid()
#     c1.SetLogy()
    
    # Legend
    legend = r.TLegend(0.45,0.5,0.9,0.9);
    for _, gr in results_to_draw:
        legend.AddEntry(gr, gr.GetName(), "l");
    legend.Draw();
    
    graph0.SetTitle("")
    
    c1.Update()
    raw_input("Press ENTER to stop the script")
     
    
if __name__ == "__main__":
    Geant4_results_file = "../Geant4/Results/1cm_HeIBut_80_20.root"
    plot_comparison(Geant4_results_file)



