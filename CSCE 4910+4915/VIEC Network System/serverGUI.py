#author - Marco Duarte

#!/usr/bin/python3

from tkinter import *               # gui  library
from tkinter.ttk import Notebook, Style, Button   #importing some classes from Tkinter extension
import tkinter.messagebox
import time
import random
from enum import IntEnum
from PIL import Image, ImageTk #Python Imaging Library
import IC_Database
from multiprocessing import Process, Queue
import time
import os, signal

class cStatus(IntEnum):
  OFFLINE = 0
  RUNNING = 1
  CAUTION = 2
  FAILED  = 3

class cGUI:
  def __init__(self): #accept a q for the Queue Object
    self.database = IC_Database.database(False)

    #initial IC statuses
    self.mS1 = cStatus.OFFLINE
    self.mS2 = cStatus.OFFLINE
    self.mS3 = cStatus.OFFLINE

    self.toShutdown = ['offline', 'offline', 'offline']

    #Root frame
    self.mRoot = Tk()
    self.mNote = Notebook(self.mRoot) #notebook used to create tabs

    #Gif Frames
    self.frames =[PhotoImage(file="pi.gif", format="gif -index %i" %(i)) for i in range(80)]
    self.idx1 = 0
    self.idx2 = 0
    self.idx3 = 0

    self.mRoot.title("VIEC Network System Status")  #set title
    self.mRoot.attributes("-zoomed", True)          #start in fullscreen

    #Placing Notebook and create tab frames
    self.mNote.grid(row=0, column=0)
    self.mStatusTab = Frame(self.mNote)
    self.mHelpTab = Frame(self.mNote)

    #add tabs to notebook
    self.mNote.add(self.mStatusTab, text="IC Statuses")
    self.mNote.add(self.mHelpTab, text="Help")
    self.mRoot.attributes("-alpha", 0)      #set window as transparent
    self.mRoot.update()                     #displays window
    self.mOffset = self.mNote.winfo_reqheight() #size of the tabs

    self.height, self.width = self.getScreenSize()  #get usable screen size
    self.mUsable = self.height - (2*self.mOffset) #size for status tab = screen size - tab size

