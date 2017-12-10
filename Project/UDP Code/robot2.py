import SocketServer

class MyUDPHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        data = self.request[0].strip()
        socket = self.request[1]
        print "{} wrote:".format(self.client_address[0])
        print data

if __name__ == "__main__":
    print "Server for this jimmy has started"
    HOST, PORT = "192.168.0.102", 3200
    server = SocketServer.UDPServer((HOST, PORT), MyUDPHandler)
    server.serve_forever()
