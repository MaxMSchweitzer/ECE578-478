import SocketServer
import subprocess

class MyUDPHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        data = self.request[0].strip()
        socket = self.request[1]
        print "{} wrote:".format(self.client_address[0])
        print data
	
	# Wait for the command to be sent over, then write it to a file and callable
	# motion script with that file as a command line argument.
	# This tells the robot to do whatever is in the text file,  following the format
	# of motion script.
	posture_file = open("posture_file.txt", "w")
	posture_file.write(str(data))
	posture_file.close()
	subprocess.Popen(["./project", "./posture_file.txt"])
		

if __name__ == "__main__":
    print "Server for this jimmy has started"
	# This will need to be changed to whatever your robot's IP address is
    HOST, PORT = "192.168.1.101", 3100
    server = SocketServer.UDPServer((HOST, PORT), MyUDPHandler)
    server.serve_forever()
	
    print "Hello world"










#import socket
#
#def Main():
#    host = 'localhost'
#    port = 5000
#
#    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#    s.bind((host,port))
#
#    print "Server Started."
#    while True:
#        data, addr = s.recvfrom(1024)
#        print "message From: " + str(addr)
#        print "from connected user: " + str(data)
#    c.close()
#
#if __name__ == '__main__':
#    Main()



