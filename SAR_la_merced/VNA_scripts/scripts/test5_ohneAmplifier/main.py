#!/home/soporte/anaconda3/bin/python3

import vna as v
import h5py
import pandas as pd

m = v.vnaClient()
m.connect()
m.send_idn()
m.send_ifbw(ifbw=1000)
m.send_number_points(points=300)
m.send_freq_start(freq_start=15) # GHz
m.send_freq_stop(freq_stop=15.5) # 15.5 GHz
m.send_power(power=-5)
m.send_cfg()

# Save dataset as HDF5 file
data = m.send_sweep()
df=pd.DataFrame([data])
# hf = h5py.File("real_data_meassure_11.hdf5", 'w')    
hf = df.to_hdf('./vna_amp_6_4.h5','data')                                                                                                                                                                                                                                                                                                                                                                                                 
#print(len(data))
#hf.create_dataset("dataset", data=data)
#hf.close()

m.close()
