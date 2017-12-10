from heapq import heappush, heappop # for priority queue
import math
import time
import random
import socket

class node:
    xPos = 0 # x position
    yPos = 0 # y position
    distance = 0 # total distance already travelled to reach the node
    priority = 0 # priority = distance + remaining distance estimate
    def __init__(self, xPos, yPos, distance, priority):
        self.xPos = xPos
        self.yPos = yPos
        self.distance = distance
        self.priority = priority
    def __lt__(self, other):                                                                # comparison method for priority queue
        return self.priority < other.priority
    def updatePriority(self, xDest, yDest):
        self.priority = self.distance + self.estimate(xDest, yDest) * 10 # A*
    # give higher priority to going straight instead of diagonally
    def nextMove(self, dirs, d):                                                            # d: direction to move
        if dirs == 8 and d % 2 != 0:
            self.distance += 14
        else:
            self.distance += 10
    # Estimation function for the remaining distance to the goal.
    def estimate(self, xDest, yDest):
        xd = xDest - self.xPos
        yd = yDest - self.yPos
        # Euclidian Distance
        d = math.sqrt(xd * xd + yd * yd)
        # Manhattan distance
        # d = abs(xd) + abs(yd)
        # Chebyshev distance
        # d = max(abs(xd), abs(yd))
        return(d)

# A-star algorithm.
# The path returned will be a string of digits of directions.
def pathFind(the_map, n, m, dirs, dx, dy, xA, yA, xB, yB):
    closed_nodes_map = []                                                                   # map of closed (tried-out) nodes
    open_nodes_map = []                                                                     # map of open (not-yet-tried) nodes
    dir_map = []                                                                            # map of dirs
    row = [0] * n
    for i in range(m):                                                                      # create 2d arrays
        closed_nodes_map.append(list(row))
        open_nodes_map.append(list(row))
        dir_map.append(list(row))

    pq = [[], []]                                                                           # priority queues of open (not-yet-tried) nodes
    pqi = 0                                                                                 # priority queue index
    # create the start node and push into list of open nodes
    n0 = node(xA, yA, 0, 0)
    n0.updatePriority(xB, yB)
    heappush(pq[pqi], n0)
    open_nodes_map[yA][xA] = n0.priority                                                    # mark it on the open nodes map

    # A* search
    while len(pq[pqi]) > 0:
        # get the current node w/ the highest priority
        # from the list of open nodes
        n1 = pq[pqi][0]                                                                     # top node
        n0 = node(n1.xPos, n1.yPos, n1.distance, n1.priority)
        x = n0.xPos
        y = n0.yPos
        heappop(pq[pqi])                                                                    # remove the node from the open list
        open_nodes_map[y][x] = 0
        closed_nodes_map[y][x] = 1                                                          # mark it on the closed nodes map

        # quit searching when the goal is reached
        # if n0.estimate(xB, yB) == 0:
        if x == xB and y == yB:
            # generate the path from finish to start
            # by following the dirs
            path = ''
            while not (x == xA and y == yA):
                j = dir_map[y][x]
                c = str((j + dirs / 2) % dirs)
                path = c + path
                x += dx[j]
                y += dy[j]
            return path

        # generate moves (child nodes) in all possible dirs
        for i in range(dirs):
            xdx = x + dx[i]
            ydy = y + dy[i]
            if not (xdx < 0 or xdx > n-1 or ydy < 0 or ydy > m - 1
                    or the_map[ydy][xdx] == 1 or closed_nodes_map[ydy][xdx] == 1):
                                                                                            # generate a child node
                m0 = node(xdx, ydy, n0.distance, n0.priority)
                m0.nextMove(dirs, i)
                m0.updatePriority(xB, yB)
                                                                                            # if it is not in the open list then add into that
                if open_nodes_map[ydy][xdx] == 0:
                    open_nodes_map[ydy][xdx] = m0.priority
                    heappush(pq[pqi], m0)
                                                                                            # mark its parent node direction
                    dir_map[ydy][xdx] = (i + dirs / 2) % dirs
                elif open_nodes_map[ydy][xdx] > m0.priority:
                                                                                            # update the priority
                    open_nodes_map[ydy][xdx] = m0.priority
                                                                                            # update the parent direction
                    dir_map[ydy][xdx] = (i + dirs / 2) % dirs
                                                                                            # replace the node
                                                                                            # by emptying one pq to the other one
                                                                                            # except the node to be replaced will be ignored
                                                                                            # and the new node will be pushed in instead
                    while not (pq[pqi][0].xPos == xdx and pq[pqi][0].yPos == ydy):
                        heappush(pq[1 - pqi], pq[pqi][0])
                        heappop(pq[pqi])
                    heappop(pq[pqi])                                                        # remove the target node
                                                                                            # empty the larger size priority queue to the smaller one
                    if len(pq[pqi]) > len(pq[1 - pqi]):
                        pqi = 1 - pqi
                    while len(pq[pqi]) > 0:
                        heappush(pq[1-pqi], pq[pqi][0])
                        heappop(pq[pqi])       
                    pqi = 1 - pqi
                    heappush(pq[pqi], m0)                                                   # add the better node instead
    return ''                                                                               # if no route found


#Draw circles based on start x and y of center and radius
def DrawCircle(StartX, StartY, radius, n, m, the_map):
    for i in range (StartX - radius, StartX + radius):
        for j in range (StartY - radius, StartY + radius):
            if ((i - StartX)* (i - StartX) + (j - StartY)*(j - StartY) <= radius * radius and ((i >= 0) and (j >= 0) and (i <= n-1) and (j <= m-1))):    
                the_map[i][j] = 1


