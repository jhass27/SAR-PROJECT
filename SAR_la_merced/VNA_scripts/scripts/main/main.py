#!/home/soporte/anaconda3/bin/python3

import vna2 as v
import h5py

m = v.vnaClient()
m.connect()
m.send_idn()
m.send_ifbw(ifbw=1000)
m.send_number_points(points=100)
m.send_freq_start(freq_start=15) # GHz
m.send_freq_stop(freq_stop=15.5) # GHz
m.send_power(power=-5)
m.send_cfg()

# Save dataset as HDF5 file
data = m.send_sweep()
hf = h5py.File("data_real_pequeantenas_meassure1.hdf5", 'w')
print(len(data))
hf.create_dataset("dataset", data=data)
hf.close()

m.close()
