"""
***************************************************************
                        API - VNA
***************************************************************
Notes:
    > VNA: Copper Mountain S5180
    > SCPI commands
    > Socket protocol
    > Enable the option to work with sockets in VNA Software, by
      pressing the following softkeys:
      System -> Misc Setup -> Network Setup -> Socket Server -> ON
"""
import time
import socket
import sys
import string
import os
import numpy as np

HOST = '127.0.0.1' # Local IP
PORT = 5025 # Port number
IFBW_VALUES = [10, 20, 50, 100, 200, 500, 1000, 2000, 5000,
               10000, 20000, 50000, 100000]
BUFFER_LENGTH = 100 #100

SCPI_IDN = b"*IDN?\r\n"
SCPI_RESET = b"*RST\r\n"
SCPI_NUMBER_OF_POINTS = b":SENS:SWE:POIN %d\r\n"
SCPI_IFBW = b"SENS:BWID %d\r\n"
SCPI_FREQ_START = b":SENS:FREQ:STAR %.3fGHZ\r\n"
SCPI_FREQ_STOP = b":SENS:FREQ:STOP %.3fGHZ\r\n"
SCPI_POWER = b"SOUR:POW %.2fdBm\r\n"

SCPI_TRACE_NUMBER = b":CALC:PAR:COUN %d\r\n" # Sets the number of traces
SCPI_TRACE_PARAMETER = b":CALC1:PAR%d:DEF %s\r\n" # Selects the measurement parameter of the trace, i.e. S-parameter
SCPI_SELECT_TRACE = b"CALC1:PAR%d:SEL\r\n"
SCPI_DISPLAY = b":DISP:WIND:SPL %d\r\n" # Sets the layout of the graph in the current channel
SCPI_TRACE_FORMAT = b":CALC1:SEL:FORM %s\r\n" # e.g. MLOG, PHAS, REAL, IMAG, etc

SCPI_TRIG_SOURCE = b":TRIG:SOUR BUS\r\n"
SCPI_CONT_OFF = b":INIT1:CONT OFF\r\n"
SCPI_INIT_ON = b"INIT1\r\n"
SCPI_TRIG_SINGLE = b":TRIG:SING\r\n"
SCPI_OPC = b"*OPC?\r\n"
SCPI_READ_DATA = b"CALC:TRAC%d:DATA:SDAT?\r\n"

