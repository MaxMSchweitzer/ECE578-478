import SocketServer

class MyUDPHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        data = self.request[0].strip()
        socket = self.request[1]
        print "{} wrote:".format(self.client_address[0])
        print data

if __name__ == "__main__":
    print "Server for this jimmy has started"
    HOST, PORT = "127.0.0.1", 3100
    server = SocketServer.UDPServer((HOST, PORT), MyUDPHandler)
    server.serve_forever()

