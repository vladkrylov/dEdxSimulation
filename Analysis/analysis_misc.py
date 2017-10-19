import ROOT as r

def get_color(i):
    color = i + 2
    
    if color == 10:
        color = r.kGray + 3
    elif color == 8:
        color = r.kRed - 3
    
    return color 