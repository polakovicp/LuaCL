#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <fstream>

using namespace std;

namespace Logging
{
    class Log
    {
    private:
        ostream * Debug;
        ostream * Output;
    public:
        Log()
        {
            Debug = new ostream(cerr.rdbuf());
            Output = new ofstream("hecate.log");
        };
        Log(bool c)
        {
            Debug = new ostream(cerr.rdbuf());
            Output = new ofstream("hecate.log");
        };
        ~Log()
        {
            Debug->flush();
            Output->flush();
        };
        void Close()
        {

        };
        friend Log& operator<<(Log& log, const char * val)
        {
            *log.Output << val;
            return log;
        };
    };

    static Log LOG(false);
    static Log DBG;
//    Log& operator<<(Log& log, T& val)
//    {
//        *log.Debug << val;
//        return log;
//    };

//    Log& operator<<(Log& log, const char * val)
//    {
//        *log.Debug << val;
//        return log;
//    };
};

#endif // LOG_HPP