class vnaClient():
    def __init__(self, host = None, port = None):
        if host is None:
            host = HOST
        if port is None:
            port = PORT
        self.host = host
        self.port = port

    def connect(self):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        try:
            self.s.connect((self.host, self.port))
        except socket.error:
                self.s.close()
                self.s = None

        if self.s is None:
            print("could not establish connection to the vna")
        else:
            print("connection to the vna established")

    def send(self, data):
        self.error = 0

        try:
            self.s.send(data)
        except socket.error:
            self.error = -1

        if self.error < 0:
            print("could not send instruction to the vna")
        #else:
        #    print 'Data sent: %s\n' % data

    def recv(self, buffer_len = None):
        if buffer_len is None:
            recv_flag = False
            buffer_len = BUFFER_LENGTH
        else:
            recv_flag = True
            buffer_len = buffer_len
        self.buffer_len = buffer_len
        self.data = ''
        aux = ''
        flag = False
        x = ''

        self.data = self.s.recv(buffer_len)
        """
        while 1:
            aux = self.s.recv(1)
            if aux:
                for x in aux:
                    self.data += x
                    if recv_flag:
                        if len(self.data) == buffer_len:
                            flag = True
                            break
                    else:
                        if len(self.data) == buffer_len or x == '\n':
                            flag = True
                            break
            else:
                break

            if flag:
                break
        """
        return self.data

    def send_idn(self):
        self.send(data = SCPI_IDN)
        print(self.recv())

    def send_ifbw(self, ifbw = None):
        if ifbw is None:
            ifbw = IFBW_VALUES[1]
        else:
            if ifbw in IFBW_VALUES:
                ifbw = ifbw
            else:
                print("ifbw not valid")
                return
        self.ifbw = ifbw
        self.send(data = SCPI_IFBW %self.ifbw)

    def send_number_points(self, points = None):
        if points is None:
            points = 1601
        else:
            if 2 <= points <= 200000:
                points = points
            else:
                print("number of points out of range [2-200000]")
                return
        self.points = points
        self.send(data = SCPI_NUMBER_OF_POINTS %self.points)

    def send_freq_start(self, freq_start = None): # Units: GHz
        if freq_start is None:
            freq_start = 15.5
        else:
            if 12.4 <= freq_start <= 18.0:
                freq_start = freq_start
            else:
                print("frequency out of range [12.4-18.0 GHz]")
                return
        self.freq_start = freq_start
        #print freq_start
        self.send(data = SCPI_FREQ_START %self.freq_start)

    def send_freq_stop(self, freq_stop = None):
        if freq_stop is None:
            freq_stop = 16.5
        else:
            if 12.4 <= freq_stop <= 18.0:
                freq_stop = freq_stop
            else:
                print("frequency out of range [12.4-18.0 GHz]")
                return
        self.freq_stop = freq_stop
        #print freq_stop
        self.send(data = SCPI_FREQ_STOP %self.freq_stop)

    def send_power(self, power = None): # Units: dBm
        if power is None:
            power = 0
        else:
            if -40 <= power <= 10: # (dBm)
                power = power
            else:
                print("invalid power value [-40 ; +10]dBm")
                return
        self.power = power
        self.send(data = SCPI_POWER %self.power)

    def send_cfg(self):
        #DISPLAY CONFIGURATION
        #Two traces on screen for the s21 parameter:
        #First for the Real value, and the other one for the Imag value
        self.send(data = SCPI_TRACE_NUMBER %2) # Number of traces

        self.send(data = SCPI_DISPLAY %2) # Allocate traces
        self.send(data = SCPI_TRACE_PARAMETER %(1, b"S21"))  # Trace: 1
        self.send(data = SCPI_SELECT_TRACE %1)
        self.send(data = SCPI_TRACE_FORMAT %(b"REAL")) # Options: MLOG, PHAS, REAL, IMAG
        self.send(data = SCPI_TRACE_PARAMETER %(2, b"S21")) # Trace: 2
        self.send(data = SCPI_SELECT_TRACE %2)
        self.send(data = SCPI_TRACE_FORMAT %(b"IMAG"))

        self.send(data = SCPI_TRIG_SOURCE)
        self.send(data = SCPI_CONT_OFF)

        print("vna succesfuly configured!")

    def send_sweep(self):
        # Put channel 1 to the trigger waiting state
        self.send(data = SCPI_INIT_ON)
        # Starts sweeping in channel 1
        self.send(data = SCPI_TRIG_SINGLE)
        # Waits for the end of the sweep
        while True:
            self.send(data = SCPI_OPC)
            x = self.recv()
            # print(x)
            if x == b"1\n":
                break
        x = None
        # Reads out measurement data(complex data) in Trace 1
        self.send(data = SCPI_READ_DATA %1)
        size_element = 18 # 18 is the number of digits of a single element
        LEN = size_element*(2*self.points) # 2 due to the data type: complex(R+jI)
        DATA_AUX = self.recv(LEN)
        # print("Data last characters are: ", DATA[-1])
        DATA = ""
        while DATA_AUX[-1]!=10: # \n character
            # print('Yes!')
            DATA += DATA_AUX.decode("utf-8")
            DATA_AUX = self.recv(LEN)
        DATA += DATA_AUX.decode("utf-8")

        # else:
        #     DATA_FIN = DATA
        #     print('No!')
        #DATA2 = self.recv(LEN)
        #DATA3 = self.recv(LEN)
        # print("Len of data is: " + str(self.points))
        # print("Len of Data1 is: ", len(DATA))
        # print("Data1 is: ")
        # print(DATA)
        # print("Len of Data2 is: ", len(DATA_AUX))
        # print("Data2 is: ")
        # print(DATA_AUX)
        # # print("Data3 is: ")
        # print(DATA3)
        # Converts byte data to string data
        # DATA = DATA.decode("utf-8")
        # Converts string data to array complex 64 data
        data_array = np.fromstring(DATA, dtype = float, sep=',')
        data_array = list(data_array)
        complex_data_array = np.array(data_array[::2])+1j*np.array(data_array[1::2])
        complex_data_array.astype(np.complex64)
        #
        # print("Array data is: ")
        # print(complex_data_array)
        print("sweep sent and data received")
        #
        return complex_data_array

    def close(self):
        self.s.close()
        print('connection to the vna closed')
