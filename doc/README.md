# Notes

2019-01-19

List of spectra databases:
https://www.internetchemistry.com/chemistry/spectral%20database.php

Spectrum database:
http://spectra.arizona.edu/utzinger_spectra.sql

## 2019-01-15

gcc -I ../src -o estimate_rgb  estimate_rgb.c ../src/as7265x.c ../src/i2c.c ./stockman_sharpe.c -lm

Test with banana:

echo " 14126 4527 1598 1110 2898 6069 1758 2312 1963 689 3168 687 2196 639 1079 636 1552 4865" | ./estimate_rgb 



## 2019-01-14

Numerical models of eye color senstivity:
http://www.cvrl.org/

Human eye cones linear response: 
linss2_10e_fine.csv

https://mathematica.stackexchange.com/questions/57389/convert-spectral-distribution-to-rgb-color


Human cone spectral sensitivities (2 degree) or cone fundamentals. The Stockman & Sharpe (2000).
Downloaded as CSV file from http://www.cvrl.org/
"The spectral sensitivities of the middle- and long-wavelength-sensitive cones derived from measurements in observers of known genotype"
AndrewStock, Lindsay T.Sharpe
https://doi.org/10.1016/S0042-6989(00)00021-3
(available free online)




## 2019-01-13

Experimental setup

12V incadescent bulb is placed at the bottom of a steel can. 
It is covered with white tissue paper which acts as a diffuser.
The lamp is powered by a Rigol DP832 PSU.

The resistence of the leads is measured by measuring the short
circuit current. This wa found to be 0.591A at 0.08V. Ie R=0.135 ohms.
Lead resistence is used in the bulb power calculation.

The AS7268x (Sparkfun SKU ???) is taped to the lid.

calibrate.sh  ramps up the current through the bulb. A AS7265x
reading is taken at each  current increment.

Power is calculated by multiplying the bulb current (measured by the DP832) 
with the PSU voltage (measured by the DP832) minus a correction factor to
allow for the voltage drop on the leads.

Bulb temperature derivation:

The current is ramped up from zero until a faint glow is noticed. This is
the Draper point at 795K. It is hard to know exactly when this point is
reached, so there is some uncertainty about this value. It was found to 
be Ibulb=0.175A, Vpsu=0.831. In theory this datapoint can be used to derive temperature
from bulb power (ie Vbulb x Ibulb).

Pbulb = Ksb * T ^ 4

(Vpsu*Ipsu - Ipsu*Ipsu*Rleads) = Ksb * 798 ^ 4

Ksb = 0.141 / 798^4 = 3.477E-13

Tbulb = ((Vpsu*Ipsu - Ipsu*Ipsu*Rleads)/Ksb) ^ 0.25



## 2019-01-12

Some calibration experiments on the AS72651 (vis) sensor. Using incadescent halogen bulb. Nominally rated at 12V. 
Positioned approx 30mm from sensor (element directly over sensor aperture). Varying driving voltage.

V, A, 8 x adc, 8 x calib

