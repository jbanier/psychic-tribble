from mpl_toolkits.basemap import Basemap
import matplotlib.pyplot as plt
import numpy as np
fig = plt.figure(figsize=(20,12))
#Custom adjust of the subplots
plt.subplots_adjust(left=0.05,right=0.95,top=0.90,bottom=0.05,wspace=0.15,hspace=0.05)
ax = plt.subplot(111)
#Let's create a basemap around Belgium
m = Basemap(resolution='i',projection='merc', llcrnrlat=49.0,urcrnrlat=52.0,llcrnrlon=1.,urcrnrlon=8.0,lat_ts=51.0)
m.drawcountries(linewidth=0.5)
m.drawcoastlines(linewidth=0.5)
 
m.drawparallels(np.arange(49.,53.,1.),labels=[1,0,0,0],color='red',dashes=[1,0],labelstyle='+/-',linewidth=0.2) # draw parallels
m.drawmeridians(np.arange(1.,9.,1.),labels=[0,0,0,1],color='red',dashes=[1,0],labelstyle='+/-',linewidth=0.2) # draw meridians
 
plt.show()
