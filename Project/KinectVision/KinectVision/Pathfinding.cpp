//Pathfinding algorithm

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <math.h>
#include <queue>
#include <string>
#include <tuple>

using namespace std;

const int MAP_WIDTH = 100;		//horizontal test map size
const int MAP_HEIGHT = 100;		//vertical test map size
const int Direction = 8;		//number of directions we can travel in

static int closed_node_map[MAP_WIDTH][MAP_HEIGHT]; // map of closed (tried-out) nodes
static int open_node_map[MAP_WIDTH][MAP_HEIGHT];	// map of open (not-yet-tried) nodes
static int map[MAP_WIDTH][MAP_HEIGHT];				//n x m
static int Direction_map[MAP_WIDTH][MAP_HEIGHT];	//map of directionals



//directionals, this is two seperate arrays that contain pairs of directions for each map.  
//1 means move positive (right / up) 0 means no move, and -1 means negative (left, down)
//supported directions in order are:  up, diag up right , up, diag up left, left, diag down left, down, diag down right

//****************************************************************************
//*   \     |   /      0       1      2      3       4       5       6        7
//*   <--  []  -->   right, up right, up, up left, left, down left, down, down right 
//*   /     |   \
//****************************************************************************
static int DirectionX[] = { 1, 1, 0,-1,-1,-1, 0, 1 };
static int DirectionY[] = { 0, 1, 1, 1, 0,-1,-1,-1 };

//directions to commands
enum DIRECTION_COMMAND {MOVE_RIGHT, MOVE_UPRIGHT, MOVE_UP, MOVE_UPLEFT, MOVE_LEFT, MOVE_DOWNLEFT,
	MOVE_DOWN, MOVE_DOWNRIGHT, WALK_STOP, WALK_READY, WALK_SPEED_SLOW, WALK_SPEED_FAST};



class SearchNode
{
		int NodePosX;			//current position of node in X
		int NodePosY;			// current position of node being searched
		int level;				//distance traveled to reach node
		int priority;			//the priority of node
public:
		SearchNode(int xp, int yp, int d, int p)
		{
			NodePosX = xp;
			NodePosY = yp;
			level = d;
			priority = p;
		}
		int GetPriority() const { return priority; }
		int GetLevel() const { return level; }
		int getXPos() const { return NodePosX; }
		int getYPos() const { return NodePosY; }

		
		//get priority of nodes
		void updatePriority(const int & DestX, const int & DestY)
		{
			priority = level + estimate(DestX, DestY) * 10;
		}

		void nextLevel(const int & i) // i: direction
		{
			level += (Direction == 8 ? (i % 2 == 0 ? 10 : 14) : 10);
		}

		const int & estimate(const int & DestX, const int & DestY) const
		{
			static int xd, yd, d;
			xd = DestX - NodePosX;
			yd = DestY - NodePosY;
			//euclidian distance compute
			d = static_cast<int>(sqrt(xd*xd + yd*yd));
			return d;
		}
		
};

// Determine priority (in the priority queue)
bool operator<(const SearchNode & a, const SearchNode & b)
{
	return a.GetPriority() > b.GetPriority();
}


