#
# ----------------------------------------------------------------------------------- #
#
#  Python macro for illustrating test statistics among them the Kolmogorov-Smirnov test.
#
#  The Kolmogorov-Smirnov test (KS-test) is a general test for two distributions in 1D
#  are the same. This program applies both a binned and an unbinned KS test, and compares
#  it to a Chi2 test and a simple comparison of means.
#
#  References:
#    http://en.wikipedia.org/wiki/Kolmogorov-Smirnov_test
#
#  Author: Troels C. Petersen (NBI)
#  Email:  petersen@nbi.dk
#  Date:   23rd of September 2013
#
# ----------------------------------------------------------------------------------- #

from ROOT import *
from array import array


# ----------------------------------------------------------------------------------- #
def sqr(a) : return a*a
# ----------------------------------------------------------------------------------- #

# Setting what to be shown in statistics box:
gStyle.SetOptStat("emr")
gStyle.SetOptFit(1111)

# Random numbers from the Mersenne-Twister:
r = TRandom3()
r.SetSeed(0)

verbose = True


# ----------------------------------------------------------------------------------- #
# Make Histograms:
# ----------------------------------------------------------------------------------- #

# Resulting p-values for each test (means, Chi-Squared binned, KS binned, KS unbinned):
Hist_ProbMu  = TH1F("Hist_ProbMu",  "Hist_ProbMu",   50,  0.0, 1.0)
Hist_ProbCSb = TH1F("Hist_ProbCSb", "Hist_ProbCSb",  50,  0.0, 1.0)
Hist_ProbKSb = TH1F("Hist_ProbKSb", "Hist_ProbKSb",  50,  0.0, 1.0)
Hist_ProbKS  = TH1F("Hist_ProbKS" , "Hist_ProbKS",   50,  0.0, 1.0)


# ----------------------------------------------------------------------------------- #
# Generate and fit data:
# ----------------------------------------------------------------------------------- #

# How many experiments, and how many events in each:
Nexp = 10
Nevents = 1000

# Define the two Gaussians to be generated:
dist_meanA  =  0.0
dist_widthA =  1.0
dist_meanB  =  0.0
dist_widthB =  1.0


# Two distributions to be compared:
Hist_gaussA  = TH1F("Hist_gaussA", "Hist_gaussA",  100, -5.0, 5.0)
Hist_gaussB  = TH1F("Hist_gaussB", "Hist_gaussB",  100, -5.0, 5.0)


for iexp in range (Nexp) :

    x_gaussA = []
    x_gaussB = []

    sumA = [0.0, 0.0, 0.0]
    sumB = [0.0, 0.0, 0.0]

    # Make empty arrays:
    x_gaussA_array = array( 'd' ) 
    x_gaussB_array = array( 'd' ) 


    # Generate data:
    # --------------
    for i in range (Nevents) :
        xA = r.Gaus(dist_meanA, dist_widthA)
        sumA[1] += xA
        sumA[2] += xA*xA
        x_gaussA_array.append(xA)
        Hist_gaussA.Fill(xA)
    
        xB = r.Gaus(dist_meanB, dist_widthB)
        sumB[1] += xB
        sumB[2] += xB*xB
        x_gaussB_array.append(xB)
        Hist_gaussB.Fill(xB)


    # Test distributions:
    # -------------------

    # Test mean and width:
    meanA = sumA[1] / float(Nevents)
    widthA = sqrt(sumA[2] / float(Nevents) - meanA*meanA)
    meanB = sumB[1] / float(Nevents)
    widthB = sqrt(sumB[2] / float(Nevents) - meanB*meanB)
    dMean = meanA - meanB
    dMeanError = sqrt(sqr(widthA)/float(Nevents) + sqr(widthB)/float(Nevents))
    pMean = TMath.Erfc(dMean / dMeanError / sqrt(2.0)) / 2.0        # Turn a number of sigmas into a probability!
    Hist_ProbMu.Fill(pMean)

    # Binned Chi2 and Kolmogorov-Smirnov Test:
    # (see options at http://root.cern.ch/root/html/TH1.html#TH1:KolmogorovTest):
    pCSbinned = Hist_gaussA.Chi2Test(Hist_gaussB, "UU")
    pKSbinned = Hist_gaussA.KolmogorovTest(Hist_gaussB, "UU")
    Hist_ProbCSb.Fill(pCSbinned)
    Hist_ProbKSb.Fill(pKSbinned)

    # Unbinned Kolmogorov-Smirnov Test (requires sorting):
    x_gaussA_array = array('d', sorted(x_gaussA_array))
    x_gaussB_array = array('d', sorted(x_gaussB_array))

    # Make Kolmogorov test with unbinned data in two sorted arrays:
    pKS = TMath.KolmogorovTest(Nevents, x_gaussA_array, Nevents, x_gaussB_array, "")
    Hist_ProbKS.Fill(pKS)

    if (verbose and iexp < 10) :
      print " %4d:  pMean: %6.4f   pCSbinned: %6.4f   pKSbinned: %6.4f   pKS: %6.4f"%(iexp, pMean, pCSbinned, pKSbinned, pKS)

    if (Nexp > 1) :
        Hist_gaussA.Reset("ICE")
        Hist_gaussB.Reset("ICE")

    else :
        # Make a white canvas and plot the two distributions:
        c0 = TCanvas("c0","",120,20,900,600)
        c0.SetFillColor(0)
        
        Hist_gaussA.SetLineWidth(2)
        Hist_gaussA.SetLineColor(2)
        Hist_gaussA.Draw()
        
        Hist_gaussB.SetLineWidth(2)
        Hist_gaussB.SetLineColor(4)
        Hist_gaussB.Draw("same")


# ------------------------------------------------------------------ #
# Show the distribution of fitting results:
# ------------------------------------------------------------------ #

# Make a white canvas divided into four regions:
c1 = TCanvas("c1","",150,70,500,700)
c1.SetFillColor(0)
c1.Divide(1,4)
    
c1.cd(1)
Hist_ProbMu.SetMinimum(0.0)
Hist_ProbMu.SetLineWidth(2)
Hist_ProbMu.SetLineColor(4)
Hist_ProbMu.Draw()
    
c1.cd(2)
Hist_ProbCSb.SetMinimum(0.0)
Hist_ProbCSb.SetLineWidth(2)
Hist_ProbCSb.SetLineColor(4)
Hist_ProbCSb.Draw()
    
c1.cd(3)
Hist_ProbKSb.SetMinimum(0.0)
Hist_ProbKSb.SetLineWidth(2)
Hist_ProbKSb.SetLineColor(4)
Hist_ProbKSb.Draw()
    
c1.cd(4)
Hist_ProbKS.SetMinimum(0.0)
Hist_ProbKS.SetLineWidth(2)
Hist_ProbKS.SetLineColor(4)
Hist_ProbKS.Draw()
    
c1.Update()
# c1.SaveAs("TestDist.eps")



# ---------------------------------------------------------------------------------- 
raw_input('Press Enter to exit')