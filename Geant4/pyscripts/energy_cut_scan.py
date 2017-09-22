import os

from change_parameter import change_parameter
from logistics import *
from SystemOfUnits import *

# SCAN_RESULTS_FILE = os.path.join(get_proj_dir(), "Results/HeiBut_80_20_E=1MeV_cut_scan.root")

def clean_existing_results(res_file):
    if os.path.isfile(res_file):
        os.remove(res_file)


def get_tree_name(val):
    return "Lcut=%deV" % (val/eV)


def scan(cuts, res_file):
    for en_lower, en_upper in cuts:
        set_secondary_cut_lower(en_lower)
        set_secondary_cut_upper(en_upper)
        tree_name = get_tree_name(en_lower)
        
        if not run2file(res_file, tree_name=tree_name):
            print "Something went wrong during one of the scan simulation."
            break


if __name__ == "__main__":
    cuts_lower = [E*eV for E in ([1, 2, 3, 4] + range(5, 51, 5))]
    cuts_upper = [100*GeV for E in cuts_lower]
    
    energies = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0]
    for i in range(len(energies)):
        kinetic_energy = energies[i]
        res_file = os.path.join(get_proj_dir(), "Results/ArCO2_70_30_E=%sMeV_cut_scan.root" % str(kinetic_energy))
        clean_existing_results(res_file)
        set_energy(kinetic_energy*MeV)
        scan(zip(cuts_lower, cuts_upper), res_file)

