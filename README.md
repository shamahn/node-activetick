node-activetick-addon
=====================
[![Gitter](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/dchem/node-activetick-addon?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)

ActiveTick API addon for Node.js

Author: Jae Yang - [dchem] (https://github.com/dchem/)

### Change Notes:
* 2014-02-18 - 0.0.1 - Initial commit

### Installation dependency
* Download Activetick cppsdk from Activetick.com website, unpack into working
  directory where you will run ```npm install activetick```.
* Install Python 2.7
* Install node-gyp
```
npm install node-gyp -g
```
* Install unzip if Linux
```
sudo apt-get install unzip
```

### Additional installation dependency for Windows:
* Install MinGW
* Install msys-unzip instead of unzip
* Install msys-wget
```
mingw-get install msys-unzip
mingw-get install msys-wget
```
* Install Microsoft Visual Studio

### Installation from NPM:
```
npm install activetick
```

For Windows with MSVS 2013:
```
npm install activetick --msvs_version=2013
```

For Windows with MSVS 2012:
```
npm install activetick --msvs_version=2012
```
Alternatively, include GYP_MSVS_VERSION=2012 or GYP_MSVS_VERSION=2013 in 
environment variables for Windows.

### Additional installation dependency for OS X:
* Install xcode command line tools
* Install homebrew
* Install wget through homebrew

### Installation from git repo:

1. Make sure to have Activetick API package downloaded to the same directory as this project
2. Run the preinstall.sh which does the following:
  * Copies the Activetick API package and places them in the import directory
  * Download libjson 7.6.1
3. ```node-gyp rebuild```
4. If build fails because you have VS2012, use ```node-gyp --msvs_version=2012 rebuild```

### Usage
TODO

## Activetick API Methods
TODO

### Tests:
TODO

### Issues:
* See issues list in https://github.com/dchem/node-activetick-addon/issues?state=open

### License
Copyright (c) 2015 Jae Yang. See LICENSE file for license rights and limitations (MIT).
