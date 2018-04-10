#include <NurAPI.h>

/// <summary>
/// Sets the sensor configuration.
/// </summary>
/// <param name="hApi">The hAPI.</param>
/// <param name="sensorAction">The sensor action.</param>
/// <param name="enabled">if set to <c>true</c> [enabled].</param>
/// <returns></returns>
int SetSensorConfig(HANDLE hApi, int sensorAction, bool enabled)
{
	struct NUR_SENSOR_CONFIG sensors;
	sensors.lightAction = sensorAction;
	sensors.lightEnabled = enabled;
	sensors.tapAction = sensorAction;
	sensors.tapEnabled = enabled;
	return NurApiSetSensorConfig(hApi, &sensors, sizeof(sensors));
}