//the heart and soul of pathfinding.  Takes four arguments, which is simply the start coords, and the
//goal coords.  returns string/ map of directionals as the route 
string PathFinder(const int & StartX, const int & StartY, const int & GoalX, const int & GoalY)
{
	//make a priority queue
	static priority_queue<SearchNode> pq[2];
	static int pqIndex;
	static SearchNode* N0;			//node for Width  X
	static SearchNode* M0;			//node for height Y
	static int i, j, x, y, xdx, ydy;
	static char c;

	pqIndex = 0;

	//reset node maps
	for (y = 0; y < MAP_HEIGHT; y++)
	{
		for (x = 0; x < MAP_WIDTH; x++)
		{
			closed_node_map[x][y] = 0;
			open_node_map[x][y] = 0;
		}
	}

	//create new start node
	N0 = new SearchNode(StartX, StartY, 0, 0);
	N0->updatePriority(GoalX, GoalY);
	pq[pqIndex].push(*N0);
	open_node_map[StartX][StartY] = N0->GetPriority();

	while (!pq[pqIndex].empty())  //while not empty
	{
		//construct a new node begin searching!
		N0 = new SearchNode(pq[pqIndex].top().getXPos(), pq[pqIndex].top().getYPos(), pq[pqIndex].top().GetLevel(), pq[pqIndex].top().GetPriority());

		x = N0->getXPos();
		y = N0->getYPos();

		pq[pqIndex].pop();													//remove node from open list
		open_node_map[x][y] = 0;
		closed_node_map[x][y] = 1;											//mark it on closed map

		//terminate search when goal is reached, condition checks and produces output.
		//of directionals
		if (x == GoalX && y == GoalY)
		{
			string path = "";
			while (!(x == StartX && y == StartY))
			{
				j = Direction_map[x][y];
				c = '0' + (j + Direction / 2) % Direction;
				path = c + path;
				x += DirectionX[j];
				y += DirectionY[j];

			}

			delete N0;
			//empty leftovers
			while (!pq[pqIndex].empty()) pq[pqIndex].pop();
			return path;
		}


		//generate moves for child in all possible directions
		for (i = 0; i < Direction; i++)
		{
			xdx = x + DirectionX[i];
			ydy = y + DirectionY[i];

			//generate edge cases, like if its on edge of map
			//like x is negative (not allowed, x is out of bounds, or map is 1, or closed map is 1
			if (!(xdx < 0 || xdx > MAP_WIDTH - 1 || ydy < 0 || ydy > MAP_HEIGHT - 1 || map[xdx][ydy] == 1 || closed_node_map[xdx][ydy] == 1))
			{
				M0 = new SearchNode(xdx, ydy, N0->GetLevel(), N0->GetPriority());
				M0->nextLevel(i);															//i is direction
				M0->updatePriority(GoalX, GoalY);

				//if not in open list add to it
				if (open_node_map[xdx][ydy] == 0)
				{
					open_node_map[xdx][ydy] = M0->GetPriority();
					pq[pqIndex].push(*M0);

					Direction_map[xdx][ydy] = (i + Direction / 2) % Direction;
				}
				else if (open_node_map[xdx][ydy] > M0->GetPriority())
				{
					//update priority info
					open_node_map[xdx][ydy] = M0->GetPriority();
					//update parent direction info
					Direction_map[xdx][ydy] = (i + Direction / 2) % Direction;

					// replace the node
					// by emptying one pq to the other one
					// except the node to be replaced will be ignored
					// and the new node will be pushed in instead
					while (!(pq[pqIndex].top().getXPos() == xdx &&
						pq[pqIndex].top().getYPos() == ydy))
					{
						pq[1 - pqIndex].push(pq[pqIndex].top());
						pq[pqIndex].pop();
					}
					pq[pqIndex].pop();

					//empty the larger size pq to smaller one
					if (pq[pqIndex].size() > pq[1 - pqIndex].size())
					{
						pqIndex = 1 - pqIndex;
					}
					while (!pq[pqIndex].empty())
					{
						pq[1 - pqIndex].push(pq[pqIndex].top());
						pq[pqIndex].pop();
					}
					pqIndex = 1 - pqIndex;
					pq[pqIndex].push(*M0);

				}
				delete M0;
			}

		}
		delete N0;

	}
	return "";			//no route found
}

