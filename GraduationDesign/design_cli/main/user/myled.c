/**
  ******************************************************************************
  * @file	: myled.c
  * @brief	: 
  * Details
  *
  * Created on			: 2019年3月7日
  * Author				: 123456
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "myled.h"
#include "function.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
const char* TAG_LED = "MYLED";

uint16_t fan_Speed = FAN_S;
//ledc_set_duty(LEDC_HS_MODE, LEDC_HS_CH0_CHANNEL, LEDC_TEST_DUTY);
/* USER CODE BEGIN PV */

void set_fan(int pow){
	fan_Speed = pow;
	PWM_Set(fan_Speed);
}

void open_fan(void){
	PWM_Set(FAN_S);
}

void close_fan(void){
	PWM_Set(0);
}

void up_fan(void){
	fan_Speed += 100;
	if(fan_Speed > FAN_MAX)
		fan_Speed = FAN_MAX;
	if(fan_Speed < FAN_S)
		fan_Speed = FAN_S;
	PWM_Set(fan_Speed);
}

void down_fan(void){
	fan_Speed -= 100;
	if(fan_Speed < FAN_critical)
		fan_Speed = 0;
	PWM_Set(fan_Speed);
}

void PWM_Set(uint16_t num)
{
	ledc_set_fade_with_time(PWM_MODE, PWM_CHANNEL, num, 0);
	ledc_fade_start(PWM_MODE, PWM_CHANNEL,0);
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/


/* USER CODE BEGIN PFP */
void myled_init(void)
{
//	int ch;
	 /*
	     * Prepare and set configuration of timers
	     * that will be used by LED Controller
	     */
	ledc_timer_config_t ledc_timer = {
			.duty_resolution = LEDC_TIMER_10_BIT, // resolution of PWM duty
			.freq_hz = 500,                      // frequency of PWM signal
			.speed_mode = PWM_MODE,           // timer mode
			.timer_num = PWM_TIMER            // timer index
			};
	// Set configuration of timer0 for low speed channels
	ledc_timer_config(&ledc_timer);

	gpio_set_direction(PWM_GPIO,GPIO_MODE_OUTPUT);
	/*
	     * Prepare individual configuration
	     * for each channel of LED Controller
	     * by selecting:
	     * - controller's channel number
	     * - output duty cycle, set initially to 0
	     * - GPIO number where LED is connected to
	     * - speed mode, either high or low
	     * - timer servicing selected channel
	     *   Note: if different channels use one timer,
	     *         then frequency and bit_num of these channels
	     *         will be the same
	     */
	ledc_channel_config_t ledc_channel = {
	            .channel    = PWM_CHANNEL,
	            .duty       = 0,
	            .gpio_num   = PWM_GPIO,
	            .speed_mode = PWM_MODE,
	            .timer_sel  = PWM_TIMER
	        };
	        // Set LED Controller with previously prepared configuration
	        ledc_channel_config(&ledc_channel);
	        // Initialize fade service.
	            ledc_fade_func_install(0);
	            PWM_Set(PWM_OFF);

//	            xTaskCreate(&fan_test, "fan_test", 4096, NULL, 15, NULL);
}
/* USER CODE END PFP */

