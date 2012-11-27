#include "MapSearchNode.h"
#include "Map.h"
// Qt
#include <QDebug>

MapSearchNode::MapSearchNode():
	_x(0),
	_y(0)
{
}


MapSearchNode::MapSearchNode( unsigned int px, unsigned int py ):
	_x(px),
	_y(py)
{

}

float MapSearchNode::GoalDistanceEstimate(MapSearchNode &nodeGoal)
{
	float xd = qAbs(float(((float)_x - (float)nodeGoal._x)));
	float yd = qAbs(float(((float)_y - (float)nodeGoal._y)));

	return xd + yd;
}

/*!

*/
bool MapSearchNode::IsGoal(MapSearchNode &nodeGoal)
{
	return IsSameState(nodeGoal);
}

/*!
 This generates the successors to the given Node. It uses a helper function called
 AddSuccessor to give the successors to the AStar class. The A* specific initialisation
 is done for each node internally, so here you just set the state information that
 is specific to the application
*/
bool MapSearchNode::GetSuccessors(AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node)
{
	int parent_x = -1;
	int parent_y = -1;

	if( parent_node )
	{
		parent_x = parent_node->x();
		parent_y = parent_node->y();
	}

	MapSearchNode NewNode;

	// push each possible move except allowing the search to go backwards

	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			int xPos = _x + x;
			int yPos = _y + y;
			if (Cnoti3D::Map::isPassable(xPos, yPos) &&
				!((parent_x == xPos) && (parent_y == yPos)) &&
				!((_x == xPos) && (_y == yPos)))
			{
				NewNode = MapSearchNode( _x + x, _y + y );
				astarsearch->AddSuccessor( NewNode );
			}
		}
	}

	return true;
}

float MapSearchNode::GetCost(MapSearchNode &successor)
{
//	return (float) GetMap( x, y );

	// If can pass
	return 1;
	// If can't pass
	return 1000;
}

/*!
  Check if is the same node (x & y are equal).
*/
bool MapSearchNode::IsSameState(MapSearchNode &rhs)
{
	if( (_x == rhs._x) &&
		(_y == rhs._y) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void MapSearchNode::printNodeInfo()
{
	qDebug() << "[MapSearchNode::printNodeInfo]" << _x << "," << _y;
}

