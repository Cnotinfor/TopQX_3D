#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "../cnoti3d_global.h"
#include "MapSearchNode.h"
// aStar
#include "../3rdparty/astar/stlastar.h"
// Qt
#include <QVector3D>

namespace Cnoti3D
{

class Map;

class CNOTI3DLIB_EXPORT PathFinder
{
public:
    PathFinder();

	void loadMap(const QString &filename);
	QList<QVector3D> getPath(QVector3D start, QVector3D finish);
	void setMapPoint1(float point1X, float point1Y);
	void setMapPoint2(float point2X, float point2Y);

	void addImpassableChar(const QChar &c);
	void removeImpassableChar(const QChar &c);

	QChar getChar(QVector3D pos);
	bool isPassable(QChar c);
private:
	AStarSearch<MapSearchNode> astarsearch;
	Map *_map;
	// Methods
	QList<MapSearchNode *> reducePath(QList<MapSearchNode *> list);
	bool passableRect(MapSearchNode *node1, MapSearchNode *node2);
};

} //namespace Cnoti3D


#endif // PATHFINDER_H
