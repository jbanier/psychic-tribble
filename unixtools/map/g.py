import urllib
from IPy import IP
import time

def getCoordinates():
    log = open('/var/log/ufw.log', 'r')
    coordinates = []
    tempip = {}
    for entry in log.readlines():
        kval = {}
        for raw in entry.split():
            try:
                (key, value) = raw.split('=')
                kval[key] = value
            except:
                pass
        ip = IP(kval['SRC'])
        if ip.iptype() != 'PRIVATE':
            try:
                tempip[kval['SRC']] = 1 + tempip[kval['SRC']]
            except KeyError:
                tempip[kval['SRC']] = 1
    for blockedip in tempip:
        try:
            rawweb = urllib.urlopen('http://api.hostip.info/get_html.php?ip='+ blockedip  +'&position=true').read()
        except IOError:
            rawweb = urllib.urlopen('http://api.hostip.info/get_html.php?ip='+ blockedip  +'&position=true').read()

        response = {}
        for l in rawweb.splitlines():
            if len(l) > 1:
                (key, value) = l.split(':')
                response[key] = value
        coordinates.append(response)
        print "Z",
        time.sleep(5)
    return coordinates


if __name__ == '__main__':
    print getCoordinates()
