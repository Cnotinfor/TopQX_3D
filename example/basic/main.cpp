// 3D Graphics engine
#include "example.h"
// Qt
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Cnoti3D::QOgreGraphicsView* view = new Cnoti3D::QOgreGraphicsView();
//    view->resize( 800, 600 );
	Example* my3D = new Example(/*view*/);
	Q_UNUSED( my3D );
//    view->setAlignment(Qt::AlignCenter);
//    view->show();
    return a.exec();
}


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

//#include "blueprint.h"
//#include <iostream>

//int main( int /*argc*/, const char** /*argv*/ )
//{
//    {
//        // Run the test code.
//        Blueprint test;
//        test.loadFile("house.ivgd");
//
//        const int NUM_TEST = 5;
//        int tx[NUM_TEST]	= {	24,	25,	10,	6,	0	};	// x of test
//        int ty[NUM_TEST]	= {	9,	9,	5,	5,	0	};	// y of test
//        int door[NUM_TEST]	= {	0,	0,	0,	1,	0	};	// toggle door? (before move)
//        int result[NUM_TEST]= { MicroPather::SOLVED, MicroPather::SOLVED, MicroPather::NO_SOLUTION,
//                                MicroPather::SOLVED, MicroPather::SOLVED };
//        unsigned check[NUM_TEST]	= { 139640, 884, 0, 129313, 2914 };
//
//        for( int i=0; i<NUM_TEST; ++i )
//        {
//            if ( door[i] )
//                test.ToggleDoor();
//            int _result = test.SetPos( tx[i], ty[i] );
//
//            if ( _result == result[i] )	{
//                // result good.
//                if ( _result == MicroPather::SOLVED ) {
//                    if ( test.Checksum() == check[i] )
//                        printf( ">> Test %d to (%d,%d) ok\n", i, tx[i], ty[i] );
//                    else
//                        printf( ">> Test %d to (%d,%d) BAD CHECKSUM\n", i, tx[i], ty[i] );
//                }
//                else {
//                    printf( ">> Test %d to (%d,%d) ok\n", i, tx[i], ty[i] );
//                }
//            }
//            else {
//                printf( ">> Test %d to (%d,%d) BAD RESULT\n", i, tx[i], ty[i] );
//            }
//        }
//    }

//    Cnoti3D::Map dungeon("../media/house.ivgd");
//    bool done = false;
//    char buf[ 256 ];
//
//    while ( !done ) {
//        dungeon.Print();
//        printf( "\n# # to move, q to quit, r to redraw, d to toggle doors, t for touched\n" );
        //gets( buf );
        //printf( "\n" );

//        std::cin.getline( buf, 256 );
//
//        if ( *buf )
//        {
//            if ( buf[0] == 'q' ) {
//                done = true;
//            }
//            else if ( buf[0] == 'd' ) {
//                dungeon.ToggleDoor();
//                dungeon.ClearPath();
//            }
//            else if ( buf[0] == 't' ) {
//                dungeon.ToggleTouched();
//            }
//            else if ( buf[0] == 'r' ) {
//                dungeon.ClearPath();
//            }
//            else if ( isdigit( buf[0] ) ) {
//                int x, y;
//                sscanf( buf, "%d %d", &x, &y );	// sleazy, I know
//                dungeon.SetPos( x, y );
//            }
//        }
//        else
//        {
//            dungeon.ClearPath();
//        }
//    }
//    return 0;
//}
