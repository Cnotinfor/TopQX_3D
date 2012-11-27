/*
Copyright (c) 2000-2005 Lee Thomason (www.grinninglizard.com)

Grinning Lizard Utilities.

This software is provided 'as-is', without any express or implied 
warranty. In no event will the authors be held liable for any 
damages arising from the use of this software.

Permission is granted to anyone to use this software for any 
purpose, including commercial applications, and to alter it and 
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must 
not claim that you wrote the original software. If you use this 
software in a product, an acknowledgment in the product documentation 
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and 
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source 
distribution.
*/

#ifndef GRID_H
#define GRID_H

#include "Cnoti3d_global.h"
#include "../3rdParty/micropather/micropather.h"

#include <ctype.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>

#include <map>
#include <iostream>
#include <fstream>
#include <string>

namespace Cnoti3D
{

using namespace micropather;

struct Cell_grid {
   int x;
   int y;
};

class CNOTI3DLIB_EXPORT Grid : public Graph
{
  private:
	Grid( const Grid& );
	void operator=( const Grid& );
  
	int playerX, playerY;
	std::vector<void*> path;
	std::vector<int> toDeleteList;
	std::map<char, bool> doorsClose;
	std::vector< Cell_grid > pathCell;
	bool doorsOpen;
	bool showConsidered;
	std::string gMap;
	int mapX, mapY;
	float initX, initY, height, width, stepX, stepY;

	MicroPather* pather;

  public:
	Grid();

	~Grid();

	int X();
	int Y();

	int pathIt;
#ifdef _WIN32
    std::vector<Cell_grid> getPath();
#else
	std::vector< Cell_grid > getPath();
#endif

	unsigned Checksum();

	void Load( const char* filename );
	void setPoint1(float point1X, float point1Y);
	void setPoint2(float point2X, float point2Y);
	void Init( float point1X, float point1Y, float point2X, float point2Y );
	void ClearPath();

	void ToggleTouched() ;
	void SwitchDoor( char c );
	void OpenDoor( char c );
	void CloseDoor( char c );
	void ToggleDoor() ;
	bool isDoorClose( char c );

	int Passable( float x, float y);
	int Passable( int nx, int ny ) ;
	int ReducePath();
	bool PassableRect(int minX, int minY, int maxX, int maxY);

	int RealToInt( float real, bool axeX );
	float IntToReal( int index, bool axeX );

	int getPathSize(){ return pathCell.size(); };
	float getPathRealX( int index );
	float getPathRealY( int index );
	int getIdRoom( float x, float y );
	char getChar( int x, int y );

	int SetPlayer( float x, float y );
	int SetPos( int nx, int ny ) ;
	int SetPosReal( float nx, float ny ) ;

	std::string Print();
	void NodeToXY( void* node, int* x, int* y ) ;
	void* XYToNode( int x, int y );
		
	
	float LeastCostEstimate( void* nodeStart, void* nodeEnd );
	void AdjacentCost( void* node, std::vector< StateCost > *neighbors );

	void PrintStateInfo( void* node );
	static int Min(int a, int b)
	{
		if(a<=b)
			return a;
		else
			return b;
	};
	static int Max(int a, int b)
	{
		if(a>=b)
			return a;
		else
			return b;
	};
};

} //namespace Cnoti3D


#endif
