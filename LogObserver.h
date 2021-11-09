#include "LoggingObserver.h"

class LogObserver : public Observer {
    public:
        LogObserver();
        ~LogObserver();
        void update(ILoggable*);
    private:
        Subject* subject;
};
