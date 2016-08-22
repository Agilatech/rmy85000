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
#include "DataManip.h"

#ifdef DEBUG
#  define DPRINT(x) do { std::cerr << x; std::cerr << std::endl; } while (0)
#else
#  define DPRINT(x) do {} while (0)
#endif


static const std::string name = "RMY85000";
static const std::string version = "0.8.0";

static const int RMY85000_READ_DELAY = 400; // milliseconds
static const int RMY85000_CALIBRATION_MULTIPLIER = 1.5;

static const int numValues = 2;

static const std::string valueNames[numValues] = {"speed", "direction"};
static const std::string valueTypes[numValues] = {"float", "integer"};

class Rmy85000Drv {
    
public:
    Rmy85000Drv(std::string devfile);
    
    static std::string getVersion();
    static std::string getRmy85000();
    static int getNumValues();
    static std::string getTypeAtIndex(int index);
    static std::string getNameAtIndex(int index);
    
    bool isActive();
    std::string getValueAtIndex(int index);
    
protected:
    
    bool initialize();
    std::string readValue0();
    std::string readValue1();
    
private:
    
    bool readSerial();
    void sendString(unsigned char *string, int numChars);
    
    bool active = false;
    
    const char *serialDevice;
    char receiveBuffer[100];
    int serialFile;
    
};

#endif /* defined(__Rmy85000Drv__) */
