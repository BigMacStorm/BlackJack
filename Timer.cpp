/**
@file   Timer.cpp
@author Aaron Whitehead <aaronbmwhitehead@gmail.com>
@brief This program uses sys/time to calculate runtime
@version Original Code 1.00 (02/19/2014) - C.S. Student
*/

// Program Description/Support /////////////////////////////////////
/**
@mainpage  This program reads uses a class that has been created in order to first get the current time, and then call the class again to find the total runtime. This program is used to see the runtime of certain functions.
*/
// Precompiler Directives //////////////////////////////////////////
//
///// NONE
//
// Header Files ///////////////////////////////////////////////////
//
#include  <iostream> 
#include  <stddef.h> 
#include  <sys/time.h> 
#include  <cstdio>
//#include "Timer.h"
//
using namespace std;
//
// Class Definitions //////////////////////////////////////////////
//
///// NONE
//
// Free Function Prototypes ///////////////////////////////////////
//

//
///// Main Function Implementation ///////////////
//
///// NONE
//

/**
Constructs the value of the data types in the timer class

This function constructs the timer class by assigning the default values to the private data members.
 
@param  Perameters None
@pre none
@post none
@exception none
@return none
*/
Timer::Timer()
{
   timerWasStarted = false;
   timerWasEnded = false;
}

/**
Starts the timer object

This function sets the current time of data, using the gettimeofday function, and sets the gotten value to a timeval object. This function also sets the boolean to true. Throws a runtime error if it could not grab time.
 
@param  Perameters none
@pre none
@post none
@exception runtime error, failed to get the value of the gettimeofday function
@return void
*/
void Timer::start() throw (runtime_error)
{
   gettimeofday(&tod1, NULL);//get data
   if(tod1.tv_sec == -1) throw runtime_error("failed to get clock val");//throw
   else timerWasStarted = true;//flip boolean
}

/**
Stops the timer object

This function sets the current time of data, using the gettimeofday function, and sets the gotten value to a timeval object. This function also sets the boolean of ended to true. Throws a logic error if it was called out of turn
 
@param  Perameters none
@pre none
@post none
@exception logic_error, called stop out of turn
@return void
*/
void Timer::stop() throw (logic_error)
{
   if(!timerWasStarted) throw logic_error("stop() before start()"); //throw
   else
   {
      gettimeofday(&tod2, NULL); //get data
      timerWasEnded = true; //flip boolean
   }
}

/**
returns the diffrence in time data

This function grabs the neccesary actual data out of the timeval objects in order to find a difference in seconds.
 
@param  Perameters none
@pre timerwasstarted and timerwasended
@post none
@exception logic_errors, called if this function is ran out of turn
@return double, the value of time2 minus time1 divided to put into terms of seconds.
*/
double Timer::getElapsedTime() const throw (logic_error)
{   

   if(!timerWasStarted) throw logic_error("getElapseTime() before start()"); //throw
   else if(!timerWasEnded) throw logic_error("getElapseTime() before stop()"); //throw
   else
   {      
      long long t1, t2; //temp variables
      t1 = tod1.tv_sec * 1000000 + tod1.tv_usec; //convert stored data to seconds
      t2 = tod2.tv_sec * 1000000 + tod2.tv_usec; //again
      return (t2 - t1)/1000000.00; //return the value divided by 1000000.00 to get value in seconds
   }
}