def draw_map(the_map,n,m):
    print 'Map:'
    for y in range(m):
        for x in range(n):
            xy = the_map[y][x]
            if xy == 0:
                print '.', # space
            elif xy == 1:
                print 'O', # obstacle
            elif xy == 2:
                print 'S', # start
            elif xy == 3:
                print 'R', # route
            elif xy == 4:
                print 'F', # finish
        print

#UDP client code for sending command
def Client_send(msm_string):
    host = '127.0.0.1'
    port = 3100

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.sendto(msm_string + "\n", (host, port))
    s.close()


def CreateCommandTarget(Dir,Com_Del,Com_List):
    global count
    msm = ""
    if not Com_Del:
        Com_Del.append(Dir)                             #list is empty, add to list [cur, prev]
        return
    elif len(Com_Del) == 1:
        Com_Del.insert(0,Dir)
    else:
        Com_Del.insert(0,Dir)
        Com_Del.pop(2)

    print Com_Del
    c = Com_Del[0]
    p = Com_Del[1]
    if ( c == p ):
        count += 1
        msm = "1"
        print "MoveForward"
    elif ( p - c == 1 and c != 0):
        count += 1
        print "Turn left 45, Move forward"
        msm = "2 45"                          
    elif ( p - c == 2 and c != 0):
        count += 1
        print "Turn left 90, Move forward"
        msm = "2 90"                          
    elif ( p - c == 3 and c != 0):
        count += 1
        print "Turn left 135, Move forward"
        msm = "2 135" 
    elif ( c - p == 1 and c != 0):
        count += 1
        print "Turn right 45, Move forward"
        msm = "2 -45"                          
    elif ( c - p == 2 and c != 0):
        count += 1
        print "Turn right 90, Move forward"
        msm = "2 -90"                          
    elif ( c - p == 3 and c != 0):
        count += 1
        print "Turn right 135, Move forward"
        msm = "2 -135" 
    elif ( c == 0 and p == 7) or (p == 0 and c == 1):
        count += 1
        print "Turn right 45, Move forward"
        msm = "2 -45"
    elif ( c == 0 and p == 6) or (p == 0 and c == 2):
        count += 1
        print "Turn right 90, Move forward"
        msm = "2 -90"
    elif ( c == 0 and p == 5) or (p == 0 and c == 3):
        count += 1
        print "Turn right 135, Move forward"
        msm = "2 -135"
    elif ( c == 0 and p == 1) or (p == 0 and c == 7):
        count += 1
        print "Turn left 45, Move forward"
        msm = "2 45"
    elif ( c == 0 and p == 2) or (p == 0 and c == 6):
        count += 1
        print "Turn left 90, Move forward"
        msm = "2 90"
    elif ( c == 0 and p == 3) or (p == 0 and c == 5):
        count += 1
        print "Turn left 135, Move forward"
        msm = "2 135"

    #print count
    print msm
    Client_send(msm)                                       #send commands to UDP delegation




        
# MAIN
def Main():
    global count
    count = 0
    dirs = 8                                                                # number of possible directions to move on the map
    if dirs == 4:
        dx = [1, 0, -1, 0]
        dy = [0, 1, 0, -1]
    elif dirs == 8:
        dx = [1, 1, 0, -1, -1, -1, 0, 1]
        dy = [0, 1, 1, 1, 0, -1, -1, -1]


    # 0 = right, 1 = down right , 2 = down, 3 = down-left , 4 = left, 5 = up-left, 6 = up, 7 = Up-right
    n = 50                                                                 # horizontal size of the map
    m = 50                                                                 # vertical size of the map
    the_map = []
    row = [0] * n
    CommandDelegate = []
    CommandList = ['MOVE_FORWARD', 'MOVE_TURNLEFT', 'MOVE_TURNRIGHT', 'MOVE_BACK', 'WALK_READY', 'WALK_SLOW', 'WALK_FAST']
    for i in range(m):                                                      # create empty map
        the_map.append(list(row))

    #draw obstacles if any.  
    DrawCircle(15, 15, 5, n, m, the_map)

    # Defined start location for robot and destination
    global xA
    global yA
    (xA, yA) = (40, 5)          # robot position
    (xB, yB) = (7, 15)          # Final Destination

    print 'Map size (X,Y): ', n, m
    print 'Start: ', xA, yA
    print 'Finish: ', xB, yB

    #while (xA != xB and yA != yB):
    # Actual Running A Star Algorithm
    t = time.time()
    route = pathFind(the_map, n, m, dirs, dx, dy, xA, yA, xB, yB)
    print 'Time to generate the route (seconds): ', time.time() - t
    print 'Route:'
    print route
    
    # mark the route on the map
    if len(route) > 0:
        x = xA
        y = yA
        the_map[y][x] = 2
        for i in range(len(route)):
            j = int(route[i])
            x += dx[j]
            y += dy[j]
            the_map[y][x] = 3
            #print 'currnt direction:' , j
            CreateCommandTarget(j,CommandDelegate,CommandList) 
        the_map[y][x] = 4

        draw_map(the_map,m,n)
        # This will be new robot position fed in from the kinect
        #xA -= 1
        #yA += 1
            

if __name__=="__main__":
    Main()
