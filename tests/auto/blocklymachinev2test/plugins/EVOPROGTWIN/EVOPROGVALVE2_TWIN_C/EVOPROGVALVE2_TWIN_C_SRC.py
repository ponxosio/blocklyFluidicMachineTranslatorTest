import time
from routingValve import RoutingValve

class EVOPROGVALVE2_TWIN_C(RoutingValve):
	
	def __init__(self, params):
		"""constructor"""
		self.i2cAddress = params['i2c_address'];
		self.actualPosition = 0;

	def moveToPosition(self, communications, position):
		"""
			must send instructions to the machine to move this valve to the position "position",
			the command must be sended throught the communications object,
			communications object has the next api:
				*) nbytessend sendString(string) -- send the string to the machine, return the bytes sended;
				*) string receiveString() -- receive and returns a string from the machine (stops when \n is received), can block;
				*) string readUntil(endCharacter) -- returns a string received from the machine, stops when the endCharacter arrives;
				*) void synchronize() -- synchronize with the machine;
		"""
		if (position != self.actualPosition) :
			if (position == 0) :
				self.closeValve(communications);
			else :
				positionsMove = abs(self.actualPosition - position);
			
				communications.sendString("M " + str(self.i2cAddress) + " " + str(position));
				time.sleep(.400 * positionsMove);
				self.actualPosition = position;
	
	def closeValve(self, communications):
		"""
			must send instructions to the machine to close this valve to position "position",
			the command must be sended throught the communications object,
			communications object has the next api:
				*) nbytessend sendString(string) -- send the string to the machine, return the bytes sended;
				*) string receiveString() -- receive and returns a string from the machine (stops when \n is received), can block;
				*) string readUntil(endCharacter) -- returns a string received from the machine, stops when the endCharacter arrives;
				*) void synchronize() -- synchronize with the machine;
		"""
		communications.sendString("H " + str(self.i2cAddress));
		time.sleep(3);
		self.actualPosition = 0;