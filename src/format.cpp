#include <string>
#include <iomanip>

#include "format.h"

using std::string;
using namespace std;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function

string Format::ElapsedTime(long seconds)
{ 
    
    int hrs,minu,sec;
    stringstream data;
    hrs = seconds / 3600;
    minu = (seconds % 3600)/60;
    sec = (seconds % 3600) % 60;

    data << setw(2) << setfill('0') << hrs << ":"
        << setw(2) << setfill('0') << minu << ":"
         << setw(2) << setfill('0') << sec << endl;

  
   return data.str();
    
    
}
