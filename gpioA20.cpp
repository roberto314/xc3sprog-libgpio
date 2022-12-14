#include "gpioA20.h"

IO_GPIOD_A20::IO_GPIOD_A20()
 : IOMatrixA20(18, 23, 24, 25) // TMS, TCK, TDI, TDO
{
}
// Pins used on Pine64: - does not work!
// all on Pi-2 40-pin connector
// 75 - PC4 - Pin 32 - TDO
// 74 - PC6 - Pin 36 - TDI
// 70 - PC10 - Pin 38 - TCK
// 68 - PC11 - Pin 40 - TMS

// Pins used on Olimexino: - works
// all on GPIO-3 40-pin connector
// 248 - PH24 - Pin 34 - TDO
// 249 - PH25 - Pin 36 - TDI
// 250 - PH26 - Pin 38 - TCK
// 251 - PH27 - Pin 40 - TMS