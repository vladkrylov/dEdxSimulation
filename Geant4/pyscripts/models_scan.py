import os
from logistics import *


def get_tree_name(model_name):
    mac_name_to_G4_class = {
        "emstandard_opt0": "G4EmStandardPhysics",
        "emstandard_opt1": "G4EmStandardPhysics_option1",
        "emstandard_opt2": "G4EmStandardPhysics_option2",
        "emstandard_opt3": "G4EmStandardPhysics_option3",
        "emstandard_opt4": "G4EmStandardPhysics_option4",
        "emstandardWVI": "G4EmStandardPhysicsWVI",
        "emstandardSS": "G4EmStandardPhysicsSS",
        "emstandardGS": "G4EmStandardPhysicsGS",
        "emlivermore" : "G4EmLivermorePhysics",
        "empenelope": "G4EmPenelopePhysics",
        "emlowenergy": "G4EmLowEPPhysics"
        }
    
    return mac_name_to_G4_class.get(model_name) or "Unknown_model"


if __name__ == "__main__":
    out_file = os.path.join(get_proj_dir(), "PAI_HeIBut_80_20_models_scan.root")
    models = ["emstandard_opt0",
              "emstandard_opt1", 
              "emstandard_opt2", 
              "emstandard_opt3", 
              "emstandard_opt4",
#               "emstandardWVI",
#               "emstandardSS",
#               "emstandardGS",
              "emlivermore", 
              "empenelope",
              "emlowenergy"
              ]
    
    if os.path.isfile(out_file):
        os.remove(out_file)
    
    energy = 3.*MeV
    set_energy(energy)
    
    for m in models:
        set_phys_model(m)
        tree_name = get_tree_name(m)
        
        if not run2file(out_file, tree_name=tree_name):
            print "Something went wrong during one of the scan simulation."
            break
        
        
        
    