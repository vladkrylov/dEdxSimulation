import os

from change_parameter import change_parameter
from logistics import *
from SystemOfUnits import *

SCAN_RESULTS_FILE = os.path.join(get_proj_dir(), "Results/G4.root")

def clean_existing_results():
    os.remove(SCAN_RESULTS_FILE)


def scan(energies):
    mac_file = run_mac()
    for e in energies:
        set_energy(e)
        run2file(SCAN_RESULTS_FILE)

if __name__ == "__main__":
    clean_existing_results()
    scan([0.5, 3.0])
    