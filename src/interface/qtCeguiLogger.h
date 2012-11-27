#ifndef QTCEGUILOGGER_H
#define QTCEGUILOGGER_H

#include <QtCore>

#include <CEGUI.h>
#include <CEGUILogger.h>

class qtCeguiLogger : public CEGUI::Logger
{
public:
	qtCeguiLogger();

	void logEvent (const CEGUI::String& message, CEGUI::LoggingLevel level=CEGUI::Standard);
	void setLogFilename (const CEGUI::String& filename, bool append=false);
};

#endif // QTCEGUILOGGER_H
