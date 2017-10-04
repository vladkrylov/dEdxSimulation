import os
import ROOT as r


def clean_existing_results(res_file):
    if os.path.isfile(res_file):
        os.remove(res_file)


def branch_to_list(tfile, ttree_name, tbranch_name):
    dist = []
    tree = tfile.Get(ttree_name)
    counter = 0
    for event in tree:
        counter += 1
        try:
            dist.append(getattr(event, tbranch_name))
        except:
            pass
        
    if len(dist) == 0:
        print "No data found for Geant4 distribution"
        
    return dist


def get_hist(distribution,
             name,
             color=r.kRed,
             line_width=2,
             n_bins=600,
             x_min=0,
             x_max=600
             ):
    h = r.TH1F(name, name, n_bins, x_min, x_max)
    for x in distribution:
        h.Fill(x)
    
    # attributes
    if color == 5:
        color = r.kYellow + 1
    h.SetLineColor(color)
    h.SetLineWidth(line_width)
    
    return h.Clone()


