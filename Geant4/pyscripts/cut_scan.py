import os

from change_parameter import change_parameter
from logistics import *
from SystemOfUnits import *

SCAN_RESULTS_FILE = os.path.join(get_proj_dir(), "Results/1cm_HeIBut_80_20.root")

def clean_existing_results():
    if os.path.isfile(SCAN_RESULTS_FILE):
        os.remove(SCAN_RESULTS_FILE)


def get_tree_name(val):
    return "Lcut=%deV" % (val/eV)


def scan(cuts):
    for en_lower, en_upper in cuts:
        set_secondary_cut_lower(en_lower)
        set_secondary_cut_upper(en_upper)
        tree_name = get_tree_name(en_lower)
        
        if not run2file(SCAN_RESULTS_FILE, tree_name=tree_name):
            print "Something went wrong during one of the scan simulation."
            break


if __name__ == "__main__":
    clean_existing_results()
    
#     cuts_lower = [E*eV for E in [1, 2, 5, 10, 20, 25, 28, 50, 100]]
    cuts_lower = [E*eV for E in range(10, 100+1)]
    cuts_upper = [100*GeV for _ in cuts_lower]
    
    set_energy(3.*MeV)
    scan(zip(cuts_lower, cuts_upper))

