#include <tT.h>
#include <env.h>
#include <stdio.h>
#include <stdlib.h>

#define PERIOD_TEMP 1000
#define PERIOD_HUM 1500

typedef struct {
	tt_object_t object;
    int temperature;
    int humidity;
    int fan_speed;
} controller_t;


static env_result_t controller_evaluate(controller_t *self, void *arg)
{
	// controller rules
	if (self->temperature > 30 || self->humidity > 60) {
        self->fan_speed = 3; // Fast
    } else if (self->temperature > 25 || self->humidity > 50) {
        self->fan_speed = 2; // Middle
    } else {
        self->fan_speed = 1; // Slow
    }
	
	printf("fan speed changed to: %d (T:%d, H:%d)\n", self->fan_speed, 
									self->temperature, self->humidity);
	
	return 0;
}

static env_result_t sensor_temp(controller_t *self, void *arg)
{
	/* reading temperature from environment ...
	   here, we change it randomely with a delta in [-5,5] */
	   
    // generate a random delta between -5 and 5
	int delta = (rand() % 11) - 5; 

	// update temperature with the delta
	self->temperature += delta;

	// keep temperature within a realistic environment bounds
	if (self->temperature < 10) self->temperature = 10; // 10°C
	if (self->temperature > 45) self->temperature = 45; // 45°C
	
	printf("current temperature is: %d°C \n", self->temperature);
	
	// call the controller evaluation
    TT_ASYNC(self, controller_evaluate, TT_ARGS_NONE);
	
	// call periodic task sensor_temp
    TT_AFTER(ENV_MSEC(PERIOD_TEMP), self, sensor_temp, TT_ARGS_NONE);
	
	return 0;
}

static env_result_t sensor_hum(controller_t *self, void *arg)
{
	/* reading humidity from environment ...
	   here, we change it randomely with a delta in [-7,7] */
	   
    // generate a random delta between -7 and 7
	int delta = (rand() % 15) - 7; 

	// update temperature with the delta
	self->humidity += delta;

	// keep humidity within a realistic environment bounds
	if (self->humidity < 0) self->humidity = 0; // 0%
	if (self->humidity > 100) self->humidity = 100; // 100%
	
	printf("current humidity is: %d%% \n", self->humidity);
	
	// call the controller evaluation
    TT_ASYNC(self, controller_evaluate, TT_ARGS_NONE);
	
	// call periodic task sensor_hum
    TT_AFTER(ENV_MSEC(PERIOD_HUM), self, sensor_hum, TT_ARGS_NONE);
	
	return 0;
}


void init(void)
{
	// default (temp:22  hum:40  fan:1);
	static controller_t controller = { tt_object(), 22, 40, 1 }; 

	
	// call sensor_temp
	TT_AFTER(ENV_MSEC(PERIOD_TEMP),&controller, sensor_temp, TT_ARGS_NONE);
	
	// call sensor_hum
	TT_AFTER(ENV_MSEC(PERIOD_HUM),&controller, sensor_hum, TT_ARGS_NONE);
}

ENV_STARTUP(init);