############### STATUS TAB #####################################################################
    #setting column and row sizes for Status tab
    self.mStatusTab.grid_rowconfigure(0, weight=1, minsize=(self.mUsable/2))
    self.mStatusTab.grid_rowconfigure(1, weight=1, minsize=(self.mUsable/2))
    self.mStatusTab.grid_columnconfigure(0, weight=1, minsize=(self.width))

    #splitting status tab into two frames
    self.mTopFrame = Frame(self.mStatusTab, width=self.width, height=(self.mUsable/2))
    self.mTopFrame.grid(row=0)
    self.mTopFrame.grid_rowconfigure(0, weight=1, minsize=(self.mUsable/2))
    self.mTopFrame.grid_columnconfigure(0, weight=1, minsize=(self.width/2))
    self.mTopFrame.grid_columnconfigure(1, weight=1, minsize=(self.width/2))

    self.mBottomFrame = Frame(self.mStatusTab, width=self.width, height=(self.mUsable/2))
    self.mBottomFrame.grid(row=1)
    self.mBottomFrame.grid_rowconfigure(0, weight=1, minsize=(self.mUsable/2))
    self.mBottomFrame.grid_columnconfigure(0, weight=1, minsize=self.width)

    #IC Frames
    self.mIC1 = Frame(self.mTopFrame, width=(self.width/2), height=(self.mUsable/2))
    self.mIC1.grid(row=0, column=0)
    self.mIC1.grid_columnconfigure(0, weight=1, minsize=(self.width/2))

    self.mIC2 = Frame(self.mTopFrame, width=(self.width/2), height=(self.mUsable/2))
    self.mIC2.grid(row=0, column=1)
    self.mIC2.grid_columnconfigure(0, weight=1, minsize=(self.width/2))

    self.mIC3 = Frame(self.mBottomFrame, width=self.width, height=(self.mUsable))
    self.mIC3.grid(row=0, column=0)
    self.mIC3.grid_columnconfigure(0, weight=1, minsize=self.width)

    #IC Styles
    self.s = Style()
    self.s.configure("offline.TButton", font=(None, 15), pady=3, foreground='black', state=DISABLED)
    self.s.map('offline.TButton',foreground=[('disabled', 'black')])
    self.s.configure("runnning.TButton", font=(None, 15), pady=3, foreground='green', state=NORMAL)
    self.s.configure("caution.TButton", font=(None, 15), pady=3, foreground='yellow4', state=NORMAL)
    self.s.configure("failed.TButton", font=(None, 15), pady=3, foreground='red', state=NORMAL)

    #add IC1 widgets
    self.mIC1Label = Label(self.mIC1, text="Habitat Lighting System", font=(None, 15), pady=5)
    self.mIC1Label.grid(row=0, column=0)

    self.mIC1ImageLabel = Label(self.mIC1, image=self.frames[0], width=128, height=128)
    self.mIC1ImageLabel.grid(row=1)

    self.mIC1Status = Button(self.mIC1, text="Offline", style='offline.TButton', state=DISABLED, command=lambda app=1: self.shutdownIC(app))
    self.mIC1Status.grid(row=2)

    #add IC2 widgets
    self.mIC2Label = Label(self.mIC2, text="Environment Monitoring System",
                           font=(None, 15),pady=5)
    self.mIC2Label.grid(row=0, column=0)

    self.mIC2ImageLabel = Label(self.mIC2, image=self.frames[0], width=128, height=128)
    self.mIC2ImageLabel.grid(row=1)

    self.mIC2Status = Button(self.mIC2, text="Offline", style='offline.TButton', state=DISABLED, command=lambda app=2: self.shutdownIC(app))
    self.mIC2Status.grid(row=2, column=0)

    #add IC3 widgets
    self.mIC3Label = Label(self.mIC3, text="Reaction Control System", font=(None, 15), pady=5)
    self.mIC3Label.grid(row=0, column=0)

    self.mIC3ImageLabel = Label(self.mIC3, image=self.frames[0], width=128, height=128)
    self.mIC3ImageLabel.grid(row=1)

    self.mIC3Status = Button(self.mIC3, text="Offline", style='offline.TButton', state=DISABLED, command=lambda app = 3: self.shutdownIC(app))
    self.mIC3Status.grid(row=2)

############### HELP TAB #######################################################################

    self.mHelpTab.grid_columnconfigure(0, weight=1, minsize=self.width)
    self.mHelpTab.grid_rowconfigure(0, weight=1, minsize=self.mUsable/8)
    self.mHelpTab.grid_rowconfigure(1, weight=1, minsize=3*self.mUsable/8)
    self.mHelpTab.grid_rowconfigure(2, weight=1, minsize=1*self.mUsable/8)

    self.help =PhotoImage(file="help.gif", format="gif")

    self.mHelpTitleLabel = Label(self.mHelpTab, text="How to Replace ICs", font=(None, 20), pady=5)
    self.mHelpTitleLabel.grid(row=0, column=0)

    self.mHelpImageLabel = Label(self.mHelpTab, image=self.help, width=300, height=225)
    self.mHelpImageLabel.grid(row=1)

    self.mHelpInsLabel = Label(self.mHelpTab, text="1. Remove the Ethernet, Power, and Universal Connector\
    \n 2. Plug in new IC                                                                        "\
    , font=(None, 15), pady=5)
    self.mHelpInsLabel.grid(row=2, column=0)

