#ifndef _SENSOREXAMPLE_H_
#define _SENSOREXAMPLE_H_ 1

#include <NurApi.h>

/// <summary>
/// Sets the sensor configuration.
/// </summary>
/// <param name="hApi">The hAPI.</param>
/// <param name="sensorAction">The sensor action.</param>
/// <param name="enabled">if set to <c>true</c> [enabled].</param>
/// <returns></returns>
int SetSensorConfig(HANDLE hApi, int sensorAction, bool enabled);

#endif
