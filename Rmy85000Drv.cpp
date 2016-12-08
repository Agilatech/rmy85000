/**
 * \file Rmy85000Drv.cpp
 *
 *  Created by Scott Erholm on 8/22/16.
 *  Copyright (c) 2016 Agilatech. All rights reserved.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "Rmy85000Drv.h"

Rmy85000Drv::Rmy85000Drv(std::string devfile) {
    
    this->serialDevice = devfile.c_str();
    
    if (initialize()) {
        this->active = true;
    }
    else {
        std::cerr << name << " did not initialize. " << name << " is inactive" << std::endl;
    }
    
}

std::string Rmy85000Drv::getVersion() {
    return name + " " + version;
}

std::string Rmy85000Drv::getDeviceName() {
    return name;
}

std::string Rmy85000Drv::getDeviceType() {
    return type;
}


int Rmy85000Drv::getNumValues() {
    return numValues;
}

std::string Rmy85000Drv::getTypeAtIndex(int index) {
    if ((index < 0) || (index > (numValues - 1))) {
        return "none";
    }
    
    return valueTypes[index];
}

std::string Rmy85000Drv::getNameAtIndex(int index) {
    if ((index < 0) || (index > (numValues - 1))) {
        return "none";
    }
    
    return valueNames[index];
}

bool Rmy85000Drv::isActive() {
    return this->active;
}

std::string Rmy85000Drv::getValueByName(std::string name) {
    
    for (int i = 0; i < numValues; i++) {
        if (name == valueNames[i]) {
            return this->getValueAtIndex(i);
        }
    }
    
    return "none";
}

std::string Rmy85000Drv::getValueAtIndex(int index) {
    
    if (!this->active) {
        return "none";
    }

    if (index == 0) {
        return this->readValue0();
    }
    else if (index == 1) {
        return this->readValue1();
    }
    else {
        return "none";
    }
    
}

bool Rmy85000Drv::initialize() {
    
    if ((this->serialFile = open(this->serialDevice, O_RDWR | O_NOCTTY | O_NDELAY)) < 0 ) {
        std::cerr << "RMY85000 : Failed to open serial device file " << this->serialDevice << std::endl;
        return false;
    }
    
    struct termios options;
    tcgetattr(this->serialFile, &options);
    
    // Set up the communications options:
    // 9600 baud, 8-bit, enable receiver, no modem control lines
    options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
    options.c_iflag = IGNPAR | ICRNL;    //ignore partity errors, CR -> newline
    tcflush(this->serialFile, TCIFLUSH);             //discard file information not transmitted
    tcsetattr(this->serialFile, TCSANOW, &options);  //changes occur immmediately
    
    // Now set parameters on the device
    unsigned char tripleEsc[4] = "\x1B\x1B\x1B";
    unsigned char operate[4] = "XX\n";
    unsigned char rs232[9] = "SET0102\n";
    unsigned char polled[9] = "SET0203\n";
    unsigned char units[9] = "SET0404\n";
    unsigned char damping[9] = "SET0905\n";
    unsigned char sample[9] = "SET1230\n";
    unsigned char direction[9] = "SET1300\n";
    
    // Setting ESC x 3 ///
    sendString(tripleEsc, 3);
    
    usleep(RMY85000_READ_DELAY * 1000);
    if (!readSerial()) { return false; }
    
    // Setting RS-232 output ///
    sendString(rs232, 8);
    
    usleep(RMY85000_READ_DELAY * 1000);
    if (!readSerial()) { return false; }
    
    // Setting ASCII POLLED output ///
    sendString(polled, 8);
    
    usleep(RMY85000_READ_DELAY * 1000);
    if (!readSerial()) { return false; }
    
    // Setting Wind Speed Units ///
    sendString(units, 8);
    
    usleep(RMY85000_READ_DELAY * 1000);
    if (!readSerial()) { return false; }
    
    // Setting Damping ///
    sendString(damping, 8);
    
    usleep(RMY85000_READ_DELAY * 1000);
    if (!readSerial()) { return false; }
    
    // Setting Sample Rate ///
    sendString(sample, 8);
    
    usleep(RMY85000_READ_DELAY * 1000);
    if (!readSerial()) { return false; }
    
    // Setting Direction Mode ///
    sendString(direction, 8);
    
    usleep(RMY85000_READ_DELAY * 1000);
    if (!readSerial()) { return false; }
    
    // Sending Operation CMD ///
    sendString(operate, 3);
    usleep(RMY85000_READ_DELAY * 1000);
    
    // Sometimes the device will send an acknoledgment to the operate command,
    // but sometimes not.  This is difficult to handle the different cases, so
    // instead we will simply flush both read and write from the tty.
    // This somewhat reasonable assumption is that if all the previous reads
    // succeeded, then the device is operating correctly now.
    tcflush(this->serialFile, TCIOFLUSH);
    
    return true;
}

std::string Rmy85000Drv::readValue0() {
    
    if (!this->active) {
        return "none";
    }
    
    float speed = 0;
    
    unsigned char poll[4] = "M0!";
    sendString(poll, 3);
    usleep(RMY85000_READ_DELAY * 1000);
    
    if (readSerial()) {
        
        std::string buff(this->receiveBuffer);
        
        // Output is a ww.w ddd ss*cc<CR> where 'ww.w' is speed and ddd is direction
        speed = std::stof(buff.substr(2,4), nullptr) * RMY85000_CALIBRATION_MULTIPLIER;
        
    }
    else {
        std::cerr << "RMY85000 : Failed to read from serial device file " << this->serialDevice << std::endl;
    }

    return DataManip::dataToString(speed, 1);
}

std::string Rmy85000Drv::readValue1() {
    
    if (!this->active) {
        return "none";
    }
    
    uint16_t dir = 0;
    
    unsigned char poll[4] = "M0!";
    sendString(poll, 3);
    usleep(RMY85000_READ_DELAY * 1000);
    
    if (readSerial()) {
        
        std::string buff(this->receiveBuffer);
        
        // Output is a ww.w ddd ss*cc<CR> where 'ww.w' is speed and ddd is direction
        dir = std::stoi(buff.substr(7,3), nullptr, 10);
        
        // Handle the case that the direciton is 360
        if (dir > 359) { dir = 0; }
        
    }
    else {
        std::cerr << "RMY85000 : Failed to read from serial device file " << this->serialDevice << std::endl;
    }
    
    return DataManip::dataToString(dir);
}

bool Rmy85000Drv::readSerial() {
    
    long count;
    
    if ((count = read(this->serialFile, (void*)this->receiveBuffer, 100)) < 0) {
        return false;
    }
    else {
        this->receiveBuffer[count] = 0;
    }
    return true;
}

// This function is completely ridiculous but necessary.  The RMY85000 cannot
// read a string sent to it at the stated baud.  Therefore, we have to send one
// character at a time with a delay in between.
void Rmy85000Drv::sendString(unsigned char *string, int numChars) {
    for (int i = 0; i < numChars; i++) {
        write(this->serialFile, string+i, 1);
        usleep(1000);
    }
}