//prototypes
void DrawObstacle(int [], int []);
void DrawMap(string, int, int);
void DrawCircle(int, int, int);
void CreateCommandTarget(int, int [], int);
int main()
{
//build an empty map
for (int y = 0; y < MAP_HEIGHT; y++)
{
	for (int x = 0; x < MAP_WIDTH; x++)
	{
		map[x][y] = 0;
	}
}

int ObsX[4] = { 30, 75, 75, 30 };
int ObsY[4] = { 45, 45, 35, 35 };

//DrawObstacle(ObsX, ObsY);
DrawCircle(50, 50, 15);

// select the start and finish points as we receive from kinect data
// randomly select start and finish locations
srand(time(NULL));

int xA, yA, xB, yB;
switch (rand() % 8)
{
case 0: xA = 0; yA = 0; xB = MAP_WIDTH - 1; yB = MAP_HEIGHT - 1; break;
case 1: xA = 0; yA = MAP_HEIGHT - 1; xB = MAP_WIDTH - 1; yB = 0; break;
case 2: xA = MAP_WIDTH / 2 - 1; yA = MAP_HEIGHT / 2 - 1; xB = MAP_WIDTH / 2 + 1; yB = MAP_HEIGHT / 2 + 1; break;
case 3: xA = MAP_WIDTH / 2 - 1; yA = MAP_HEIGHT / 2 + 1; xB = MAP_WIDTH / 2 + 1; yB = MAP_HEIGHT / 2 - 1; break;
case 4: xA = MAP_WIDTH / 2 - 1; yA = 0; xB = MAP_WIDTH / 2 + 1; yB = MAP_HEIGHT - 1; break;
case 5: xA = MAP_WIDTH / 2 + 1; yA = MAP_HEIGHT - 1; xB = MAP_WIDTH / 2 - 1; yB = 0; break;
case 6: xA = 0; yA = MAP_HEIGHT / 2 - 1; xB = MAP_WIDTH - 1; yB = MAP_HEIGHT / 2 + 1; break;
case 7: xA = MAP_WIDTH - 1; yA = MAP_HEIGHT / 2 + 1; xB = 0; yB = MAP_HEIGHT / 2 - 1; break;
}

cout << "Map Size (X, Y): " << MAP_WIDTH << MAP_HEIGHT << endl;
cout << "Start: " << xA << "," << yA << endl;
cout << "Finish: " << xB << "," << yB << endl;

//obtain route, run clock and start pathfinder alg.  Algorithm is run
//send in the coords of start and finish, which is randomly generated for now.  
//we can change this to reflect kinect values!

clock_t start = clock();
string route = PathFinder(xA, yA, xB, yB);
if (route == "")
cout << "Empty Route generated" << endl;
clock_t end = clock();
double time_elapsed = double(end - start);
cout << "Time to calculate route (ms): " << time_elapsed << endl;
cout << "Route: " << route << endl;

//run the drawMap function
DrawMap(route, xA, yA);

getchar();				//wait for enter keypress
return(0);				//program is done when robot reaches the goal point
}

//take in Vectors to draw, perhaps the argument feed should be a list of 4 vertices list<tuple <int, int>> VerticePairs
void DrawObstacle(int xOb[], int yOb[])
{
	//map given arrays to coords of obstacles
	int A[2] = { xOb[0], yOb[0] };
	int B[2] = { xOb[1], yOb[1] };
	int C[2] = { xOb[2], yOb[2] };
	int D[2] = { xOb[3], yOb[3] };
	//if the Y coords are the same for two poinbts, draw a line between em
	if (A[1] == B[1])
	{
		for (int i = A[0]; i <= B[0]; i++)
		{	map[i][A[1]] = 1; }

	}
	if (B[0] == C[0])
	{
		for (int i = B[1]; i >= C[1]; i--)
		{ map[B[0]][i] = 1; }
	}

	if (C[1] == D[1])
	{
		for (int i = C[0]; i >= D[0]; i--)
		{ map[i][C[1]] = 1; }
	}

	if (A[0] == D[0])
	{
		for (int i = D[1]; i <= A[1]; i++)
		{ map[D[0]][i] = 1; }
	}
}

