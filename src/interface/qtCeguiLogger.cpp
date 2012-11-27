#include "qtCeguiLogger.h"

#include <QDebug>

qtCeguiLogger::qtCeguiLogger()
{
}


void qtCeguiLogger::logEvent (const CEGUI::String &message, CEGUI::LoggingLevel level )
{
	switch(level)
	{
	case CEGUI::Informative:
	case CEGUI::Standard:
		qDebug() << "[CEGUI]" << message.c_str();
		break;
	case CEGUI::Warnings:
		qDebug() << "[CEGUI]" << message.c_str();
		break;
	case CEGUI::Errors:
		qDebug() << "[CEGUI]" << message.c_str();
		break;
	}
}

void qtCeguiLogger::setLogFilename (const CEGUI::String &filename, bool append )
{
	Q_UNUSED(filename);
	Q_UNUSED(append);
	// does nothing
}
