/*!
\class Blueprint blueprint.h <Blueprint>
This Class stores information about the walkable/not walkable "world tiles" and it used to find paths in the world that connect two two-dimension points.
It provides path finder and A* solver (astar or a-star) and it uses MicroPather as A* Solver. (http://grinninglizard.com/micropather/)

    0---> (Width)
    |
    |
    v (Height)
*/
#include "blueprint.h"

#include <QDebug>
#include <QFile>
#include <QList>
#include <QObject>
#include <QPoint>
#include <QIODevice>
#include <QTextStream>

namespace Cnoti3D
{
    /*!
    Constructs a new Blueprint.
    \param fileURI The URI of the file that contains information about the world
    */
    Blueprint::Blueprint(const QString& fileURI) : _playerX( 0 ), _playerY( 0 ), _pather( 0 )
    {
        _pather = new micropather::MicroPather( this, 20 );	// Use a very small memory block to stress the pather
        _mapWidth = _mapHeigth = 0;
        _playerX = 32;
        _playerY = 60;
        loadFile(fileURI);
    }

    Blueprint::~Blueprint() {
        delete _pather;
    }

    /*!
    Gets the player x postion
    */
    int Blueprint::x()
    {
        return _playerX;
    }

    /*!
    Gets the player y postion
    */
    int Blueprint::y()
    {
        return _playerY;
    }

    /*!
    Return the "checksum" of the last path returned by Solve(). Useful for debugging,
    and a quick way to see if 2 paths are the same.
    */
    unsigned Blueprint::checksum()
    {
        return _pather->Checksum();
    }

    /*!
    Cleans the path
    */
    void Blueprint::clearPath()
    {
        _path.resize( 0 );
    }

    /*!
    Return the hash of a 2D-position. This hash should be improved
    */
    QString Blueprint::hash(int x, int y)const
    {
        QString str = QString::number(x);
        str.prepend('#');
        str.prepend(QString::number(y));
        return str;
    }

    /*!
    Returns true if the tile is passable.
    */
    int Blueprint::passable( int nx, int ny )
    {
        int toRet=0;
        if (    nx >= 0 && nx < _mapWidth
                && ny >= 0 && ny < _mapHeigth )
        {
            QChar c = gMap.value(hash(ny,nx),' ');
            if ( c == ' ' )
            {
                toRet = 1;
            }
            else if ( c >= 'A' && c<='z' )
            {
                toRet = passable( c );
            }
        }
        return toRet;
    }

    /*!
    Checks if this tile is passable
    */
    int Blueprint::passable( QChar c )
    {
        return _doorsOpen.value(c,false);
    }

    /*!
    Sets tile as passable
    */
    void Blueprint::setPassable( QChar c, bool passable)
    {
        _doorsOpen.insert(c,passable);
    }

    /*!
    Sets tiles as passable
    */
    void Blueprint::setPassable( const QList<QChar> doorsLst, bool passable )
    {
        for(int i=0;i<doorsLst.size();i++)
        {
            setPassable(doorsLst.at(i),passable);
        }
    }

    /*!
    Sets player position
    */
    int Blueprint::setPos( int nx, int ny )
    {
        int result = 0;
        if ( passable( nx, ny ) == 1 )
        {
            float totalCost;

            result = _pather->Solve( xYToNode( _playerX, _playerY ), xYToNode( nx, ny ), &_path, &totalCost );

            if ( result == micropather::MicroPather::SOLVED ) {
                _playerX = nx;
                _playerY = ny;
            }
            qDebug()<< ( "Pather returned %d\n", result );

        }
        return result;
    }

    /*!
    Prints the map to qDebug
    */
    void Blueprint::print()
    {
        QString str=" ";

        for(int j=0;j<_mapWidth;j++)
        {
            str += QString::number(j%10);
        }
        qDebug()<< str;
        for(int i=0;i<_mapHeigth;i++)
        {
            str = QString::number(i%10);
            for(int j=0;j<_mapWidth;j++)
            {
                if(i==_playerY && j==_playerX)
                {
                    str+='@';
                }else
                {
                    for(int k=0; k<_path.size(); k++ )
                    {
                        int x, y;
                        nodeToXY( _path[k], &x, &y );
                        if ( y == i && x == j)
                        {
                            str+= '0' + k%10;
                        }
                    }
                    if(str.size()<j+2)
                    {
                        str+= gMap.value(hash(i,j),' ');
                    }
                }
            }
            str;
            qDebug()<< str;
        }
    }

