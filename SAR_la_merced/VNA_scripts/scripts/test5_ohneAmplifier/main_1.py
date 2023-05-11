#!/home/soporte/anaconda3/bin/python3

import serial
import time
from pandas.core.api import DataFrame
import vna as v
import h5py
import pandas as pd
import os.path
import os

#connect to the VNA:
m = v.vnaClient()
m.connect()

#Establish serial connection with arduino:

ser = serial.Serial('/dev/ttyACM0',115200,timeout=1)
ser.flushOutput()
print('Serial connected')

#Initializing variables:
i=0
r=0

#Establish path to save data
root_path="/home/sar/Documents/VNA_scripts/scripts/test5_ohneAmplifier/November_2022/"

arr = os.listdir(root_path)
numbers=[]
for filename in arr:
    numbers.append(int(filename[5:-3]))
print(numbers)
numbers.sort()
if len(numbers)==0:
    i=0
else:
    max=numbers[-1]
    i=max+1

while True:
    line = ser.readline()
    if line:
        string = line.decode()  # convert the byte string to a unicode string
        r = int(string) # convert the unicode string to an int

    print(r)
    
    if(r==5):

        m.send_idn()
        m.send_ifbw(ifbw=1000)
        m.send_number_points(points=300)
        m.send_freq_start(freq_start=15) # GHz
        m.send_freq_stop(freq_stop=15.5) # 15.5 GHz
        m.send_power(power=-5)
        m.send_cfg()
        data = m.send_sweep()

        file_exists = os.path.exists(root_path + f"dset_{i}.h5")

        df=pd.DataFrame([data])

        if(file_exists==False):
            df.to_hdf(root_path + f'dset_{i}.h5','data') 
        else:
            hf_save=pd.read_hdf(root_path + f'dset_{i}.h5')
            #n_columns=len(hf_save.columns)-1
            #hf_save.insert(0,n_columns+1,df)
            hf_save=pd.concat([hf_save,df])
            hf_save.reset_index(drop=True,inplace=True)
            # hf = h5py.File("real_data_meassure_11.hdf5", 'w')    
            hf_save.to_hdf(root_path + f'dset_{i}.h5','data')                                                                                                                                                                                                                                                                                                                                                                                                 
        #print(len(data))
        #hf.create_dataset("dataset", data=data)
        #hf.close()
       # p=p+1
    else:
        if(r==7):
            i=i+1
            df=pd.DataFrame([data])
            df=df.iloc[0:0]
            df.to_hdf(root_path + f'dset_{i}.h5','data') 

m.close()