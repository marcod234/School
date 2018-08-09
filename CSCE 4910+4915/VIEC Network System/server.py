'''
Richard Ervin Jr.
Purpose: this script gets the status from an IC's log file
'''

import time
from datetime import datetime, timedelta
import paramiko
import connection
import IC_Database
from multiprocessing import Process, Event, Queue
import serverGUI
import socket, ssl
import subprocess
import random
import os, signal, atexit

class ConnectError(Exception):
    def __init__(self, message):
        self.message = message

class Shutdown(Exception):
    pass

def connect(IPaddr):
    (client, sftp) = (None, None) 
    try:
        client = paramiko.SSHClient()
        client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        client.connect(hostname=IPaddr, username='pi', password='kramyeet')
        sftp = client.open_sftp()
        return (client, sftp) 
    except:
        raise ConnectError("Couldn't connect to" + IPaddr)
        
def getStatus(line, past, oldStatus):
    status = 'good'
    #print("Just read the following from log:", line)
    if oldStatus == 'good':
        lapse = timedelta(seconds=15)
    else:
        lapse = timedelta(minutes=1)

    log = line.split('-')
    try:
        curr = datetime.strptime(log[0], '%m/%d/%y\t%H:%M:%S')
        #print(past, curr, lapse)
        if 'Fail' in log[1]:
            status = 'fail'
            # print("Found 'Failure' in log")
        elif (past + lapse) < curr:
            if oldStatus == 'good':
                status = 'timeout'
                curr = past
            else:
                status = 'fail'
    
        return (curr, status)
    except Exception as ex:
        print("Status error", ex)
        return (None, 'fail')

def monitor(IC, c, e):#, q):
    print("Monitoring " + IC[0])
    database = IC_Database.database(False)
    shutdownNow = False
    #past = datetime.strptime("10/17/95     01:32:00", '%m/%d/%y\t%H:%M:%S')
    try:
        (client, sftp) = connect(IC[0])
        # connectionGood = client != None and sftp != None
        # if not connectionGood:
            # print("Monitor connection not good to" + IC[0])
            # database.updateStatus(("FAILED", IC[0]))
            # raise 
        # else:
        print('connected to ' + IC[0])
        #print(sftp.getcwd()) 
        state = 'good'
        c.setblocking(False)
        past = datetime.now()
        while not e.is_set():
            try:
                try:
                    line = c.recv(1024).decode()
                    #print('got line')
                    curr = datetime.now()
                    message = curr.strftime('%m/%d/%y\t%H:%M:%S') + '-' + line
                except ssl.SSLError: continue
                print('message',message)
                (past, state) = getStatus(message, past, state)
                if state == 'fail':
                    print(curr.strftime('%m/%d/%y - %H:%M:%S') + " Failure Detected in: Application " + str(IC[1]))
                    database.updateStatus(("FAILED", IC[0]))
                    #time.sleep(5)
                    break 
                elif state == 'timeout':
                    print(curr.strftime('%m/%d/%y - %H:%M:%S') + ' Timeout: App' + str(IC[1]))
                    database.updateStatus(('CAUTION', IC[0]))
                
            except Exception as ex:
                print(curr.strftime('%m/%d/%y - %H:%M:%S') + ' Error: App ' + str(IC[1]) + str(ex))
                #print(" Failure Detected in: Application " + str(IC[1]))
                database.updateStatus(("FAILED", IC[0]))
                # print(str(e))
                #print(line)
            except Shutdown: raise Shutdown
            time.sleep(2)             
    except Exception as ex:
        print(datetime.now().strftime('%m/%d/%y - %H:%M:%S') + ' Exception in Monitor App ' + str(IC[1])+ ' ' + str(ex))
        #print('Error occured in Application', IC[1]) 
    except ConnectError as ce:
        print(datetime.now().strftime('%m/%d/%y - %H:%M:%S') + " Monitor connection not good to" + IC[0])
        database.updateStatus(("FAILED", IC[0]))
    except Shutdown: 
        shutdownNow = True
        print(datetime.now().strftime('%m/%d/%y - %H:%M:%S') + " Beginning shutdown process for App " + str(IC[1]))
    finally:
        c.close()
        if sftp != None:
            sftp.close()
        if client != None:
            try:
                client.exec_command("rm We_Out_Here")
                client.exec_command("sudo killall python")
                client.exec_command("sudo killall python3")
                if e.is_set() or shutdownNow: 
                    database.removeIC((IC[0],))
                    client.exec_command('sudo shutdown now')
                    print(datetime.now().strftime('%m/%d/%y - %H:%M:%S') + " Completed shutdown process for App " + str(IC[1]))
            except Exception as ex:
                print(datetime.now().strftime('%m/%d/%y - %H:%M:%S') + " Not able to shutdown IC: " + IC[0] + ' because ' + str(ex))
            client.close()
        exit(0)

