# 6.179-Final-Project
#### IAP 2018

An eclipse predictor utilizing Forward-Euler methods and sphere intersection mathematics to solve iteratively for eclipses (both solar and lunar) occurring in the future. Starting data is obtained from NASA’s HORIZONS system which can be used to generate ephemerides for solar-system bodies (see [HORIZONS Web-Interface](https://ssd.jpl.nasa.gov/horizons.cgi)). 

The solver reports total eclipses, but because of factors inside of its intersection calculations, may not report partial eclipses in which the centroid of the lunar body does not project onto any portion of the earth.

Date and time of eclipse that is outputted represents the beginning of the eclipse event with respect to your computer's timezone. This does not mean, however, that your current location will be in the eclipse path.

Accurate in reporting total eclipses up to 2030 and beyond (with default timestep values)! 

Recommend compiling with -o3 optimization flag.
