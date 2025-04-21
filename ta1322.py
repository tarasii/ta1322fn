import wiringpi
import sys


def set_freq(addr, freq):
	tmp = freq * 8 + 3836
	fd = wiringpi.wiringPiI2CSetupInterface("/dev/i2c-2", addr)
	dt = [tmp>>8, tmp & 0xff, 0xCE, 0]
	print(dt)
	wiringpi.wiringPiI2CWrite(fd, dt[0])
	wiringpi.wiringPiI2CWrite(fd, dt[1])
	wiringpi.wiringPiI2CWrite(fd, dt[2])
	wiringpi.wiringPiI2CWrite(fd, dt[3])

def get(addr):
	fd = wiringpi.wiringPiI2CSetupInterface("/dev/i2c-2", addr)
	res = wiringpi.wiringPiI2CRead(fd)
	print(res)

def ScanDev():
	found = False;
	for x in range(128):
		fd = wiringpi.wiringPiI2CSetupInterface("/dev/i2c-2", x)
		#print(x, fd)
		if (fd < 0): continue
		
		res = wiringpi.wiringPiI2CRead(fd)
		#print(x, res)
		
		if (res < 0): continue
	
		print(f"0x{x:02x} found")
		found = True
		
	if not found:
		print("No device found")
	

x = int(sys.argv[1])
print(f"Частота : {x} MHz", );
    
get(0x61)
set_freq(0x60, x)
get(0x61)

#ScanDev()