############### START GUI #######################################################################

    self.mRoot.after(25, self.updateGif)
    self.mRoot.after(1000, self.update)
    self.mRoot.protocol("WM_DELETE_WINDOW", self.onClosing)
    self.shutdown = False

  def onClosing(self):
    self.shutdown = True
    tkinter.messagebox.showinfo("System Shutdown", "Shutting down... Please wait.")

  def getScreenSize(self):
    height = self.mRoot.winfo_height()      #get current height of screen
    width  = self.mRoot.winfo_width()       #get current width of screen 
    self.mRoot.attributes("-alpha", 1)      #set window as opaque
    return (height, width)

  def update(self):
    self.mS1 = self.setStatus(self.mS1, self.mIC1Status, (1,))
    self.mS2 = self.setStatus(self.mS2, self.mIC2Status, (2,))
    self.mS3 = self.setStatus(self.mS3, self.mIC3Status, (3,))
    self.mRoot.after(1000, self.update)

  def updateGif(self):    
    status1 = self.database.getStatus((1,))
    if len(status1) != 0 and (status1[0][0] == 'RUNNING' or status1[0][0] == 'CAUTION'):
        self.idx1 = (self.idx1 + 1) % 80 
        self.mIC1ImageLabel.configure(image=self.frames[self.idx1])

    status2 = self.database.getStatus((2,))
    if len(status2) != 0 and (status2[0][0] == 'RUNNING' or status2[0][0] == 'CAUTION'):
        self.idx2 = (self.idx2 + 1) % 80 
        self.mIC2ImageLabel.configure(image=self.frames[self.idx2])
    
    status3 = self.database.getStatus((3,))
    if len(status3) != 0 and (status3[0][0] == 'RUNNING' or status3[0][0] == 'CAUTION'):
        self.idx3 = (self.idx3 + 1) % 80 
        self.mIC3ImageLabel.configure(image=self.frames[self.idx3])

    self.mRoot.after(35, self.updateGif)

  def setStatus(self, oldS, label, app):
    #this is where the database will be read
    status = (self.database.getStatus(app))

    if len(status) != 0:
      if status[0][0] == 'RUNNING':   newS = cStatus.RUNNING
      elif status[0][0] == 'CAUTION': newS = cStatus.CAUTION
      elif status[0][0] == 'FAILED' : newS = cStatus.FAILED
      else: raise Exception
    else: 
      newS = cStatus.OFFLINE
      self.toShutdown[app[0]-1] = 'offline'
      
    if oldS != newS:
        self.setStatusString(newS, label)
    return newS

  def setStatusString(self, s, label):
    if s == cStatus.OFFLINE:
      label.configure(text="Offline", state=DISABLED, style='offline.TButton') #fg="black",
    elif s == cStatus.RUNNING:
      label.configure(text="Running", state=NORMAL, style='runnning.TButton')#fg="green",
    elif s == cStatus.CAUTION:
      label.configure(text="Not Responding", state=NORMAL, style='caution.TButton')#, fg="yellow4")
    else:
      label.configure(text="Failed", state=NORMAL, style='failed.TButton')#, fg="red")

  def shutdownIC(self, app): #function to trigger a shutdown sequence for a particular IC
    self.toShutdown[app-1] = 'true'
    #print('changed',app)

	
def GUI(guiQ):
  gui = cGUI()
  # gui.mRoot.after(33, gui.updateGif)
  # gui.mRoot.after(2000, gui.update)
  #gui.mRoot.mainloop()  
  while True:
    #gui.mRoot.update_idletasks()
    gui.mRoot.update()
    if gui.shutdown:
        #guiQ.put("stop!")
        os.kill(os.getppid(), signal.SIGINT)

'''def shutdown(signum, frame, p):
  p.terminate()
  p.join()
   print("Exited via signal")
  os.kill(os.getpid(), signal.SIGKILL)
  print('parent dead')
'''

if __name__ == '__main__':
    q = Queue()
    q.cancel_join_thread()

    p = Process(target=GUI, args=(q,))
    p.start()

    #signal.signal(signal.SIGINT, lambda signum, frame: shutdown(signum, frame, p))

    print("I'm lit!!!")
    start = time.time()
    while True:
        if not q.empty():
            m = q.get_nowait()
            if m == "stop!":
                break
        # print("I'm still going")
        curr = time.time()
        if curr-start >= 300:
            p.terminate()
            p.join()
            break

    print('done!')
