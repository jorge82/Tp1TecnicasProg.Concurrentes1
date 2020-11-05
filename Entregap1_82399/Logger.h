#if !defined LOGGER_H
#define LOGGER_H

#include <iostream>
#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>
#include <time.h> 
#include <fstream> 
#include <mutex>          

using namespace std;

class Logger{

public:

   static Logger* getLogger(string file_name);
   void writeToLogFile(string &log);
   ~Logger();

private:

   Logger(string file_name);  // Private so that it can  not be called
   //Logger(Logger const&);             // copy constructor is private
   //Logger& operator=(Logger const&){};  // assignment operator is private


   static Logger* m_pInstance;
   std::ofstream file;
   std::mutex mMutex;

};
#endif // LOGGER_H