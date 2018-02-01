# 6.179-Final-Project
#### IAP 2018

An eclipse predictor utilizing Forward-Euler methods and sphere intersection mathematics to solve iteratively for eclipses (both solar and lunar) occurring in the future. Starting data is obtained from NASA’s HORIZONS system which can be used to generate ephemerides for solar-system bodies (see [HORIZONS Web-Interface](https://ssd.jpl.nasa.gov/horizons.cgi)). 

The solver reports total eclipses, but because of factors inside of its intersection calculations, may not report partial eclipses in which the centroid of the lunar body does not project onto any portion of the earth.

Accurate in reporting total eclipses up to 2030 and beyond (with default timestep values)!

Recommended to compile with -o3 optimization flag.
