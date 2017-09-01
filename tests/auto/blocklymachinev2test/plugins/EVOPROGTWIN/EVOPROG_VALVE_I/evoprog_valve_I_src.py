from routingValve import RoutingValve

class EVOPROG_VALVE_I(RoutingValve):
	
	def __init__(self, params):
		"""constructor"""
		self.address = params["i2c_address"];

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
		communications.sendString("MOVE " + str(self.address) + " " + str(position))
	
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
		communications.sendString("MOVE " + str(self.address) + " " + "HOME")