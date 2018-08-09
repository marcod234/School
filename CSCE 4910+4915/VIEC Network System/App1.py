#Author - Marco Duarte
import Adafruit_GPIO as GPIO
import Adafruit_PCA9685 as PWM
import time
import Adafruit_GPIO.MCP230xx as MCP
import Adafruit_CharLCD as LCD

def step(channel, channel2, min, max, up):
    step = int(.05*(max-min))
    t =.05 
    if up:
        curr = min
        while curr < max:
            pwm.set_pwm(channel, 0, curr)
            pwm.set_pwm(channel2, 0, curr)
            curr += step
            time.sleep(t)
    else:
        curr = max
        while curr > min:
            pwm.set_pwm(channel, 0, curr)
            pwm.set_pwm(channel2, 0, curr)
            curr -= step
            time.sleep(t)

if __name__ == '__main__':
    mcp = MCP.MCP23017(0x24) # MCP23017 creating an object of the class called MCP
    pwm = PWM.PCA9685()
    pwm.set_pwm_freq(500)
    min = 409
    max = 4095
    Flag=0 #Motion not  detected
    counter = 0

    lcd_columns = 16
    lcd_rows    = 2
    lcd = LCD.Adafruit_CharLCDBackpack()
    lcd.set_backlight(0)
    lcd.clear()
    
    detected = False

    try:
        while True:  #infinite loop
            mcp.setup(0, GPIO.IN) #configuring the pin as an input
            mcp.pullup(0, True)

            Flag = mcp.input(0)
            #    mcp.setup(0, GPIO.OUT)
            #    mcp.output(0, GPIO.LOW)
            if Flag == GPIO.HIGH:
                if not detected:
                    print("Motion Detected")
                    lcd.clear()
                    lcd.set_backlight(0)
                    lcd.message("Motion Detected")
                    step(9,10,min, max, True)
                    time.sleep(.1)
                    # lcd.clear()
                    detected = True
                else: print("No Motion Still")
            else:
                if detected:
                    lcd.clear()
                    lcd.set_backlight(0)
                    lcd.message("No Motion")
                    print("No Motion")
                    step(9,10,min, max, False)
                    time.sleep(.1)
                    detected = False
                else: print("Motion Detected Still")
            time.sleep(.1)
    except Exception as e:
        print 'Error!' + str(e)
    finally:
        if detected:
            step(9, 10, 0, max, False)
        else:
            step(9, 10, 0, min, False)
        lcd.clear()
        lcd.set_backlight(1)
