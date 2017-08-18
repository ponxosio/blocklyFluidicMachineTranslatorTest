from peristalticPump import PeristalticPump

class EVOPROGPUMP(PeristalticPump):
	
	def __init__(self, params):
		"""constructor"""
		self.i2cAddress = params['i2caddress'];
		self.actualDirection = 1;

	
	def startPumping(self, communications, direction, rate):
		"""
			must send instructions to the machine to start pumping in direction "direction" with a rate of "rate" l/s
			the command must be sended throught the communications object,
			communications object has the next api:
				*) nbytessend sendString(string) -- send the string to the machine, return the bytes sended;
				*) string receiveString() -- receive and returns a string from the machine (stops when \n is received), can block;
				*) string readUntil(endCharacter) -- returns a string received from the machine, stops when the endCharacter arrives;
				*) void synchronize() -- synchronize with the machine;
		"""
		if (rate == 0.0) :
			self.stopPump(communications);
		else :
			frequency = rate * 9.216e+6;
		
			if (direction != self.actualDirection) :
				communications.sendString("invP " + str(self.i2cAddress));
				self.actualDirection *= -1;
		
			communications.sendString("P " + str(self.i2cAddress) + " " + str(frequency));
	
	def stopPump(self, communications):
		"""
			must send instructions to the machine to stop this pump,
			the command must be sended throught the communications object,
			communications object has the next api:
				*) nbytessend sendString(string) -- send the string to the machine, return the bytes sended;
				*) string receiveString() -- receive and returns a string from the machine (stops when \n is received), can block;
				*) string readUntil(endCharacter) -- returns a string received from the machine, stops when the endCharacter arrives;
				*) void synchronize() -- synchronize with the machine;
		"""
		communications.sendString("pPump " + str(self.i2cAddress));