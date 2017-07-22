import ROOT as r

def halfwidth(main_curve):
    canvas = r.TCanvas("c", "c", 50, 50, 600, 600)
    
    main_curve.GetXaxis().SetTitleSize(0.05);
    
    return canvas, main_curve


def fullwidth(main_curve):
    canvas = r.TCanvas("c", "c", 50, 50, 900, 600)
    
    main_curve.GetXaxis().SetTitleSize(0.04);
    main_curve.GetYaxis().SetTitleSize(0.04);
    
    return canvas, main_curve