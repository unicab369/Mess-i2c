#include <TimeLib.h>

enum TimeSource {
    TIME_NETWORK, TIME_RTC, TIME_BUILT
};

const char BUILT_DATE[] = __DATE__;
const char BUILT_TIME[] = __TIME__;

class ClockItem {
    uint8_t getMonth(char *month) {
        if (strcmp(month, "Jan") == 0) { return 1; } 
        else if (strcmp(month, "Feb") == 0) { return 2; } 
        else if (strcmp(month, "Mar") == 0) { return 3; } 
        else if (strcmp(month, "Apr") == 0) { return 4; } 
        else if (strcmp(month, "May") == 0) { return 5; } 
        else if (strcmp(month, "Jun") == 0) { return 6; } 
        else if (strcmp(month, "Jul") == 0) { return 7; } 
        else if (strcmp(month, "Aug") == 0) { return 8; } 
        else if (strcmp(month, "Sep") == 0) { return 9; } 
        else if (strcmp(month, "Oct") == 0) { return 10; } 
        else if (strcmp(month, "Nov") == 0) { return 11; } 
        else if (strcmp(month, "Dec") == 0) { return 12; } 
        return 0;
    }

    TimeElements getTime() {
        TimeElements output;
        breakTime(now(), output);
        return output;
    }

    char dateTimeStr[23];
    char dateStr[11];
    TimeElements timeRef;

    public:
        TimeSource source;
        uint32_t getTimeNow() { return now(); }

        void begin() {
            loadBuildTime();
            setTime(timeRef.Hour, timeRef.Minute, timeRef.Second, timeRef.Day, timeRef.Month, timeRef.Year+1970);
            source = TIME_BUILT;
        }

        void loadBuildTime() {
            char month[4];
            int year, day;
            // AppPrint("[Clock]", "BUILT_DATE", BUILT_DATE);
            // AppPrint("[Clock]", "BUILT_TIME", BUILT_TIME);

            sscanf(BUILT_DATE, "%4s %d %d", &month, &day, &year);
            sscanf(BUILT_TIME, "%hhu:%hhu:%hhu", &timeRef.Hour, &timeRef.Minute, &timeRef.Second);
            
            timeRef.Day = day;
            timeRef.Month = getMonth(month);
            timeRef.Year = year-1970;    // offset year
        }

        void configure(time_t time, TimeSource _source) {
            if (time>0 == false) return;
            // AppPrint("[Clock]", __func__);
            setTime(time);
            source = _source;
        }

        String getDate() {
            char str[12];
            TimeElements dt = getTime();
            snprintf_P(str, 12, PSTR("%02u %02u %02u"), dt.Month, dt.Day, dt.Year);
            return String(str);
        }

        char* getDisplay() {
            TimeElements dt = getTime();
            char sourceChar = (source == TIME_NETWORK) ? 'N' : (source == TIME_RTC) ? 'R' : 'B';
            snprintf_P(dateTimeStr, sizeof(dateTimeStr), PSTR("%02u:%02u:%02u %02u-%02u-%02u %c"), 
                        dt.Hour, dt.Minute, dt.Second, dt.Month, dt.Day, dt.Year+1970, sourceChar);
            return dateTimeStr; 
            // return "";
        }

        //! NOTE: year is offset from 1970
        //! NOTE: dateStr is being used as dataPath and can NOT!!! have '/' charactor
        char* getDateTimeStr() {
            TimeElements dt = getTime();
            snprintf_P(dateTimeStr, sizeof(dateTimeStr), PSTR("%02u:%02u:%02u %02u-%02u-%02u"), 
                        dt.Hour, dt.Minute, dt.Second, dt.Month, dt.Day, dt.Year+1970);
            return dateTimeStr; 
            // return "";            
        }

        char* getDateStr() {
            TimeElements dt = getTime();
            snprintf_P(dateStr, sizeof(dateStr), PSTR("%02u-%02u-%02u"), 
                        dt.Month, dt.Day, dt.Year+1970);
            return dateStr;
        }
};