#ifndef MAPSEARCHNODE_H
#define MAPSEARCHNODE_H

#include "../3rdParty/astar/stlastar.h"

class MapSearchNode
{
public:
	MapSearchNode();
	MapSearchNode( unsigned int px, unsigned int py );

	float GoalDistanceEstimate( MapSearchNode &nodeGoal );
	bool IsGoal( MapSearchNode &nodeGoal );
	bool GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node );
	float GetCost( MapSearchNode &successor );
	bool IsSameState( MapSearchNode &rhs );

	void printNodeInfo();

	unsigned int x(){return _x;}
	void setX(unsigned int px){_x = px;}
	unsigned int y(){return _y;}
	void setY(unsigned int py){_y = py;}

protected:
	unsigned int _x;	 // the (x,y) positions of the node
	unsigned int _y;
};

#endif // MAPSEARCHNODE_H
