/**
 * \file Rmy85000Drv.h
 *
 *  Created by Scott Erholm on 8/22/16.
 *  Copyright (c) 2016 Agilatech. All rights reserved.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __Rmy85000Drv__
#define __Rmy85000Drv__

#include <iostream>
#include <fstream>
#include <termios.h>
#include <fcntl.h>
#include <string>
#include <unistd.h>
#include <thread>
#include <cmath>
#include "Device.h"
#include "DataManip.h"

class Rmy85000Drv : public Device {
    
public:
    Rmy85000Drv(std::string devfile, float calibration = 1.75);
    virtual std::string getValueAtIndex(int index);
    virtual void setCalibration(float cal);
    
    static const int NUM_VALUES = 6;
    
protected:
    
    bool initialize();
    std::string readValue0();
    std::string readValue1();
    std::string readValue2();
    std::string readValue3();
    std::string readValue4();
    std::string readValue5();
    
private:
    
    // Create an array of read functions, so that multiple functions can be easily called
    typedef std::string(Rmy85000Drv::*readValueType)();
    readValueType readFunction[NUM_VALUES] = { &Rmy85000Drv::readValue0, &Rmy85000Drv::readValue1,
                                               &Rmy85000Drv::readValue2, &Rmy85000Drv::readValue3,
                                               &Rmy85000Drv::readValue4, &Rmy85000Drv::readValue5};
    
    // The serial device in the RMY85000 seems slow to respond, below the stated baud.
    // So, we add this extra read delay before trying to read a response
    static const int RMY85000_READ_DELAY = 550; // milliseconds
    
    static const int RMY85000_REFRESH_RATE = 1000; // milliseconds
    
    // Multiplies the raw output by this factor to achieve a calibrated output.
    float calibrationMultiplier = 1.75;

    bool readSerial();
    void sendString(unsigned char *string, int numChars);
    void processSerialStream();
    
    const char *serialDevice;
    char receiveBuffer[100];
    int serialFile;
    
    float speed, avg_speed, gust_speed;
    uint16_t dir, avg_dir, gust_dir;
    
};

#endif /* defined(__Rmy85000Drv__) */
