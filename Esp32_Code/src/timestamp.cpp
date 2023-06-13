#include <timestamp.h>

#define countof(a) (sizeof(a) / sizeof(a[0]))

ThreeWire myWire(TIMESTAMP_DATA_PIN, TIMESTAMP_CLOCK_PIN, TIMESTAMP_RESET_PIN ); // Create a three wire type object
RtcDS1302<ThreeWire> Rtc( myWire ); // Create an RTC object

void setupTimestamp(){
    Rtc.Begin(); // Initialize the RTC
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__); // Receive the date and time of the compilation

    if(Rtc.GetIsWriteProtected())   Rtc.SetIsWriteProtected(false); // Disable write protection if enabled
    if(!Rtc.GetIsRunning()) Rtc.SetIsRunning(true); // Start the RTC if not already running

    RtcDateTime now = Rtc.GetDateTime(); // Get the current date and time from the RTC
    
    if(now < compiled) Rtc.SetDateTime(compiled); // Set the RTC's date and time to the compiled timestamp if it's earlier
}

String Get_Date_Time(){
    RtcDateTime now = Rtc.GetDateTime(); // Get the current date and time from the RTC
    char datestring[20]; // Variable to store the formatted date and time string

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"), // Format of the displayed information
            now.Day(), // Day
            now.Month(), // Month
            now.Year(), // Year
            now.Hour(), // Hour
            now.Minute(), // Minutes
            now.Second() ); // Seconds
    return String(datestring); // Return the formatted date and time string
}

String Get_Date_Time_File_Name(){
    RtcDateTime now = Rtc.GetDateTime(); // Get the current date and time from the RTC
    char datestring[20]; // Variable to store the formatted date and time string

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%04u/%02u/%02u %02u:%02u:%02u"), // Format of the displayed information
            now.Year(), // Year
            now.Month(), // Month
            now.Day(), // Day
            now.Hour(), // Hour
            now.Minute(), // Minutes
            now.Second() ); // Seconds
    return String(datestring); // Return the formatted date and time string
}