/*!
\class ObjectManager ObjectManager.h <ObjectManager>

Class to manage objects created in Cnoti3D

\sa Object3D
*/

#include "ObjectManager.h"
#include "Object3dAbstract.h"
#include "Object3dMousable.h"
#include "Object3dAnimated.h"
#include "Character.h"
#include <QDebug>

namespace Cnoti3D {

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{

}
/*!
Check if exists a Cnoti3dObject named /a name.
*/
bool ObjectManager::isCnoti3dObject( QString name )
{
	return  _objectHash.contains( name );
}

/*!
Check if Cnoti3dObject named /a name is mouse aware.
//*/
//bool ObjectManager::isObjectMouseAware( QString name )
//{
////	qDebug() << "[ObjectManager::isObjectMouseAware] Object Name:" << name;

//	if( _objectHash.contains( name ) )
//	{

//		Object3DAbstract* object = _objectHash.value(name);
//		if ( dynamic_cast<Object3DMousable*>(object) == 0)
//		{
//			return false;
//		}else
//		{
//			return true;
//		}
//	}
//	return false;
//}

/*!
Check if Cnoti3dObject named /a name is mouse aware and is available (enabled sttus is true).
*/
Object3DMousable* ObjectManager::isObjectMouseAwareAvailable( QString name )
{
//	qDebug() << "[ObjectManager::isObjectMouseAware] Object Name:" << name;
	if( _objectHash.contains( name ) )
	{
		Object3DMousable* object = dynamic_cast<Object3DMousable*>(_objectHash.value(name));
//		if( name.contains( "flute_box" ))
//		{
//			qWarning() << "[ObjectManager::isObjectMouseAware]: "<< name << object;

//		}
		if( object != 0 && object->isEnable() )
		{
			return object;
		}
	}
	return NULL;
}

/*!
Gets the Cnoti3dObject named /a name.
*/
Object3DAbstract* ObjectManager::object3DAbstract( QString name )
{
	if( _objectHash.contains( name ) )
	{
		return _objectHash.value(name);
	}
	return NULL;
}

/*!
Adds new object to hash table, but only if doesn't exist one with the same name.
*/
bool ObjectManager::addObject3DAbstract( Object3DAbstract* object )
{
	bool toReturn = false;
	if( !_objectHash.contains( object->name() ) )
	{
		_objectHash.insert( object->name(), object );
		if( isAnimated( object ) )
		{
			if(!_obj3DAnimLst.contains(object))
			{
				_obj3DAnimLst.append(object);
			}
		}
		toReturn = true;
	}
	return toReturn;
}

/*!
Removes object from hash table.
*/
void ObjectManager::removeCnoti3dObject( QString name )
{
	if( _objectHash.contains( name ) )
	{
		_objectHash.remove( name );
		/*ToDo remove objects _obj3DAnimLst.remove*/
	}
}

void ObjectManager::updateObjects(qreal timeSinceLastFrame)
{
	// Animation in Ogre uses seconds, not miliseconds
	for(int i=0;i<_obj3DAnimLst.size();i++)
	{
		//qDebug() << "[ObjectManager::updateObjects]" << _obj3DAnimLst.at(i)->name();
		_obj3DAnimLst.at(i)->update( timeSinceLastFrame );
	}

}

/*!
Check is from class that contains animation and becuase of that needs to be updated.
*/
bool ObjectManager::isAnimated( Object3DAbstract* object )
{
	Object3DAnimated* animated = dynamic_cast<Object3DAnimated*>(object);
	if( animated != NULL )
	{
		return true;
	}

	Character* character = dynamic_cast<Character*>(object);
	if( character != NULL )
	{
		return true;
	}
	return false;
}

}	// namespace Cnoti3D
