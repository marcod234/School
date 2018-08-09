'''
Richard Ervin Jr.
Purpose: This gets the IP address of all connected ICs and 
distributes stored applications to each IC
'''

import sys, os
import subprocess
import paramiko
from datetime import datetime, timedelta
from os.path import isfile, join

def getIPAddrs():
    IP = []
    with subprocess.Popen(["sudo", "arp-scan", "--interface=eth0", "--localnet"], stdout=subprocess.PIPE) as proc:
        (out, err) = proc.communicate()
        out = out.decode('utf-8')
        out = out.split('\n')
        for line in out:
            if 'Raspberry' in line and 'DUP' not in line:
                line = line.split()
                IP.append(line[0])
    return IP
        
class connection():
    def __init__(self, database):
        self.database = database
        self.applications = sorted([f for f in os.listdir(".") if "App" in f])
        '''
            App 1 = Lit
            App 2 = Env
            App 3 = Rea
        '''
        #print('Program Intialization done!!!')

    def connected(self):
        IPs = getIPAddrs()
        added = []
        if len(IPs) > 0:
            for IP in IPs:
                #print(IP, " - ", IPs)
                try:
                    client = paramiko.SSHClient()
                    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
                    client.connect(hostname=IP, username='pi', password='kramyeet')
                    sftp = client.open_sftp()
                    client.exec_command("sudo pigpiod")
                    
                    status = self.database.getICStatus((IP,))
                    if len(status) == 0:
                        #print("New IC:", IP)
                  
                        try:
                            sftp.put("position.py", "position.py")
                            stdin, stdout, stderr = client.exec_command("python position.py", get_pty=True)
                            for line in iter(stdout.readline, ""):
                                line = line.rstrip()
                                print(IP,'is connected to system')
                                print(IP, 'sent', line)
                                try:
                                    app = int(line)
                                except:
                                    app = 0
                                    break
                            print ("application:", app)
                            
                            if app > 0 and app < 4:
                                sftp.put("runApplication.py", "runApplication.py")
                                sftp.put(self.applications[app-1], self.applications[app-1])

                                IC = (IP, app, "RUNNING", 0)
                                #self.database.addIC(IC)
                                added.append(IC)
                            else:
                                print("bad application")
                        except Exception as e2:
                               print("Couldn't deploy or start application",e2)
                except:
                   print("Can't connect to IC at IP Address:", IP)
        return added

    def removeFailed(self):
        IPs = getIPAddrs()
        failed = self.database.getFailed()
        #print('IPs:', IPs)
        #print('failed:', failed)
        if len(failed) > 0:
            for IC in failed:
                #print('evaluating', IC[0])
                if IC[0] not in IPs:
                    self.database.removeIC((IC[0],))
                    curr = datetime.now()
                    print(curr.strftime('%m/%d/%y\t%H:%M:%S') + ' Shutdown complete for ' + IC[0])

    #https://www.saltycrane.com/blog/2010/02/python-paramiko-notes/
    def notOutHere(self, sftp):
        """Return True if the remote path exists"""
        try:
            sftp.stat("~/We_Out_Here")
        except IOError as e:
            if 'No such file' in str(e):
                return False
            raise
        else:
            return True

if __name__ == '__main__':
    database = IC_Database()
    con = connection(database)
    con.connected()
