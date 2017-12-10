import SocketServer

class MyUDPHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        data = self.request[0].strip()
        socket = self.request[1]
        print "{} wrote:".format(self.client_address[0])
        print data

if __name__ == "__main__":
    print "Server for this jimmy has started"
    HOST, PORT = "127.0.0.3", 3300
    #HOST, PORT = "192.168.0.113", 3300
    server = SocketServer.UDPServer((HOST, PORT), MyUDPHandler)
    server.serve_forever()

























#def Main():
#	print "Client connection established"
#
#	#HOST, PORT = "127.0.0.1", 5000
#	HOST, PORT = "localhost",9999	
#
#	server = SocketServer.UDPServer((HOST, PORT), MyUDPHandler)
#	server.serve_forever()
	
#	HOST1, PORT1 = "127.0.0.2", 5001 
#	server1 = SocketServer.UDPServer((HOST1, PORT1), MyUDPHandler)
#	server1.serve_forever()
#	
#	HOST2, PORT2 = "127.0.0.3", 5002
#	server2 = SocketServer.UDPServer((HOST2, PORT2), MyUDPHandler)
#	server2.serve_forever()

#if __name__ == "__main__":
#	Main()	
