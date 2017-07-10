import os
from logistics import *

if __name__ == "__main__":
    
    models = ["emstandard_opt1", "emstandard_opt2", "emstandard_opt3", "emstandard_opt4", "emlivermore", "empenelope", "pai"]
    energy = 3*MeV
    for m in models:
        set_phys_model(m)
        set_energy(energy/MeV)
        
        out_file = get_proj_path("Results/3MeV/G4" + m + ".root")
        os.remove(out_file)
        run2file(out_file, mode="recreate")
    