def printMessages(q):
    while not q.empty():
        print(q.get_nowait())

def GUI(guiQ, shutdownICQ, mainP):
    gui = serverGUI.cGUI()
    while True:
        gui.mRoot.update()
        if gui.shutdown:
            guiQ.put("stop!")
        if 'true' in gui.toShutdown:
            print('gui found ic to shutdown')
            for IC in range(3):
                if gui.toShutdown[IC] == 'true':
                    shutdownICQ.put(IC+1)
                    gui.toShutdown[IC] = 'offline'

def shutdownIC(shutdownICQ):
    database = IC_Database.database(False)

    while True:
        #print('try shutdown')
        if not shutdownICQ.empty():
            try:
                print('try get what app to shutdown')
                app = int(shutdownICQ.get())
                                
                IC = database.getIC((app,))
                try: (client, sftp) = connect(IC[0][0])
                except ConnectError: continue
                
                print('connected to soon to be shutdown IC')
                client.exec_command("rm We_Out_Here")
                time.sleep(.02)
                print('shutting down app')
                client.exec_command('sudo killall python')
                client.exec_command("sudo shutdown now")
                os.kill(IC[0][3], signal.SIGKILL)
                print('App', str(app), 'has been set to shutdown')
                database.removeIC((IC[0][0],))
                print(IC[0][0], 'closed')
                client.close()
                sftp.close()
            except:
                pass

def shutdownSystem(gui, ICProcesses, shutdownICProcess):#, q):
    print('Beginning System Shutdown Procedure...')
    if shutdownICProcess.is_alive():
        shutdownICProcess.terminate()
        shutdownICProcess.join()
    print('Shutdown IC Process Dead')
    for p in ICProcesses:
        if p.is_alive():
            #p.terminate()
            p.join()
    print('IC Processes dead')
    #printMessages(q)
    gui.terminate()
    gui.join()
    print("GUI dead")

def sendShutdownSignal(signum, frame):
    raise Shutdown
    
def getSvrIP():
    with subprocess.Popen(["ip", "route", "list"], stdout=subprocess.PIPE) as proc:
        (out, err) = proc.communicate()
        out = out.decode('utf-8')
        out = out.split('\n')
        for line in out:
            if 'eth0' in line:
                line = line.split()
                return line[-3]

if __name__ == '__main__':
    database = IC_Database.database(True)
    guiQ = Queue()
    shutdownICQ = Queue()
    gui = Process(target=GUI, args=(guiQ,shutdownICQ, os.getpid()))
    gui.start()

    shutdownICProcess = Process(target=shutdownIC, args=(shutdownICQ,))
    shutdownICProcess.start()
    
    conn = connection.connection(database)
    e = Event()
    ICProcesses = []
    #q = Queue()
    #q.cancel_join_thread()
    #signal.signal(signal.SIGINT, lambda *_: startShutdown)#shutdownSystem(gui, processes))
    atexit.register(shutdownSystem, gui, ICProcesses, shutdownICProcess)#, q)
    signal.signal(signal.SIGTERM, sendShutdownSignal)

    port = 101795#int(sys.argv[1])) #port provided as a cmd line arg

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #creating the socket
    bound = False
    while not bound:
        try:
            s.bind(('',socket.htons(port))) #binding to the port
            bound = True
        except OSError:
            port = random.randint(5000,15000)   #int(sys.argv[1])) #port provided as a cmd line arg
            print('bad port found now using', port)
    s.listen(10) #listening for clients
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s_ssl = ssl.wrap_socket(s, keyfile='key.pem', certfile='cert.pem', server_side=True)
    
    svrIP = getSvrIP()

    while True:
        if not guiQ.empty():
            m = guiQ.get_nowait()
            if m == "stop!":
                e.set()
                #shutdownICProcess.terminate()
                #shutdownSystem(gui, ICProcesses, shutdownICProcess)
                break

        #printMessages(q)
        
        added = conn.connected()
        for IC in added:
            try: (client, sftp) = connect(IC[0])
            except ConnectError: continue
            
            print('connected', IC[0])    
            command = "python3 runApplication.py " + str(IC[1]) + ' '+svrIP+' ' + str(port)
            client.exec_command(command)
            #print(command)
            #IC = (IP, app, "RUNNING")
            print(IC[0], 'start:', datetime.now().strftime('%m/%d/%y - %H:%M:%S'))

            c, address = s_ssl.accept() #accepting the connection
            print("accept ssl connection to",address[0])
            print(address)
            if sftp != None:
                sftp.close()
            client.close()
            print('conn closed')
            p = Process(target=monitor, args=(IC, c, e))#, q))
            p.start()
            database.addIC(IC)
            database.setPID((p.pid, IC[1]))
            ICProcesses.append(p)
            print('process started')
            
        conn.removeFailed()
        time.sleep(.6)              
