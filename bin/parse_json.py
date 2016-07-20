#!/usr/bin/python

import requests

url = "https://downloads.arduino.cc/packages/package_index.json"
version = "1.0.6"
name    = "Intel Curie Boards"
local_zip = "/tmp/corelibs.zip"

# Download Arduino's default JSON
r = requests.get(url)
data = r.json()

# parse JSON to search for Curie package/platform
intel = (item for item in data["packages"] if item["name"] == "Intel").next()
core = (item for item in intel["platforms"] if item["version"] == version and item["name"] == name).next()
corelibs_url = core["url"]

# Download corelibs URL
print "Downloading Corelibs: %s" % corelibs_url
r = requests.get(corelibs_url)
with open(local_zip, "wb") as fh:
	fh.write(r.content)
