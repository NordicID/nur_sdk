#include <NurApi.h>

// Example: Configure GPIO
int ConfigureGPIO(HANDLE hApi, int level)
{
	struct NUR_GPIO_CONFIG cfg;
	cfg.count = 2;

	// Configure first entry (GPIO1)
	cfg.entries[0].available = TRUE;
	cfg.entries[0].enabled = TRUE;
	cfg.entries[0].type = NUR_GPIO_TYPE_INPUT;	 // Config GPIO as input and
	cfg.entries[0].action = NUR_GPIO_ACT_NOTIFY; // send notification to host
	cfg.entries[0].edge = NUR_GPIO_EDGE_FALLING; // on falling edge.

	// Configure second entry (GPIO2)
	cfg.entries[1].available = TRUE;
	cfg.entries[1].enabled = TRUE;
	cfg.entries[1].type = NUR_GPIO_TYPE_OUTPUT;	 // Config GPIO as output and
	cfg.entries[1].edge = NUR_GPIO_EDGE_RISING;  // high by default.
	cfg.entries[1].action = NUR_GPIO_ACT_NONE;	 // NOP

	return NurApiSetGPIOConfig(hApi, &cfg, sizeof(cfg));
}

// Example: Get module GPIO1 state
int GetGPIO1State(HANDLE hApi, BOOL *state)
{
	struct NUR_GPIO_STATUS status;
	int ret = NurApiGetGPIOStatus(hApi, NUR_GPIO1, &status, sizeof(status));
	if (ret == NUR_NO_ERROR) {
		*state = status.state;
	}
	return ret;
}

// Example: Set module GPIO2 state
int SetGPIO2State(HANDLE hApi, BOOL state)
{
	return NurApiSetGPIOStatus(hApi, NUR_GPIO2, state);
}
