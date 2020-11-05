#include <stddef.h>  // defines NULL

#include "Logger.h"




// Global static pointer used to ensure a single instance of the class.



Logger* Logger::m_pInstance = NULL; 

/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor

    is private and is only called by this Instance function.

*/

Logger* Logger::getLogger(string file_name){
   if (!m_pInstance) {  // Only allow one instance of class to be generated.
        m_pInstance = new Logger(file_name);
    }
   return m_pInstance;
}

Logger::Logger(string file_name){
    this->file.open(file_name,ios::app);
    if(file.fail()){
          perror("Error opening log file");  
    }
}

    

void Logger::writeToLogFile(string &log){

    std::lock_guard<std::mutex> lock(this->mMutex);
/*
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char timestamp[26];
    ctime_s(timestamp, sizeof timestamp, &now);
    */
      // current date/time based on current system
   time_t now = time(0);
   
   // convert now to string form
   char* dt = ctime(&now);

    std::string timestampWithoutEndl(dt);    
    timestampWithoutEndl = timestampWithoutEndl.substr(0, 24);

    this->file<< timestampWithoutEndl << ": " << log << std::endl;
}

Logger::~Logger(){
    delete m_pInstance;

    file.close();
}