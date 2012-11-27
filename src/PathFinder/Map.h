#ifndef MAP_H
#define MAP_H

#include "../cnoti3d_global.h"
#include "MapSearchNode.h"
// Qt
#include <QString>
#include <QVector3D>

namespace Cnoti3D
{

class CNOTI3DLIB_EXPORT Map
{
	friend class MapSearchNode;
public:
    Map();

	void loadMap(const QString &filename);
	void setPoint1(float point1X, float point1Y);
	void setPoint2(float point2X, float point2Y);
	void init(float point1X, float point1Y, float point2X, float point2Y);

	int realToInt(float real, bool axeX);
	float intToReal(int index, bool axeX);

	void addImpassableChar(const QChar &c);
	void removeImpassableChar(const QChar &c);
	static bool isPassable(int x, int y);
	static bool isPassable(QChar c);

	QChar getChar(QVector3D pos);
	static QChar getChar(int x, int y);
private:
	static QString _activeMap;
	static QList<QChar> _unpassablesChars;
	static int mapX;
	int mapY;
	// World variable
	float initX, initY;		// World position
	float height, width;	// World size
	float stepX, stepY;		// Step whe are going to travel between world points
};

} // namespace Cnoti3D


#endif // MAP_H