0.0 0.000  1 1 0 0 0 0     1.178 1.040 0.000 0.000 0.000 0.000 
0.5 0.147  1 1 0 0 0 0     1.178 1.040 0.000 0.000 0.000 0.000 
1.0 0.187  1 1 1 1 2 3     1.178 1.040 0.924 0.888 1.854 3.536 
1.5 0.216  3 13 27 39 63 84     3.533 13.519 24.937 34.651 58.407 99.013 
1.7 0.224  7 32 65 95 143 184     8.244 33.279 60.032 84.407 132.574 216.885 
2.0 0.242  25 102 203 285 411 504     29.444 106.076 187.486 253.220 381.035 594.076 
2.3 0.256  57 239 454 635 867 1022     67.133 248.551 419.303 564.192 803.788 1204.655 
2.5 0.267  107 399 743 1018 1367 1582     126.021 414.944 686.217 904.484 1267.334 1864.739 
2.7 0.275  152 595 1079 1478 1924 2184     179.021 618.777 996.538 1313.190 1783.724 2574.330 
3.0 0.289  303 1051 1852 2486 3171 3535     356.864 1092.999 1710.462 2208.790 2939.807 4166.785 
3.2 0.298  394 1419 2468 3313 4123 4519     464.042 1475.705 2279.385 2943.572 3822.398 5326.647 
3.5 0.311  685 2234 3762 4966 6093 6599     806.773 2323.273 3474.491 4412.249 5648.768 7778.391 
3.7 0.319  767 2806 4593 6154 7371 7863     903.350 2918.131 4241.982 5467.777 6833.590 9268.296 
4.0 0.322  1327 4089 6640 8651 10291 10890     1562.901 4252.400 6132.542 7686.340 9540.697 12836.289 
4.2 0.340  1418 4914 7775 10282 11973 12499     1670.078 5110.368 7180.800 9135.470 11100.064 14732.854 
4.5 0.351  2297 6702 10606 13654 15849 16458     2705.339 6969.818 9795.442 12131.463 14693.471 19399.416 
4.7 0.359  2093 7680 11660 15497 17596 18045     2465.073 7986.900 10768.892 13768.953 16313.100 21270.049 
4.8 0.363  2305 8383 12637 16757 18951 19370     2714.761 8717.993 11671.225 14888.452 17569.309 22831.857 
4.9 0.366  2515 9089 13635 18047 20321 65535     2962.093 9452.205 12592.953 16034.606 18839.424 4294967296.000 
5.0 0.370  3680 10273 15835 20161 65535 65535     4334.195 10683.520 14624.819 17912.877 4294967296.000 4294967296.000 
5.2 0.377  3267 11508 17008 65535 65535 65535     3847.776 11967.871 15708.174 4294967296.000 4294967296.000 4294967296.000 
5.5 0.388  5523 14845 65535 65535 65535 65535     6504.826 15438.221 4294967296.000 4294967296.000 4294967296.000 4294967296.000 
5.7 0.395  4806 16353 65535 65535 65535 65535     5660.364 17006.482 4294967296.000 4294967296.000 4294967296.000 4294967296.000 
6.0 0.409  8392 65535 65535 65535 65535 65535     9883.849 4294967296.000 4294967296.000 4294967296.000 4294967296.000 4294967296.000 
6.2 0.412  6746 65535 65535 65535 65535 65535     7945.239 4294967296.000 4294967296.000 4294967296.000 4294967296.000 4294967296.000 
6.5 0.423  10749 65535 65535 65535 65535 65535     12659.854 4294967296.000 4294967296.000 4294967296.000 4294967296.000 4294967296.000 
6.7 0.428  9131 65535 65535 65535 65535 65535     10754.222 4294967296.000 4294967296.000 4294967296.000 4294967296.000 4294967296.000 
7.0 0.439  14264 65535 65535 65535 65535 65535     16799.717 4294967296.000 4294967296.000 4294967296.000 4294967296.000 4294967296.000 
7.2 0.445  12002 65535 65535 65535 65535 65535     14135.600 4294967296.000 4294967296.000 4294967296.000 4294967296.000 4294967296.000 
7.5 0.454  13860 65535 65535 65535 65535 65535     16323.897 4294967296.000 4294967296.000 4294967296.000 4294967296.000 4294967296.000 
7.7 0.461  15347 65535 65535 65535 65535 65535     18075.242 4294967296.000 4294967296.000 4294967296.000 4294967296.000 4294967296.000 
8.0 0.470  65535 65535 65535 65535 65535 65535     4294967296.000 4294967296.000 4294967296.000 4294967296.000 4294967296.000 4294967296.000 


Need to automate this. 
Can program Rigol DP832 to ramp up the lamp drive current. See https://github.com/hollanderic/dp832 for a little utility 
that can be adapted for this purpose.
 

2019-01-11

First light.

as7265x-20190111A.dat.gz  :  a few hours of hazy daylight near window