    /*!
    Converts a node into a XY position
    */
    void Blueprint::nodeToXY( void* node, int* x, int* y )
    {
//        int index = (int)node;
//        *y = index / _mapWidth;
//        *x = index - *y * _mapWidth;
    }

    /*!
    Converts a XY position into a node
    */
    void* Blueprint::xYToNode( int x, int y )
    {
        return (void*) ( y*_mapWidth + x );
    }

    /*!
    Compute the minimum path cost
    */
    float Blueprint::leastCostEstimate( void* nodeStart, void* nodeEnd )
    {
        int xStart, yStart, xEnd, yEnd;
        nodeToXY( nodeStart, &xStart, &yStart );
        nodeToXY( nodeEnd, &xEnd, &yEnd );

        /* Compute the minimum path cost using distance measurement. It is possible
       to compute the exact minimum path using the fact that you can move only
       on a straight line or on a diagonal, and this will yield a better result.
    */
        int dx = xStart - xEnd;
        int dy = yStart - yEnd;
        return (float) sqrt( (double)(dx*dx) + (double)(dy*dy) );
    }

    void Blueprint::adjacentCost( void* node, std::vector< micropather::StateCost > *neighbors )
    {
        int x, y;
        const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
        const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
        const float cost[8] = { 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f };

        nodeToXY( node, &x, &y );

        for( int i=0; i<8; ++i ) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            int pass = passable( nx, ny );
            if ( pass > 0 ) {
                if ( pass == 1 )
                {
                    // Normal floor
                    micropather::StateCost nodeCost = { xYToNode( nx, ny ), cost[i] };
                    neighbors->push_back( nodeCost );
                }
                else
                {
                    // Normal floor
                    micropather::StateCost nodeCost = { xYToNode( nx, ny ), FLT_MAX };
                    neighbors->push_back( nodeCost );
                }
            }
        }
    }

    void Blueprint::printStateInfo( void* node )
    {
        int x, y;
        nodeToXY( node, &x, &y );
        qDebug() << ( "(%d,%d)", x, y );
    }

    /*!
      Loads the map contained in file to memory
    \param fileURI The URI of the file that contains information about the world
    */
    void Blueprint::loadFile(const QString& fileURI)
    {
        QFile file( fileURI );
        //QHash <QString,char> tmpHash;
        QChar tmpChar;
        if ( file.open( QIODevice::ReadOnly ) )
        {
            QTextStream stream( &file );
            QString line;
            int y = 0;
            while ( !stream.atEnd() ) {
                line = stream.readLine(); // line of text excluding '\n'
                for(int x=0;x<line.size();x++)
                {
                    tmpChar = line.at(x);
                    if(tmpChar == '#')
                    {
                        //Wall
                        gMap.insert(hash(y,x),'#');
                    }
                    else if(tmpChar >= 'A' && tmpChar <= 'z')
                    {
                        //Door
                        gMap.insert(hash(y,x),tmpChar);
                    }
                    _mapWidth=line.size();
                }
                y++;
            }
            _mapHeigth = y;
            file.close();
            deleteRedundancy();
        }else{
            qWarning()<<"[Blueprint::loadFile]Error loading file";
        }
    }

    /*!
    Remove unecessary inner map points keeping just the boarders
    */
    void Blueprint::deleteRedundancy()
    {
        QList<QPoint> toDelete;
        QChar qChar;
        for (int i=1;i<_mapHeigth-1;i++)
        {
            for(int j=1;j<_mapWidth-1;j++)
            {
                qChar = gMap.value(hash(i,j));
                if (qChar == '#')
                {
                    if( (gMap.value(hash(i-1,j-1)) == qChar) &&
                        (gMap.value(hash(i-1,j)) == qChar) &&
                        (gMap.value(hash(i-1,j+1)) == qChar) &&
                        (gMap.value(hash(i,j+1)) == qChar) &&
                        (gMap.value(hash(i+1,j+1)) == qChar) &&
                        (gMap.value(hash(i+1,j)) == qChar) &&
                        (gMap.value(hash(i+1,j-1)) == qChar) &&
                        (gMap.value(hash(i,j-1)) == qChar))
                    {
                        toDelete.append(QPoint(i,j));
                    }
                }
            }
        }
        for(int i=0;i<toDelete.length();i++)
        {
            gMap.remove(hash(toDelete.at(i).x(),toDelete.at(i).y()));
        }
    }

}
