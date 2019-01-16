## Node driver for the RM Young 85000 Ultrasonic Anemometer

##### This driver should work on any Linux platform, and has been thoroughly tested on BeagleBone Black / Debian

### Install

```
npm install @agilatech/rmy85000
```

OR

```
git clone https://github.com/Agilatech/rmy85000.git
node-gyp configure build
```

### Usage
##### Load the module and create an instance
The constructor accepts two arguments, but provides default values if one or both are omitted.

arg0 is the serial device file, which defaults to '/dev/ttyS0'
arg1 is the speed calibration multiplier, which defaults to 1.75
```
const driver = require('@agilatech/rmy85000');

// create an instance accepting defualt values 
const rmy85000_1 = new driver.Rmy85000();

// create an instance on the /dev/ttyO2 serial device file with default calibration
const rmy85000_2 = new driver.Rmy85000('/dev/ttyO2');

// create an instance on the /dev/ttyS4 serial device file with specified calibration
const rmy85000_3 = new driver.Rmy85000('/dev/ttyS4', 2.15);
```


##### Get basic device info
```
const name = rmy85000.deviceName();  // returns string with name of device
const type = rmy85000.deviceType();  // returns string with type of device
const version = rmy85000.deviceVersion(); // returns this software version
const active = rmy85000.deviceActive(); // true if active, false if inactive
const numVals =  rmy85000.deviceNumValues(); // returns the number of parameters sensed
```


#### Get parameter info and values
```
const paramName0 = rmy85000.nameAtIndex(0);
const paramType0 = rmy85000.typeAtIndex(0);
const paramVal0  = rmy85000.valueAtIndexSync(0);

const paramName1 = rmy85000.nameAtIndex(1);
const paramType1 = rmy85000.typeAtIndex(1);
const paramVal1  = rmy85000.valueAtIndexSync(1);

// ... and so on ...

const paramName1 = rmy85000.nameAtIndex(5);
const paramType1 = rmy85000.typeAtIndex(5);
const paramVal1  = rmy85000.valueAtIndexSync(5);
```


#### Asynchronous value collection is also available
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

### Operation Notes
This driver is specifc to the RM Young 85000 series ultrasonic anemometer.  The hardware measures wind speed from 0-60 m/s with an accuracy of ±0.1 m/s. Wind direction is measured from 0-360 degrees with an accuracy of ±3 degrees.

The anemometer interfaces via RS-232, so your hardware must support this protocol.  The serial device file is specified in the constructor.

This driver returns six parameters, accounting speed and direction for instantaneous, average, and gust.
* index 0 = instantaneous wind speed
* index 1 = instantaneous wind direction
* index 2 = average wind speed
* index 3 = average wind direction
* index 4 = gust wind speed
* index 5 = gust wind direction

Technically, what is happening behind the scenes is that the driver lauches a separate thread which keeps track of the instantaneous wind speeds and directions for a period of 1 minute.  At the end of the period, it averages the values to produce average wind speed and direction. During the period it keeps track of the max speed to produce gust wind speed and direction.

The RM Young 85000 is capable of many different types of communication, and setting of several parameters, but this drive uses only serial RS-232, ASCII 9600 baud, m/s speed units, damping factor 5, and 30 sample counts. You are certainly welcome to clone the code and muck about with the parameter settings. Perhaps one day many of these parameters will be settable through an options object, but don't hold your breath.


### Dependencies
* node-gyp is used to configure and build

### Copyright
Copyright © 2017-2018 Agilatech. All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

