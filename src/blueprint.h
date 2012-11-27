#ifndef BLUEPRINT_H
#define BLUEPRINT_H

#include "Cnoti3d_global.h"
#include <math.h>

#include "3rdparty/micropather/micropather.h"
#include <QChar>
#include <QString>
#include <QHash>
#include <QList>

namespace Cnoti3D
{
	class CNOTI3DLIB_EXPORT Blueprint : public micropather::Graph
	{
	private:
		Blueprint( const Blueprint& );
		void operator=( const Blueprint& );

		int _playerX, _playerY;
		std::vector<void*> _path;
		QHash<QChar,bool> _doorsOpen;

		micropather::MicroPather* _pather;
		QHash<QString,QChar> gMap;
		int _mapWidth;
		int _mapHeigth;
		void loadFile(const QString& fileURI);
		QString hash(int x, int y)const;
		void deleteRedundancy();

	public:
		Blueprint(const QString& fileURI);

		virtual ~Blueprint();

		int x();
		int y();
		unsigned checksum();
		void clearPath();
		virtual int passable( int nx, int ny );
		virtual int passable( QChar c );
		virtual void setPassable( QChar c, bool passable );
		virtual void setPassable( const QList<QChar> doorsLst, bool passable );
		int setPos( int nx, int ny );
		void print();
		void nodeToXY( void* node, int* x, int* y );
		void* xYToNode( int x, int y );
		virtual float leastCostEstimate( void* nodeStart, void* nodeEnd );
		virtual void adjacentCost( void* node, std::vector< micropather::StateCost > *neighbors );
		virtual void printStateInfo( void* node );
	};
}
#endif // BLUEPRINT_H
