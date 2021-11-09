#include "LogObserver.h"

// LogObserver constructor
LogObserver::LogObserver()
{

}

// LogObserver destructor
LogObserver::~LogObserver()
{

}

void LogObserver::update(ILoggable* log)
{
    //TODO: File IO here
    log->stringToLog();
}
