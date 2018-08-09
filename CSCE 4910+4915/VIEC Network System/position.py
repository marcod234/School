#Author - Marco Duarte

import os
import time
import Adafruit_GPIO as GPIO
import Adafruit_GPIO.MCP230xx as MCP

class cPos():

  mDipPins = [12, 13, 14, 15]
  mPins = 4
  mPosVals = {1:"4",
              2:"3",
              4:"2",
              8:"1"}

  def __init__(self, address=0x20):
    try:
        self.mMcp=MCP.MCP23017(address)
        self.getPos()
    except Exception as ex:
        print("Error getting the position", ex)

  def getPos(self):
    self.mMcp.setup(12, GPIO.IN)
    self.mMcp.pullup(12, False)
    self.mMcp.setup(13, GPIO.OUT)
    self.mMcp.pullup(13, False)
    self.mMcp.setup(14, GPIO.IN)
    self.mMcp.pullup(14, False)
    self.mMcp.setup(15, GPIO.OUT)
    self.mMcp.pullup(15, False)

    val = self.mMcp.input(12)
    pos = val

    val = self.mMcp.input(13)
    pos += val << 1

    val = self.mMcp.input(14)
    pos += val << 2

    val = self.mMcp.input(15)
    pos += val << 3

    pos = self.mPosVals.get(pos)
    print int(pos)
    self.mMcp.setup(12, GPIO.OUT)
    self.mMcp.setup(13, GPIO.OUT)
    self.mMcp.setup(14, GPIO.OUT)
    self.mMcp.setup(15, GPIO.OUT)

cPos(0x24)

