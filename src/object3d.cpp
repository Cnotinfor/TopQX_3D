/*!
\class Object3D object3d.h <Object3D>

The Object3D implemets Object3DAbstratct class and is the base class of Object3DAnimated and Object3dMousable.

\sa Object3DAbstract, Object3DAnimated, Object3DAnimatedComplex, Object3DMousable
*/
#include "object3d.h"
// Qt
#include <QDebug>

namespace Cnoti3D
{
    /*!
    Constructs a new Object3D.
    \param sceneManager     Ogre's SceneManager
    \param objectName       Object3D's name
    */
	Object3D::Object3D( Ogre::SceneManager* sceneManager, const QString& objectName )
		:Object3DAbstract()
    {
		_sceneManager = sceneManager;
		_entity = sceneManager->getEntity( objectName.toStdString() );
    }

    /*!
    Constructs a new Object3D.
    \param entity   Ogre's entity
    */
	Object3D::Object3D( Ogre::Entity* entity )
		:Object3DAbstract()
    {
        _entity = entity;
    }

    /*!
    Destructor
    */
    Object3D::~Object3D()
    {

    }

    void Object3D::update(float timeSinceLastFrame)
    {

    }

    /*!
    Entity's name
    */
    QString Object3D::name()const
    {
		return _entity->getName().c_str();
    }

    /*!
    Entity's Scene Node
    */
    Ogre::SceneNode* Object3D::sceneNode()const
    {
        return _entity->getParentSceneNode();
    }

    /*!
    Entity's parent node
    */
    Ogre::Node* Object3D::node()const
    {
        return _entity->getParentNode();
    }

    /*!
    Entity
    */
    Ogre::Entity* Object3D::entity()const
    {
        return _entity;
    }

    //void Object3D::setNode(Ogre::Node* node)
    //{
    //
    //}

    /*!
    Adds the entity to this scene node and deletes the previous one if it was created by Object3D
    */
    void Object3D::setSceneNode(Ogre::SceneNode* sceneNode)
    {
        //
        //	Remove entity from current node
        //
        this->sceneNode()->detachObject( _entity->getName() );
        //
        //	Destroys node if we created it
        //
        QString nodeName = this->sceneNode()->getName().c_str();
        if(  nodeName == _entity->getName().c_str() )
        {
            Ogre::SceneManager* sceneMgr = this->sceneNode()->getCreator();
            sceneMgr->destroySceneNode( this->sceneNode() );
        }
        //
        //	Keeps the new node & adds the entity to it
        //
        sceneNode->attachObject( _entity );
    }

    /*!
	Sets the object position
*/
    void Object3D::setPosition(qreal x, qreal y, qreal z)
	{
		Object3DAbstract::setPosition(QVector3D(x, y, z));
        this->sceneNode()->setPosition( x, y, z );
    }
    /*!
	Sets the object position
*/
    void Object3D::setPosition(const QVector3D& pos)
	{
		Object3DAbstract::setPosition(pos);
        this->sceneNode()->setPosition( pos.x(), pos.y(), pos.z() );
    }

    /*!
    Gets the object position
    */
    QVector3D Object3D::position()const
    {
		return QVector3D( this->xPos(), this->yPos(), this->zPos() );
    }

    /*!
    Sets the object orientation
    */
    void Object3D::setOrientation(const QVector3D& lookTo)
	{
        Ogre::Vector3 ogrePos( lookTo.x(), lookTo.y(), lookTo.z() );
        this->sceneNode()->lookAt( ogrePos, Ogre::Node::TS_LOCAL );
    }

    //QVector3D Cnoti3dObject::orientation()
    //{
    //
    //}

    /*!
    Gets the object X position
    */
    qreal Object3D::xPos()const
    {
        return this->sceneNode()->getPosition().x;
    }

    /*!
    Gets the object Y position
    */
    qreal Object3D::yPos()const
    {
        return this->sceneNode()->getPosition().y;
    }

    /*!
    Gets the object Z position
    */
    qreal Object3D::zPos()const
    {
        return this->sceneNode()->getPosition().z;
    }

    /*!
    Returns true if the object is visible and false otherwise
    */
    bool Object3D::isVisible()const
    {
        return _entity->isVisible();
    }

    /*!
    Show's the object
    */
    void Object3D::show()
	{
        _entity->setVisible( true );
    }

    /*!
    Hides the object
    */
    void Object3D::hide()
	{
        _entity->setVisible( false );
    }

}
