#Author - Richard Ervin
import time
import sys, os, pty
import subprocess
from multiprocessing import Process, Event, Queue

import socket, ssl

def application(q, app):
    master, slave = pty.openpty()
    with subprocess.Popen(["python", "App" + app + ".py"], stdout=slave) as app:
        stdout = os.fdopen(master)
        start = time.time()
        #print('start', start)
        for line in iter(stdout.readline, ""):
            now = time.time()
            print('App: ' +line)
            if 'traceback' in line.lower() or "error" in line.lower():
                q.put(line + " Failure!")
                break
            else: q.put("All's well!")
            time.sleep(1)
    q.put('done executing')
    exit(0)

if __name__ == '__main__': #accept flag stating that the app should fail after a certain amount of time
    if len(sys.argv) > 3:
        try:
            server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            ssl_sock = ssl.wrap_socket(server,cert_reqs=ssl.CERT_NONE)
            ssl_sock.connect((sys.argv[2], socket.htons(int(sys.argv[3]))))
        except Exception as e:
            print('no connection', e)
            exit(0)
            
        failedOnce = False

        with open("app.log", 'w') as log:
            q = Queue()
            app = Process(target=application, args=(q, sys.argv[1]))
            app.start()
            time.sleep(6)
            print("Just having a great time printing data! :D")
            #q.get()
            while True:
                if not q.empty():
                    message = q.get_nowait()
                    log.write('Q message - ' + message + '\n')
                    print('Q message - ' + message + '\n')
                    failedOnce = False
                    if 'done executing' in message:
                        message = "Failure due to Application Ending!"
                        failedOnce = True
                    elif "Failure!" in message:
                        message = "All's well!"
                        failedOnce = True
                elif failedOnce:
                    message = "Failure!"
                else:
                    message = "All's well!"
                    failedOnce = True
                log.write("Sending '"  + message + "' to server\n")
                print("Sending '"  + message + "' to server")
                log.write(message+'\n')
                print(message)
                ssl_sock.send(message.encode())

                #log.flush()
                time.sleep(2)
                #i += 1
            app.join()

        ssl_sock.close()