//very simple actually,  use equation of a circle points 
// (x - a)^2 + (y - b)^2 = r^2 
//start search from bottom corner, search nxn area and force check if the point satisfies the 
//above equation. seaerches from bottom left to top right.  Fills in entire circle, creating the bounding circle

void DrawCircle(int StartX, int StartY, int radius)
{
	for (int i = StartX - radius; i <= StartX + radius; i++)			//50 - 25, so 25
	{
		for (int j = StartY - radius; j <= StartY + radius; j++)		//50 - 25, so 25 .  starts at 25,25
		{
			if ((i - StartX)*(i - StartX) + (j - StartY)*(j - StartY) <= radius * radius && ((i >= 0) && (j >= 0) && (i <=MAP_WIDTH-1) && (j <=MAP_HEIGHT-1)))
			{
				map[i][j] = 1;											//if equation is satisfied AND the i and j do not exceed boundaries of matrix, write 1
			}
		}
	}

}


//draws a map.  Now this isnt useful to robots but is extremely useful to testing phases
void DrawMap(string route, int xA, int yA)
{
	//follow route and display it
	if (route.length() > 0)
	{
		int j; char c;
		int x = xA;
		int y = yA;
		int CommandListJ1[MAP_WIDTH];

		map[x][y] = 2;												//basically set the start coordinates, 2 is the marker.
		for (int i = 0; i < route.length(); i++)
		{
			c = route.at(i);
			j = c - '0';
			x = x + DirectionX[j];
			y = y + DirectionY[j];
			map[x][y] = 3;											//3 is marker for ROUTE
			CreateCommandTarget(j, CommandListJ1, i);					//send number to createcommand
		}
		map[x][y] = 4;												//mark finish
	}
	// display the map with the route, iterate through matrix
	// 0 is blank, 1 is obstacle, 2 is start, 3 is route, 4 is finish
	for (int y = 0; y< MAP_HEIGHT; y++)
	{
		for (int x = 0; x< MAP_WIDTH; x++)
			if (map[x][y] == 0)
				cout << ".";
			else if (map[x][y] == 1)
				cout << "O"; //obstacle
			else if (map[x][y] == 2)
				cout << "S"; //start
			else if (map[x][y] == 3)
				cout << "R"; //route
			else if (map[x][y] == 4)
				cout << "F"; //finish
			cout << endl;
	}
}

//so take in the directional, its gonna be one of the 8 directions we go in
//I say we correspond that to a direction in the command, and have a handful of other commands 
//that are appended to them to form a complete move.  
void CreateCommandTarget(int Directional, int CommandList[], int CommandOrder)
{
	int CommandEnumerator;

	switch (Directional)
	{
	case 0: 
		//DIRECTION_COMMAND::MOVE_RIGHT; 
		if (CommandOrder )
		CommandList[CommandOrder] = CommandEnumerator;
		break;
	case 1: 
		//DIRECTION_COMMAND::MOVE_UPRIGHT; 
		CommandList[CommandOrder] = CommandEnumerator;
		break;
	case 2: 
		//DIRECTION_COMMAND::MOVE_UP; 
		CommandList[CommandOrder] = CommandEnumerator;
		break;
	case 3: 
		//DIRECTION_COMMAND::MOVE_UPLEFT;
		CommandList[CommandOrder] = CommandEnumerator;
		break;
	case 4: 
		//DIRECTION_COMMAND::MOVE_LEFT; 
		CommandList[CommandOrder] = CommandEnumerator;
		break;
	case 5: 
		//DIRECTION_COMMAND::MOVE_DOWNLEFT; 
		CommandList[CommandOrder] = CommandEnumerator;
		break;
	case 6: 
		//DIRECTION_COMMAND::MOVE_DOWN;  
		CommandList[CommandOrder] = CommandEnumerator;
		break;
	case 7: 
		//DIRECTION_COMMAND::MOVE_DOWNRIGHT; 
		CommandList[CommandOrder] = CommandEnumerator;
		break;
	}
	

	

}
