# Degrad version 3.2
from subprocess import Popen, PIPE

# Simulation parameters
# FIRST CARD: NGAS,NDELTA,IMIP,NDVEC,NSEED,ESTART,ETHRM,ECUT
NGAS = 2
NDELTA = 10000
IMIP = 1
NDVEC = 1
NSEED = 0
ESTART = 1.69e6
ETHRM = 20.
ECUT = 5000.

# SECOND CARD: NGAS1 , NGAS2, NGAS3 , NGAS4 , NGAS5 , NGAS6
NGAS1 = 3
NGAS2 = 11
NGAS3 = 0
NGAS4 = 0
NGAS5 = 0
NGAS6 = 0

# THIRD CARD: FRAC1,FRAC2,FRAC3,FRAC4,FRAC5,FRAC6,TEMP,TORR
FRAC1 = 80.
FRAC2 = 20.
FRAC3 = 0.
FRAC4 = 0.
FRAC5 = 0.
FRAC6 = 0.
TEMP = 20.
TORR = 760.

# FOURTH CARD : EFIELD,BMAG,BTHETA,IWRITE,IPEN
EFIELD = 1200
BMAG = 0
BTHETA = 0
IWRITE = 0
IPEN = 0

# FIFTH CARD: DETEFF,EXCWGHT,KGAS,LGAS,LCMP,LRAY,LPAP,LBRM,IECASC
DETEFF = 0.
EXCWGHT = 0.55
KGAS = 0
LGAS = 0
LCMP = 1
LRAY = 1
LPAP = 1
LBRM = 0
IECASC = 0

input = """%d,%d,%d,%d,%d,%.1f,%.1f,%.1f
%d,%d,%d,%d,%d,%d
%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f
%.1f,%.1f,%.1f,%d,%d
%.1f,%.2f,%d,%d,%d,%d,%d,%d,%d
""" % (
    NGAS, NDELTA, IMIP, NDVEC, NSEED, ESTART, ETHRM, ECUT,
    NGAS1, NGAS2, NGAS3, NGAS4, NGAS5, NGAS6,
    FRAC1,FRAC2,FRAC3,FRAC4,FRAC5,FRAC6,TEMP,TORR,
    EFIELD,BMAG,BTHETA,IWRITE,IPEN,
    DETEFF,EXCWGHT,KGAS,LGAS,LCMP,LRAY,LPAP,LBRM,IECASC
)

print input

degrad = Popen(['/home/vlad/10g4work/gasEDep/Degrad/degrad'], stdout=PIPE, stdin=PIPE, stderr=PIPE)
out, err = degrad.communicate(input=input)
 
outfile = open("outfile.txt", "w")
outfile.write(out.decode())


