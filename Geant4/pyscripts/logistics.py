import re
import ROOT as r

from os.path import join, isdir, isfile, normpath
from os import getcwd, access, remove, rename, X_OK
from subprocess import call
from change_parameter import change_parameter
from SystemOfUnits import *

def get_proj_dir():
    local_proj_dir = "/home/vlad/10g4work/gasEDep/Geant4"
    lx2_proj_dir = "/exp/leetech/simulations/dEdxSimulation/Geant4"
    if isdir(local_proj_dir):
        return local_proj_dir
    elif isdir(lx2_proj_dir):
        return lx2_proj_dir

    return None


def get_proj_path(path_in_project):
    p = join(get_proj_dir(), path_in_project)
    if not isdir(p) and not isfile(p):
        print("Seems like the requested %s does not exists." % p)
        return None

    return p


def get_build_dir():
    return get_proj_path("build")


def run_mac():
    return get_proj_path("TestEm8.in")


def vis_mac():
    vis_mac_fname = "vis.mac"
    proj_dir = get_proj_dir()
    if not proj_dir:
        return None
    
    vis_mac_full = join(proj_dir, vis_mac_fname)
    if isfile(vis_mac_full):
        return vis_mac_full

    return None


def form_tree_name(value=None, units=None):
    if value == None or units == None:  # get energy from run_mac
        with open(run_mac(), 'r') as f:
            candidates = re.findall("/gun/energy\s+(\d+\.*\d*\s+[a-zA-z]+)\s*\n", f.read())
            str_val = candidates[0]
            value, units = candidates[0].split()
            
    return "E=%.2f%s" % (float(value), units)
        


def ttree_exists(tfile, ttree_name):
    t = tfile.Get(ttree_name)
    try:
        t.IsFolder()
    except:
        return False
    
    return True
    
    
def G4sim_exe():
    exe_fname = "build/TestEm8"
    proj_dir = get_proj_dir()
    if not proj_dir:
        return None
    
    exe_full = join(proj_dir, exe_fname)
    if isfile(exe_full) and access(exe_full, X_OK):
        return exe_full
    

def run(mac_file):
    call([G4sim_exe(), mac_file])
    
    
def run2file(target_file, mode="update", tree_name=None):
    def_tfile_name = "G4.root"
    if not tree_name:
        tree_name = form_tree_name()
    
    set_ttree_name(tree_name)
    
    proj_dir = get_proj_dir()
    default_res_file = join(proj_dir, "build/%s" % def_tfile_name)
    
    run(run_mac())
    
    if getcwd() != normpath(get_build_dir()):  # run started from some arbitrary path 
        rename(join(getcwd(), def_tfile_name), default_res_file)  # move results file
    
    if not isfile(default_res_file):
        print("ROOT file %s with Geant4 simulation results not found!") % default_res_file
        return None
    
    if not isfile(target_file):  # target file does not exist, simple move 
        call(["mv", default_res_file, target_file])
    elif mode.lower() == 'recreate':
        remove(target_file)
    
    else:  # target file exists check if it already contains the same TTree
        f = r.TFile(target_file)
        if not ttree_exists(f, tree_name):  # TTree does not exist, merge files
            tmp = target_file + "_tmp"
            call(["hadd", "-f", tmp, default_res_file, target_file])
            remove(target_file)
            rename(tmp, target_file)
        else:  # TTree with the same name already exist, throw warning  
            print("TTree %s already exists in TFile %s. Merge was not performed.") % (tree_name, target_file)
            return None

    return True


def set_energy(en):
    change_parameter(run_mac(), "/gun/energy", "%.2f MeV" % (en/MeV))


def set_phys_model(m):
    change_parameter(run_mac(), "/testem/phys/addPhysics", "%s" % m)


def set_secondary_cut_upper(val):
    change_parameter(run_mac(), "/cuts/setHighEdge", "%.2f GeV" % (val/GeV))


def set_secondary_cut_lower(val):
    change_parameter(run_mac(), "/cuts/setLowEdge", "%.2f eV" % (val/eV))    


def set_ttree_name(val):
    change_parameter(run_mac(), "/testem/histo/setRootTreeName", val)    







    
    