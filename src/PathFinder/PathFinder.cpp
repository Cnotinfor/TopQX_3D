#include "PathFinder.h"
#include "Map.h"
// Qt
#include <QDebug>

namespace Cnoti3D
{

PathFinder::PathFinder():
	_map (NULL)
{

}

void PathFinder::loadMap (const QString &filename)
{
	if (!_map)
	{
		_map = new Map();
		if (!_map)
		{
			qWarning() << "[PathFinder::loadMap] Couldn't create map instance.";
			return;
		}
	}
	_map->loadMap(filename);
}

/*!
  Sets one point for the grid
*/
void PathFinder::setMapPoint1(float point1X, float point1Y)
{
	if (_map)
	{
		_map->setPoint1(point1X, point1Y);
	}
}

/*!
  Sets the second point for the grid and initializes the grid
*/
void PathFinder::setMapPoint2(float point2X, float point2Y)
{
	if (_map)
	{
		_map->setPoint2(point2X, point2Y);
	}
}


/*!
  Get the point to travel between start and finish.
*/
QList<QVector3D> PathFinder::getPath(QVector3D start, QVector3D finish)
{
	QList<QVector3D> returnList;
	if (!_map)
	{
		return returnList;
	}

	// Create a start state
	MapSearchNode nodeStart;
	nodeStart.setX(_map->realToInt(start.x(), true));
	nodeStart.setY(_map->realToInt(start.z(), false));

	// Define the goal state
	MapSearchNode nodeEnd;
	nodeEnd.setX(_map->realToInt(finish.x(), true));
	nodeEnd.setY(_map->realToInt(finish.z(), false));

	// Set Start and goal states
	astarsearch.SetStartAndGoalStates( nodeStart, nodeEnd );

	unsigned int SearchState;
	do
	{
		SearchState = astarsearch.SearchStep();

		int len = 0;
		MapSearchNode *p = astarsearch.GetOpenListStart();
		while( p )
		{
			len++;
			p = astarsearch.GetOpenListNext();

		}

		len = 0;
		p = astarsearch.GetClosedListStart();
		while( p )
		{
			len++;
			p = astarsearch.GetClosedListNext();
		}
	}
	while (SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING);

	if( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED )
	{
		QList<MapSearchNode *> list;
		// Get the first point
		list << astarsearch.GetSolutionStart();
		MapSearchNode *node;
		// Get the other points
		for( ;; )
		{
			node = astarsearch.GetSolutionNext();

			if (!node)
			{
				break;
			}

			list << node;

//			qDebug() << "[PathFinder::getPath]" << node->x() << node->y();
//			qDebug() << "[PathFinder::getPath]" << _map->intToReal(node->x(), true) << 0 << _map->intToReal(node->y(), false);
			
		};

		// Reduce list to essencial points
		QList<MapSearchNode *> reduceList = reducePath(list);

		// Convert to QVector3D list
		for (int i = 0; i < reduceList.size(); ++i) // First is the start position
		{
			returnList << QVector3D (_map->intToReal(reduceList[i]->x(), true), 0, _map->intToReal(reduceList[i]->y(), false));
		}


		// Once you're done with the solution you can free the nodes up
		astarsearch.FreeSolutionNodes();
	}
	else if( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED )
	{
		qWarning() << "[PathFinder::getPath] Search terminated. Did not find goal state";
	}
	qDebug() << "[PathFinder::getPath]" << returnList;

	return returnList;
}

QList<MapSearchNode *> PathFinder::reducePath(QList<MapSearchNode *> list)
{
	QList<MapSearchNode *> returnList;
	MapSearchNode *node = list.last();
	// start from the last and see if has straight  line between the 2
	int lastPos = list.size();
	while (node != list.first())
	{
		int i = 0;
		// Test (not need to test the node before the one we are testing)
		for (; i < lastPos - 1; ++i)
		{
			if (passableRect(node, list[i]))
			{
				break;
			}
		}
		// Stores the current node
		returnList.prepend(node);
		// Test another node
		node = list[i];
		lastPos = i;
	}
	return returnList;
}

bool PathFinder::passableRect(MapSearchNode *node1, MapSearchNode *node2)
{
	int minX = qMin(node1->x(), node2->x());
	int maxX = qMax(node1->x(), node2->x());
	int minY = qMin(node1->y(), node2->y());
	int maxY = qMax(node1->y(), node2->y());

	for(int i=minX; i<=maxX; i++)
	{
		for(int j=minY; j<=maxY; j++)
		{
			if(!_map->isPassable(i,j))
			{
				return false;
			}
		}
	}
	return true;
}

void PathFinder::addImpassableChar(const QChar &c)
{
	_map->addImpassableChar(c);
}

void PathFinder::removeImpassableChar(const QChar &c)
{
	_map->removeImpassableChar(c);
}

QChar PathFinder::getChar(QVector3D pos)
{
	return _map->getChar(pos);
}

bool PathFinder::isPassable(QChar c)
{
	return _map->isPassable(c);
}

} // namespace Cnoti3D
