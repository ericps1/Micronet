#!/usr/bin/python

# simple.py

import wx
import threading
import socket
import struct
import os
from time import sleep

ID_TX = wx.NewId()
ID_CONV = wx.NewId()

class RPi_IM(wx.Frame):

    def __init__(self, parent, title):

        # Start RPi_IM receiver in GNU Radio
        #os.system("python ./Dropbox/SDR/GNU\ Radio/RPi_IM.py")

        # Start xmit_tcp on raspberry pi
        #os.system("ssh pi@10.0.0.12 -p raspberry")
        #sleep(10)
        #os.system("sudo ./tcp_xmit/TCP_XMIT -t 0 -m 80000")
        #os.system("exit")

        # Initialize the GUI
        super(RPi_IM, self).__init__(parent, title=title, size=(260, 180))
        self.InitUI()
        self.Centre()
        self.Show()

    def InitUI(self):

        panel = wx.Panel(self, -1)

        # Menu bar
        menubar = wx.MenuBar()
        filem = wx.Menu()
        helpm = wx.Menu()		
        filem.Append(wx.ID_SAVE, '&Save')
        filem.Append(wx.ID_OPEN, '&Open')
        filem.Append(wx.ID_ANY, '&Preferences')		
        helpm.Append(wx.ID_ANY, '&Documentation')
        menubar.Append(filem, '&File')
        menubar.Append(helpm, '&Help')
        self.SetMenuBar(menubar)		
        self.SetSize((330, 310))		
        self.txt1 = "Conversation"

        # Main window widgets
        self.disp = wx.TextCtrl(panel, pos=(1, 1), size=(320, 250), style=wx.TE_READONLY | wx.TE_MULTILINE)
        self.inp = wx.TextCtrl(panel, pos=(1, 251), size=(250, 30))
        self.tx_b = wx.Button(panel, pos=(251,251), label='Send', size=(70, 30), id=ID_TX)

        # Transmit event handler
        self.Bind(wx.EVT_BUTTON, self.tx, id=ID_TX)

        # Timer for receive handler
        self.timer = wx.Timer(self)
        self.Bind(wx.EVT_TIMER, self.rx, self.timer)
        self.timer.Start(1000)

        # Socket to receive data from GNU Radio
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client_socket.connect(('127.0.0.1', 3491))
        self.client_socket.settimeout(0.01)

        # Socket to transmit data using the raspberry pi
        self.client_socket_xmit = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client_socket_xmit.connect(('10.0.0.12', 3490))

    # Timed update from socket
    def rx(self, event):
        char = 0
        # Receive characters from tcp socket and append them to the display
        for i in range(1,256-24):
            self.txt1 = self.client_socket.recv(1)
            if isinstance(self.txt1,basestring):
                if char == 0:
                    self.disp.AppendText('RX : ')

                self.disp.AppendText(self.txt1)
                char = 1

        # Reset the timer
        self.timer.Stop()
        self.timer.Start(1000)

    # Transmit input data	
    def tx(self,event):

        tx_data = self.inp.GetValue()

        # Display the sent data
        self.txt1 =  tx_data
        self.disp.AppendText('\nTX : '+self.txt1 + '\n')

        # Transmit the data
        self.client_socket_xmit.send(self.txt1)

if __name__ == '__main__':

    app = wx.App()
    RPi_IM(None, title='Raspberry Pi SDR IM')
    app.MainLoop()
