#Author - Marco Duarte

#!/usr/bin/python

import os
import time
import Adafruit_GPIO as GPIO
import Adafruit_GPIO.MCP230xx as MCP
import Adafruit_CharLCD as LCD

#launch class
class cLaunch:
  mKeys = ['1','2','3',
           '4','5','6',
           '7','8','9',
           '8','0','#']

  mKvals = {1: '1',
            2: '2',
            4: '3',
            8: '4',
            16: '5',
            32: '6',
            64: '7',
            128: '8',
            256: '9',
            512: '*',
            1024: '0',
            2048: '#'}


  mRowPins = [6, 5, 4, 3]
  mColumnPins = [2, 1, 0]
  mRelayPins = [11, 10, 9, 8]
  mCode = "2301#"  # "launch code"
  mRows = 4
  mCols = 3
  mKey = 12
  mRpins = 4
  mDbTime = 200 #milliseconds
  mPrev = 0
  mCurr = 0

  def __init__(self, address=0x20):
    self.mMcp = MCP.MCP23017(address) #create an object to interface with io expander
    self.mLcd = LCD.Adafruit_CharLCDBackpack()
    self.mLcd.set_backlight(0)
    self.mLcd.clear()
    self.mLcd.blink(True)

    for i in range(self.mRows):        #set pins GPA3-A6 as input pins
      self.mMcp.setup(self.mRowPins[i], GPIO.IN)
      self.mMcp.pullup(self.mRowPins[i], True)
      self.mMcp.setup(self.mRelayPins[i], GPIO.OUT)
      self.mMcp.output(self.mRelayPins[i], GPIO.HIGH)

  def readCode(self):
    tempCode = []
    self.count = 0

    while True:
        #print
      self.mCurr = time.clock()
      tot = (self.mCurr - self.mPrev) * 1000

      if(tot > self.mDbTime):
        self.mPrev = self.mCurr
        keys = self.getPins()

        if keys != 0:
          for i in range(self.mKey):
            test = keys & (1 << i)
            if test != 0:
              tempCode.append(self.getKey(keys))
              self.count += 1

          if  tempCode[self.count-1] == '#': #code finished
            s = "".join(tempCode)
            #s = "2301#"
            if s == self.mCode:
              self.mLcd.clear()
#              print "Correct code"
              self.mLcd.message("Correct code")
              #os.system("mpg123 -q rupee.mp3 &")
              time.sleep(2)

#              for i in range(self.mRpins):  #set pins GB0-B3 as output pins
#                self.mMcp.setup(self.mRelayPins[i], GPIO.OUT)
#                self.mMcp.output(self.mRelayPins[i], GPIO.LOW)

              self.mLcd.clear()
              self.mLcd.message("Opening Valve 1")
              self.mMcp.output(10, GPIO.LOW)
              time.sleep(2)

              self.mLcd.clear()
              self.mLcd.message("Opening Valve 2")
              self.mMcp.output(9, GPIO.LOW)
              time.sleep(2)

              self.mLcd.clear()
              self.mLcd.message("Opening Valve 3")
              self.mMcp.output(8, GPIO.LOW)
              time.sleep(2)

              time.sleep(20) #let water flow for a lil

              self.mLcd.clear()
              self.mLcd.message("Closing Valve 1")
              self.mMcp.output(8, GPIO.HIGH)
              time.sleep(2)

              self.mLcd.clear()
              self.mLcd.message("Closing Valve 2")
              self.mMcp.output(9, GPIO.HIGH)
              time.sleep(2)

              self.mLcd.clear()
              self.mLcd.message("Closing Valve 3")
              self.mMcp.output(10, GPIO.HIGH)
              time.sleep(2)


              self.mLcd.clear()

              return
            else:
              self.mLcd.clear()
 #             print "Incorrect code"
              self.mLcd.message("Incorrect code")
              time.sleep(2)
              self.mLcd.clear()
              tempCode =[]
              self.count = 0

  def getPins(self):
    keys = 0
    for i in range(self.mCols):      #set pins GPA0-A2 as output low pins, starting a pulse
      self.mMcp.setup(self.mColumnPins[i], GPIO.OUT)
      self.mMcp.output(self.mColumnPins[i], GPIO.LOW)
      for j in range(self.mRows):
        val = self.mMcp.input(self.mRowPins[j])
        val ^=1
        keys += (val << (j*3 + i)) #store all pressed keys in bit vector
      self.mMcp.output(self.mColumnPins[i], GPIO.HIGH)  #stop column pulse 
      self.mMcp.setup(self.mColumnPins[i], GPIO.IN)
    print "keys", keys
    return keys

  def getKey(self, x):
    print 'x', x
    var = self.mKvals.get(x)
    print 'var', var

    if self.count >= self.mLcd._cols: #shift left if chars go off screen
      self.mLcd.move_left()

    self.mLcd.write8(ord(var), True)
    return var

l = cLaunch(0x24) #create launch instance using bus address 0x24
while True:
    try:
        l.readCode()      #read code
    except Exception as e:
        print 'Error!' + str(e)
    finally:
        l.mLcd.clear()
        l.mLcd.set_backlight(1)

