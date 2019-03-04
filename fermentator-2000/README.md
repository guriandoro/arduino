# FERMENTATOR 2000

Variables to tune:
```
// Define low and high temperatures
#define LOW_TEMP 35
#define HIGH_TEMP 40
```
And optionally:
```
#define LIGHT_ON_SECS 600
#define FAN_ON_SECS 5
```

The light relay will turn ON for LIGHT_ON_SECS when (temp < LOW_TEMP).
The fan relay will turn ON for FAN_ON_SECS when (temp > HIGH_TEMP).

![alt text](https://raw.githubusercontent.com/guriandoro/arduino/master/fermentator-2000/fermentator-2000.JPG)
