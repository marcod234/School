#Author - Alberto Olvera
from datetime import datetime, timedelta

plzFail = False
timeout = timedelta(minutes=30)#Change to 3

#libraries
import smbus
import HTU21DF
#sound
import pygame
import sys
#light
import time

from PIL import Image
from PIL import ImageDraw

from Adafruit_LED_Backpack import Matrix8x8

# Import the ADS1x15 module.
import Adafruit_ADS1x15

#Example using a character LCD backpack.
import Adafruit_CharLCD as LCD


def light():
    display = Matrix8x8.Matrix8x8()

    # Initialize the display. Must be called once before using the display.
    display.begin()
    

    display.clear()

    image = Image.new('1', (8, 8))
    draw = ImageDraw.Draw(image)
    draw.rectangle((0,0,7,7), outline=255, fill=0)
    draw.rectangle((1,1,6,6), outline=255, fill=0)
    draw.rectangle((2,2,5,5), outline=255, fill=0)
    draw.rectangle((3,3,4,4), outline=255, fill=0)

    display.set_image(image)
    display.write_display()
    time.sleep(1)

    for x in range(8):
        for y in range(8):
            display.clear()
            display.set_pixel(x,y , 0)
            display.write_display()
    display.clear()

#co2
# Get I2C bus
bus = smbus.SMBus(1)

address = 0x58

bus.write_i2c_block_data(address, 0x20, [0x03])
time.sleep(1)

bus.write_i2c_block_data(address, 0x20, [0x08])
time.sleep(1)


#sound
pygame.init()
pygame.mixer.init()


# Define    LCD column and row size for 16x2   LCD.
lcd_columns = 16
lcd_rows    = 2

# Initialize the  LCD using the pins
lcd = LCD.Adafruit_CharLCDBackpack()

# Turn backlight on
lcd.set_backlight(0)

# Create an ADS1115 ADC (16-bit) instance.
adc = Adafruit_ADS1x15.ADS1115()

#Data ready event enabled for altitude, pressure, temperature
bus.write_byte_data(0x60, 0x13, 0x07)

time.sleep(.5)
# Active mode, OSR = 128, Barometer mode
bus.write_byte_data(0x60, 0x26, 0x39)
time.sleep(.5)
#co2 reading
#bus.write_i2c_block_data(address, 0x20, [0x08])
#time.sleep(1)

GAIN = 1
    
#main

start = datetime.now()
end = start + timeout
while True:
    curr = datetime.now()
    if curr > end and plzFail:
        print('error')
        break
    value2= 8500
    #time.sleep(.5)
    print("sending reset...")
    # Print a two line message
    lcd.message('Sending reset...')
    time.sleep(1)
    lcd.clear()

    #temperature
    HTU21DF.htu_reset
    temperature = HTU21DF.read_temperature()
    tempf = (temperature * 1.8) + 32
    time.sleep(.5)
   
    #humidity
    humidity = HTU21DF.read_humidity()
    time.sleep(.5)
    # status, pres MSB1, pres MSB, pres LSB
    data = bus.read_i2c_block_data(0x60, 0x00, 4)
    # Convert the data to 20-bits
    pres = ((data[1] * 65536) + (data[2] * 256) + (data[3] & 0xF0)) / 16
    pressure = (pres / 4.0) / 1000.0
   
    #smoke
    value = adc.read_adc(3, gain=GAIN)
    value = value/10
   
    #co2
    bus.write_i2c_block_data(address, 0x20, [0x08])
    time.sleep(1)
    data1 = bus.read_i2c_block_data(address, 0)
    new = data1[0] * 400
   
    #oxygen
    value2  = adc.read_adc_difference(0, gain=GAIN)

    volt = value2/2048*4.096
    volt = volt/201*10000
    voltn = volt/7.43
    voltn = voltn/10

    if tempf < 80 and humidity < 70 and pressure <  100 and value < 400 and voltn > 16 and new < 30000:
        #if all conditions are safe
        output = round(tempf,2)
        print("Temperature: %fF" % output)
        lcd.message('Temperature:\n')
        lcd.message(str(output)+ 'F')         #lcd.set_color(RED)
        time.sleep(1.9)
        lcd.clear()

        print("Humidity: %.2F%%" % humidity)
        lcd.message('Humidity:\n')
        output = round(humidity,2)
        lcd.message(str(output) + '%')
        time.sleep(1.9)
        lcd.clear()

        print "Pressure: %.2f kPa" %pressure
        lcd.message('Pressure:\n')
        output = round(pressure,2)
        lcd.message(str(output)+ 'kPa')
        time.sleep(1.9)
        lcd.clear()

        print "CO2: %.2f ppm" %new
        lcd.message('CO2:\n')
        output = round(new,2)
        lcd.message(str(output)+ 'ppm')
        time.sleep(1.9)
        lcd.clear()

        print "Oxygen: %.2f ppm" %voltn
        lcd.message('Oxygen level:\n')
        output = round(voltn,2)
        lcd.message(str(output)+ '%')
        time.sleep(1.9)
        lcd.clear()

        print "Smoke: ", value
        time.sleep(1.9)

    elif tempf > 80:
        print("TOO HOT")
        pygame.mixer.music.load("VIEC/evac-trim.wav")
        pygame.mixer.music.play(0 )
        output = round(tempf,2)
        lcd.message('TOO HOT\n')
        light()
        lcd.message(str(output)+ 'F')
        time.sleep(1.75)
        lcd.clear()

    elif humidity > 70:
        print("HUMID")
        pygame.mixer.music.load("VIEC/bom-trim.wav")
        pygame.mixer.music.play(0 )
        #       light()
        lcd.message('TOO HUMID\n')
        light()
        output = round(humidity,2)
        lcd.message(str(output) + '%')
        time.sleep(1.75)
        lcd.clear()

    elif pressure > 100:
        print("High pressure")
        pygame.mixer.music.load("VIEC/weather-trim.wav")
        pygame.mixer.music.play(0 )
        lcd.message('HIGH PRESSURE:\n')
        light()
        output = round(pressure,2)
        lcd.message(str(output)+ 'kPa')
        time.sleep(1.75)
        lcd.clear()

    elif new > 30000:
        print("HIgh Levles of CO2")
        pygame.mixer.music.load("VIEC/weather-trim.wav")
        pygame.mixer.music.play(0 )
        lcd.message('CO2 Levels\nToo High')
        light()
        output = round(new,2)
        lcd.message(str(output)+ 'ppm')
        time.sleep(1.75)
        lcd.clear()

    #still needs work
    elif value > 400:  #lightlevel 220-400, medium level 400-600, intense level >600
        print("Smoke Detected")
        pygame.mixer.music.load("VIEC/fire-trim.wav")
        pygame.mixer.music.play(0 )
        lcd.message('SMOKE\nDETECTED')
        light()
        time.sleep(1.75)
        lcd.clear()

    elif voltn < 16 :
        print("dangerous lveles")
        pygame.mixer.music.load("VIEC/Siren-trim.wav")
        pygame.mixer.music.play(0 )
        lcd.message('Oxygen Level\nTOO LOW')
        light()
        time.sleep(1.75)
        lcd.clear()

