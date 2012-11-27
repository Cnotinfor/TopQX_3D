#include "Map.h"
// Qt
#include <QDebug>
#include <QFile>
#include <QTextStream>

namespace Cnoti3D
{

QString Map::_activeMap = "";
QList<QChar> Map::_unpassablesChars = QList<QChar>();
int Map::mapX = 0;

Map::Map():
//	mapX (0),
	mapY (0),
	initX (0),
	initY (0),
	width (0),
	height (0),
	stepX (0),
	stepY (0)
{

}

void Map::loadMap (const QString &filename)
{
	QFile file (filename);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString line;
		QTextStream in(&file);
		while (!in.atEnd())
		{
			line = in.readLine();
			if(mapX == 0)
			{
				mapX = line.size();
			}
			_activeMap.append(line);
			mapY++;
		}
		file.close();
	}
	else
	{
		qWarning() << "[Map::loadMap] File" << filename << "not found";
	}

}

/*!
Sets one point for the grid
*/
void Map::setPoint1(float point1X, float point1Y)
{
	initX = point1X;
	initY = point1Y;
}

/*!
Sets the second point for the grid and initializes the grid
*/
void Map::setPoint2(float point2X, float point2Y)
{
	init(initX, initY, point2X, point2Y);
}


void Map::init( float point1X, float point1Y, float point2X, float point2Y )
{
	if (point1X < point2X)
	{
		initX = point1X;
	}
	else
	{
		initX = point2X;
	}
	if (point1Y < point2Y)
	{
		initY = point1Y;
	}
	else
	{
		initY = point2Y;
	}

	height = qAbs(point1Y - point2Y);
	width = qAbs(point1X - point2X);

	stepX = width/mapX;
	stepY = height/mapY;
}

bool Map::isPassable(int x, int y)
{
	QChar c = getChar(x, y);
	if(!c.isNull())
	{
		return isPassable(c);
	}
	return false;
}

bool Map::isPassable(QChar c)
{
	return !_unpassablesChars.contains(c);
}

/*!
  From world position to map position
*/
int Map::realToInt( float real, bool axeX )
{
	int index;
	if (axeX)	// X axes
	{
		// Check if in map boundaries
		if (real < initX)
		{
			return -1;
		}
		if (real > width)
		{
			return mapY + 1;
		}
		// Check position
		float aux = initX;
		for (index = 0; index < mapX; index++)
		{
			aux += stepX;
			if( real <= aux )
			{
				break;
			}
		}
	}
	else	// Y axes
	{
		// Check if in map boundaries
		if (real < initY)
		{
			return -1;
		}
		if (real > height)
		{
			return mapY + 1;
		}
		// Check position
		float aux = initY;
		for (index = 0; index < mapY; index++)
		{
			aux += stepY;
			if( real <= aux )
			{
				break;
			}
		}
	}
	return index;
}

/*!
  From map position to world position
*/
float Map::intToReal( int index, bool axeX )
{
	float real;
	if( axeX ) // X axes
	{
		real = initX + index * stepX + stepX/2;
	}
	else // Y axes
	{
		real = initY + index * stepY + stepY/2;
	}
	return real;
}

void Map::addImpassableChar(const QChar &c)
{
	if(!_unpassablesChars.contains(c))
	{
		_unpassablesChars.append(c);
	}
}

void Map::removeImpassableChar(const QChar &c)
{
	_unpassablesChars.removeAll(c);
}

QChar Map::getChar( QVector3D pos )
{
	int xInt = realToInt( pos.x(), true);
	int yInt = realToInt( pos.y(), false);
	return getChar(xInt, yInt);
}

QChar Map::getChar( int x, int y )
{
	// calc position
	int pos = y*mapX+x;
	// check if position is not bigger then QString size
	if(pos < _activeMap.size() && pos >= 0 )
	{
		// return character
		return _activeMap.at(pos);
	}
	return '\0';
}

} // namespace Cnoti3D
