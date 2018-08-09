# reference site to know what to do
#http://www.pythoncentral.io/introduction-to-sqlite-in-python/

#MAKE A UNIQUE THREAD/ PROCESS FOR EVERY CONNECTION TO THE IC

import sqlite3
import sys
import os

class database():
    """docstring for database"""

    def __init__(self, clearDB):
        try:
            if not os.path.exists('data/IC_Info.sqlite'):
                os.mkdir('data')
                os.mknod('data/IC_Info.sqlite')
            # Creates or opens a file called mydb with a SQLite3 DB
            self.db = sqlite3.connect('data/IC_Info.sqlite', check_same_thread=False)#isolation_level="Immediate", , check_same_thread = False))
            # Get a cursor object
            self.cursor = self.db.cursor()
            # Check if table users does not exist and create it
            if clearDB:
                try:
                    self.cursor.execute("DROP TABLE IF EXISTS IC_Info")
                    self.db.commit()
                    self.cursor.execute("CREATE TABLE IC_Info(IP_Addr TEXT PRIMARY KEY, App INTEGER, Status TEXT, PID INTEGER)")
                    self.db.commit()
                except:
                    # Roll back any change if something goes wrong
                    self.db.rollback()
        # Catch the exception
        except Exception as e:
            print("Couldn't connect to database.")
            raise e

    def __del__(self):
        self.db.close()

    def updateStatus(self, data):
        try:
            self.cursor.execute("UPDATE IC_Info SET status = ? WHERE IP_Addr = ?", data)
            self.db.commit()
        except Exception as e:
            self.db.rollback()
            print("Failed to update database. Rolling back the state...", e)

    def addIC(self, data):
        try:
            self.cursor.execute("INSERT INTO IC_Info VALUES(?, ?, ?, ?)", data)
            self.db.commit()
        except Exception as e:
            self.db.rollback()
            print("Failed to add to the database. Rolling back the state...", e)

    def getICs(self):
        self.cursor.execute("SELECT * FROM IC_Info")
        return self.cursor.fetchall()
        
    def getIC(self, App):
        self.cursor.execute("SELECT IP_Addr FROM IC_Info where App = ?", App)
        return self.cursor.fetchall()

    def printDB(self):
        self.cursor.execute("SELECT * FROM IC_Info")
        for row in self.cursor.fetchall():
            print(row)

    def getFailed(self):
        self.cursor.execute("SELECT * FROM IC_Info WHERE Status = 'FAILED'")
        return self.cursor.fetchall()

    def getStatus(self, app):
        self.cursor.execute("SELECT Status FROM IC_Info WHERE App = ?", app)
        return self.cursor.fetchall()
    
    def getICStatus(self, IPAddr):
        self.cursor.execute("SELECT Status FROM IC_Info WHERE IP_Addr = ?", IPAddr)
        return self.cursor.fetchall()
        
    def inDatabase(self, IPAddr):
        try:
            self.cursor.execute("SELECT * FROM IC_Info WHERE IP_Addr = ?", IPAddr)
            IC = self.cursor.fetchone()
        except:
            IC = None
        return IC
    
    def removeIC(self, IC):
        try:
            self.cursor.execute("DELETE FROM IC_Info WHERE IP_ADDR = ?", IC)
            self.db.commit()
        except Exception as e:
            self.db.rollback()
            print("Failed to remove from the database. Rolling back the state...", e)

    def setPID(self, data):
        try:
            self.cursor.execute("UPDATE IC_Info SET PID = ? WHERE IP_Addr = ?", data)
            self.db.commit()
        except Exception as e:
            self.db.rollback()
            print("Failed to set PID for application. Rolling back the state...", e)

