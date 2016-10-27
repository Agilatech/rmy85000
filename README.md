##Node addon for the RM Young 85000 Ultrasonic Anemometer

#####This addon should work on any Linux platform, and has been thoroughly tested on BeagleBone Black

###Install

```
npm install @agilatech/rmy85000
```

OR

```
git clone https://github.com/Agilatech/rmy85000.git
node-gyp configure build
```

###Usage
#####Load the module and create and instance
```
const addon = require('rmy85000');

// create an instance on the /dev/ttyO2 serial device file
const rmy85000 = new addon.Rmy85000('/dev/ttyO2');
```
#####Get basic device info
```
const name = rmy85000.rmy85000();  // returns string with name of device
const version = rmy85000.deviceVersion(); // returns this software version
const active = rmy85000.deviceActive(); // true if active, false if inactive
const numVals =  rmy85000.deviceNumValues(); // returns the number of paramters sensed
```
####Get paramter info and values
```
const paramName0 = rmy85000.nameAtIndex(0);
const paramType0 = rmy85000.typeAtIndex(0);
const paramVal0  = rmy85000.valueAtIndexSync(0);

const paramName1 = rmy85000.nameAtIndex(1);
const paramType1 = rmy85000.typeAtIndex(1);
const paramVal1  = rmy85000.valueAtIndexSync(1);
```
####Asynchronous value collection is also available
```
rmy85000.valueAtIndex(0, function(err, val) {
    if (err) {
        console.log(err);
    }
    else {
        console.log(`Asynchronous call return: ${val}`);
    }
});
```

###Operation Notes
This driver is specifc to the RM Young 85000 series ultrasonic anemometer.  The hardware measures wind speed from 
0-60 m/s with an accuracy of ±0.1 m/s. Wind direction is measured from 0-360 degrees with an accuracy of ±3 degrees.

The hardware interfaces via RS-232 or RS-485, so your hardware must support one of these protocols.  The serial 
device file is specified in the constructor.

###Dependencies
* node-gyp

###Copyright
Copyright © 2016 Agilatech. All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

