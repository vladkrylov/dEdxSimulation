import os

from change_parameter import change_parameter
from logistics import *
from SystemOfUnits import *

SCAN_RESULTS_FILE = os.path.join(get_proj_dir(), "Results/cut_scan.root")

def clean_existing_results():
    if os.path.isfile(SCAN_RESULTS_FILE):
        os.remove(SCAN_RESULTS_FILE)


def savetree_name(val):
    return "Lcut=%deV" % (val/eV)


def scan(cuts):
    for en_lower, en_upper in cuts:
        set_secondary_cut_lower(en_lower)
        set_secondary_cut_upper(en_upper)
        
        if not run2file(SCAN_RESULTS_FILE, tree_name=savetree_name(en_lower)):
            print "Something went wrong during one of the scan simulation."
            break


if __name__ == "__main__":
    clean_existing_results()
    
    cuts_lower = [E*eV for E in [1, 200]]
    cuts_upper = [100*GeV for E in cuts_lower]
    
    scan(zip(cuts_lower, cuts_upper))

