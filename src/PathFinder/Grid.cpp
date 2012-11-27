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


#include "grid.h"
// Qt
#include <QString>
#include <QDebug>

namespace Cnoti3D
{

using namespace micropather;

Grid::Grid( ) : playerX( 0 ), playerY( 0 ), doorsOpen( false ), showConsidered( false ), pather( 0 )
{
	pather = new MicroPather( this, 20 );	// Use a very small memory block to stress the pather
	pathIt = 0;
}

Grid::~Grid() {
	delete pather;
}
#ifdef _WIN32
std::vector<Cell_grid> Grid::getPath(){
#else
std::vector< Cell_grid > Grid::getPath(){
#endif
	return pathCell;
}
int Grid::X()	{ return playerX; }
int Grid::Y() { return playerY; }

unsigned Grid::Checksum() { return pather->Checksum(); }

void Grid::Load( const char* filename ){
	gMap = "";
	std::ifstream myfile (filename);
	if (myfile.is_open()){
		std::string line;
		mapY = 0;
		while (! myfile.eof() ){
			getline (myfile,line);
			mapX = line.size();
#ifndef _WIN32_
			// Removes \r in the end of the line
			if( line.find("\r") != std::string::npos )
				line.erase( mapX - 1 );
#endif
			gMap = gMap + line;
			mapY++;
		}
		myfile.close();
		for( int i=0; i<gMap.size(); i++){
			if( doorsClose.find( gMap[i] ) == doorsClose.end() ){
				doorsClose[gMap[i]] = true;
			}
		}
	}
//	std::ofstream outFile ("files/teste.txt");
//	outFile << gMap;
//	outFile.close();
}

/*!
  Sets one point for the grid
*/
void Grid::setPoint1(float point1X, float point1Y)
{
	initX = point1X;
	initY = point1Y;
}

/*!
  Sets the second point for the grid and initializes the grid
*/
void Grid::setPoint2(float point2X, float point2Y)
{
	Init(initX, initY, point2X, point2Y);
}

void Grid::Init( float point1X, float point1Y, float point2X, float point2Y )
{
	if( point1X < point2X)
		initX = point1X;
	else
		initX = point2X;
	if( point1Y < point2Y)
		initY = point1Y;
	else
		initY = point2Y;

	height = abs(point1Y - point2Y);
	width = abs(point1X - point2X);

	stepX = width/mapX;
	stepY = height/mapY;
}
void Grid::ClearPath()
{
	path.resize( 0 );
}

void Grid::ToggleTouched() {
	showConsidered = !showConsidered; 
	pather->Reset();
}
void Grid::SwitchDoor( char c ){
//	qDebug() << "[Grid::SwitchDoor] Door:" << c << "close" << !doorsClose[c];
	if( doorsClose.find( c ) != doorsClose.end() ){
		doorsClose[c] = !doorsClose[c];
	}
}
void Grid::OpenDoor( char c )
{
//	qDebug() << "[Grid::OpenDoor] Door:" << c;
	if( doorsClose.find( c ) != doorsClose.end() ){
		doorsClose[c] = false;
	}
}
void Grid::CloseDoor( char c )
{
//	qDebug() << "[Grid::CloseDoor] Door:" << c;
	if( doorsClose.find( c ) != doorsClose.end() ){
		doorsClose[c] = true;
	}
}
void Grid::ToggleDoor() 
{ 
	doorsOpen = !doorsOpen; 
	pather->Reset();
}

bool Grid::isDoorClose(char c)
{
	if( doorsClose.find( c ) != doorsClose.end() )
	{
		return doorsClose[c];
	}
	return false;
}

int Grid::getIdRoom( float x, float y )
{
	int xInt = RealToInt( x,true);
	int yInt = RealToInt( y,false);
	char c=getChar(xInt, yInt);
	if(c=='0')
		return 9;
	if(c>'0' && c<'9')
		return (int)(c-'0');
	return -1;
}
char Grid::getChar( int x, int y )
{
	if (    x >= 0 && x < mapX 
		 && y >= 0 && y < mapY )
	{
		int index = y*mapX+x;
		char c = gMap[ index ];
		return c;
	}		
	return '#';
}
int Grid::Passable( float x, float y)
{
	int xInt = RealToInt( x,true);
	int yInt = RealToInt( y,false);
	return Passable(xInt, yInt);
}
int Grid::Passable( int nx, int ny ) 
{
	char c = getChar( nx, ny );

	if( doorsClose.find(c) != doorsClose.end() )
	{
//		qDebug() << "[Grid::Passable] Door Passable" << c << !doorsClose[c];
		return !doorsClose[c];
	}
	else{
		return 0;
	}
}
bool Grid::PassableRect(int minX, int minY, int maxX, int maxY)
{
	for(int i=minX; i<=maxX; i++){
		for(int j=minY; j<=maxY; j++){
			if(Passable(i,j) == 0)
				return false;
		}
	}
	return true;
}
int Grid::ReducePath()
{
	toDeleteList.clear();
	if(path.size()<=1)
		return 0;

	int k=0;
	while(true){
		int x, y;
		int xNext, yNext;
		int next = k+2;
		bool toDelete;
		NodeToXY( path[k], &x, &y );
		NodeToXY( path[k+1], &xNext, &yNext );
		int minX = Grid::Min(x, xNext);
		int maxX = Grid::Max(x, xNext);
		int minY = Grid::Min(y, yNext);
		int maxY = Grid::Max(y, yNext);
		while(true){
			if(next>=path.size()-1){
				return toDeleteList.size();
			}
			else{
				NodeToXY( path[next], &xNext, &yNext );
				minX = Grid::Min(minX, xNext);
				maxX = Grid::Max(maxX, xNext);
				minY = Grid::Min(minY, yNext);
				maxY = Grid::Max(maxY, yNext);
				toDelete = PassableRect(minX, minY, maxX, maxY);
				if(toDelete){
					toDeleteList.push_back(next-1);
					next++;
				}
				else{
					k=next-1;
					NodeToXY( path[k], &x, &y );
					next++;
					minX = Grid::Min(x, xNext);
					maxX = Grid::Max(x, xNext);
					minY = Grid::Min(y, yNext);
					maxY = Grid::Max(y, yNext);
				}
				//toDelete = false;
			}
		}
		k++;
	}
	int count = toDeleteList.size();
	return count;
}
int Grid::RealToInt( float real, bool axeX )
{
	int index;
	if( axeX ){
		if( real < initX )
			return -1;
		if( real > width )
			return mapY + 1;
		float aux = initX;
		for( index=0; index<mapX; index++){
			aux += stepX;
			if( real <= aux )
				break;
		}
	}
	else{
		if( real < initY )
			return -1;
		if( real > height )
			return mapY + 1;
		float aux = initY;
		for( index=0; index<mapY; index++){
			aux += stepY;
			if( real <= aux )
				break;
		}
	}
	return index;
}
float Grid::IntToReal( int index, bool axeX )
{
	float real;
	if( axeX ){
		real = initX + index * stepX + stepX/2;
	}
	else{
		real = initY + index * stepY + stepY/2;
	}
	return real;
}
float Grid::getPathRealX( int index )
{
	float x;
	if( index<0 || index >= pathCell.size() )
		return 0.0;
	x = IntToReal( pathCell[index].x, true );
	return x;
}
float Grid::getPathRealY( int index )
{
	float y;
	if( index<0 || index >= pathCell.size() )
		return 0.0;
	y = IntToReal( pathCell[index].y, false );
	return y;
}
int Grid::SetPlayer( float x, float y )
{
	int i = RealToInt( x, true );
	int j = RealToInt( y, false );
	int result = Passable(i , j);
	if( result != 0 ){		
		playerX = i;
		playerY = j;
	}
	return result;
}
int Grid::SetPos( int nx, int ny ) 
{
	int result = MicroPather::NO_SOLUTION;
	if ( Passable( nx, ny ) == 1 )
	{
		float totalCost;
		if ( showConsidered )
			pather->Reset();
			
		result = pather->Solve( XYToNode( playerX, playerY ), XYToNode( nx, ny ), &path, &totalCost );

		if ( result == MicroPather::SOLVED ) {
			playerX = nx;
			playerY = ny;

			pathIt++;			
			toDeleteList.clear();
			//Log* pathLog = LogManager::getSingleton().createLog( "pathDebug_"+LogManager::number(pathIt)+".log", false, true);
			//pathLog->logMessage(Print());
			ReducePath();
			//Log* pathLog2 = LogManager::getSingleton().createLog( "pathReduceDebug_"+LogManager::number(pathIt)+".log", false, true);
			//pathLog2->logMessage(Print());

			pathCell.clear();
			for( int k=0; k<path.size(); ++k ) {
				bool toInsert=true;
				for(int l=0; l<toDeleteList.size(); l++){
					if(k==toDeleteList[l]){
						toInsert=false;
						break;
					}
				}
				if(toInsert){
					int x, y;
					NodeToXY( path[k], &x, &y );
					Cell_grid c;
					c.x = x;
					c.y = y;
					pathCell.insert( pathCell.end(), c);
				}
			}
		}
	}
	return result;
}
int Grid::SetPosReal( float nx, float ny )
{
	if( nx < initX || nx > initX+width || ny < initY || ny > initY + height)
		return MicroPather::NO_SOLUTION;

	int i = RealToInt( nx, true );
	int j = RealToInt( ny, false );

	int result =  SetPos( i, j );

	return result;
}
std::string Grid::Print() 
{
	std::string buf;
	std::string line;

	buf = "\n   |0000000000111111111122222222223333333333444444444455555555556666\n";
	buf +=  "   |0123456789012345678901234567890123456789012345678901234567890123\n";	
	buf +=  "---|----------------------------------------------------------------\n";
	for( int j=0; j<mapY; ++j ) {
		// Copy in the line.
		line = gMap.substr( mapX*j, mapX+1 );
		line[mapX]='\n';

		unsigned k;
		// Wildly inefficient demo code.
		for( k=0; k<path.size(); ++k ) {
			int x, y;
			NodeToXY( path[k], &x, &y );
			if ( y == j ){
				for( int l=0; l<toDeleteList.size(); ++l ) {
					if(toDeleteList[l] == k){
						line[x] = 'X';
					}
				}
				if(line[x] != 'X')
					line[x] = '0' + k%10;
			}
		}
		// Insert the player
		if ( j==playerY )
			line[playerX] = '´';
		if(j<10)
			buf +="0";
		buf += QString::number(j).toStdString();
		buf += " |";
		buf += line;
	}
	buf += "\n---|----------------------------------------------------------------\n";
	buf += "   |0000000000111111111122222222223333333333444444444455555555556666\n";
	buf += "   |0123456789012345678901234567890123456789012345678901234567890123\n";
	return buf;
}

void Grid::NodeToXY( void* node, int* x, int* y ) 
{

#ifdef Q_WS_MAC
	long index = (long)node;
#else
	int index = (int)node;
#endif
	*y = index / mapX;
	*x = index - *y * mapX;
}

void* Grid::XYToNode( int x, int y )
{
	return (void*) ( y*mapX + x );
}

float Grid::LeastCostEstimate( void* nodeStart, void* nodeEnd ) 
{
	int xStart, yStart, xEnd, yEnd;
	NodeToXY( nodeStart, &xStart, &yStart );
	NodeToXY( nodeEnd, &xEnd, &yEnd );

	/* Compute the minimum path cost using distance measurement. It is possible
	   to compute the exact minimum path using the fact that you can move only 
	   on a straight line or on a diagonal, and this will yield a better result.
	*/
	int dx = xStart - xEnd;
	int dy = yStart - yEnd;
	return (float) sqrt( (double)(dx*dx) + (double)(dy*dy) );
}

void Grid::AdjacentCost( void* node, std::vector< StateCost > *neighbors ) 
{
	int x, y;
	const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	const float cost[8] = { 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f };

	NodeToXY( node, &x, &y );

	for( int i=0; i<8; ++i ) {
		int nx = x + dx[i];
		int ny = y + dy[i];

		int pass = Passable( nx, ny );
		if ( pass >0 ) {
			StateCost nodeCost = { XYToNode( nx, ny ), cost[i] };
			neighbors->push_back( nodeCost );
		}
		else{
			// Normal floor
			StateCost nodeCost = { XYToNode( nx, ny ), FLT_MAX };
			neighbors->push_back( nodeCost );
			}
	}
}

void Grid::PrintStateInfo( void* node ) 
{
	int x, y;
	NodeToXY( node, &x, &y );
	printf( "(%d,%d)", x, y );
}

} //namespace Cnoti3D
