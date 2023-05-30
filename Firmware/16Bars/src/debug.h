
#define DEBUG 1
#if DEBUG == 1
#define debug(msg) Serial.print(msg)
#define debugln(msg) Serial.println(msg)
#else
#define debug(msg)
#define debugln(msg)
#endif