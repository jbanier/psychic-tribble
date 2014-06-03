#!/bin/env python
# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
from mpl_toolkits.basemap import Basemap
import numpy as np
import matplotlib.pyplot as plt
from g import getCoordinates

# llcrnrlat,llcrnrlon,urcrnrlat,urcrnrlon
# are the lat/lon values of the lower left and upper right corners
# of the map.
# resolution = 'c' means use crude resolution coastlines.
fig = plt.figure(figsize=(20,18))
m = Basemap(projection='cyl',llcrnrlat=-90,urcrnrlat=90,\
            llcrnrlon=-180,urcrnrlon=180,resolution='c')
m.drawcoastlines()
m.fillcontinents(color='grey',lake_color='black')
# lonlat, lonlon are lat/lon of London.
lonlat = 51.53; lonlon = 0.08
for c in getCoordinates():
    if len(c['Longitude']) > 2:
        lon = float(c['Longitude'])
        lat = float(c['Latitude'])
        m.drawgreatcircle(lon, lat,lonlon,lonlat,linewidth=2,color='b')
m.drawparallels(np.arange(-90.,91.,30.), color='red')
m.drawmeridians(np.arange(-180.,181.,60.), color='red')
m.drawmapboundary(fill_color='black')
plt.title("How about a nice game of chess?")
plt.show()
