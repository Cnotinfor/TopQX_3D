#ifndef FINALSTATE_H
#define FINALSTATE_H

#include "Cnoti3d_global.h"

#include <QFinalState>
#include <QString>

namespace Cnoti3D
{
	class StateManager;
	class CNOTI3DLIB_EXPORT FinalState : public QFinalState
	{
	public:
		FinalState(const QString& name,
				   StateManager* sm,
				   QState *parent = 0);
		FinalState(const QString& name,
				   QState *parent);
		QString name()const;
		int depth()const;
	protected:
		void onEntry(QEvent *event);

	private:
		StateManager* _stateManager;
		QString _name;
	};
}
#endif // FINALSTATE_H
