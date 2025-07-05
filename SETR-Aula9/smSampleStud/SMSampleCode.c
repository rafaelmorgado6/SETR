/* **************************************************************
 * Paulo Pedreiras, 2025/03, SETR
 *
 * Demo code of a simple SM - students version
 *
 * Limitations:
 *      Misses several events
 *      At this moment stores just one event ... no queue!
 *      Implementing a FIFO queue is homework!
 *
 ****************************************************************/

 #include <stdio.h>
 #include <stdlib.h>
 #include <pthread.h>
 #include <unistd.h>
 #include <stdbool.h>
 #include <stdint.h>
 #include <errno.h>
 #include "evtLib/myEvtLib.h"
 
 /*
  * State Machine definitions and variables
  */
 
 /* Intensity levels of lamp */
 #define MAX_INTENSITY 5
 #define MIN_INTENSITY 1
 
 /* Event types */
 #define EVT_NO        0      /* No events */
 #define EVT_ONOFF     1      /* On/Off pressed */
 #define EVT_U         2      /* Up key pressed */
 #define EVT_D         3      /* Down key pressed */
 
 /* Define the system states */
 typedef enum {
	 OFF,
	 ON
 } LightState;
 
 /* Current state struct */
 typedef struct {
	 LightState state;
	 int intensity;    
 } LightControl;
 
 /* System state variable */
 LightControl light = {OFF, 0};
 
 /* Flag to control the execution of threads */
 bool running = true;
 
 /* HW emulation layer.
  * Actuation function prototype. 
  * In a real system could be e.g. setting the PWM duty cycle of PWM 
  * channel to set the intensity. This is also an interface function 
  * that depends on the hardware used 
  */
 void SetLamp(LightControl light);
 
 /* HW emulation layer.
  * Gets the inputs from the environment. In reality it would be
  * interrupts associated with buttons pressed; in this case are inputs
  * from a user. 
  */
 void *user_input_thread(void *arg); 
 
 /*
  * Runs the state machine
 */
 void *state_machine_thread(void *arg) {
	 EventType evt;
 
	 while (running) {            
		 /* Get next event, if any */
		 if (getEVT(&evt) != MYEVT_OK) {
			 evt.event_t = EVT_NO;        
		 }        
		 
		 switch (light.state) {            
			 case OFF:                
				 if (evt.event_t == EVT_ONOFF) {
					 light.state = ON;
					 light.intensity = 1;                    
				 }
				 
				 SetLamp(light);
				 break;
		 
			 case ON:                
				 if (evt.event_t == EVT_ONOFF) {
					 light.state = OFF;
					 light.intensity = 0;                    
				 } else if (evt.event_t == EVT_U) {
					 if (light.intensity < MAX_INTENSITY) {
						 light.intensity++;
					 } else {
						 printf("Lamp is already at maximum intensity\n");
					 }
				 } else if (evt.event_t == EVT_D) {
					 if (light.intensity > MIN_INTENSITY) {
						 light.intensity--;
					 } else {
						 printf("Lamp is already at minimum intensity\n");
					 }
				 };
				 
				 SetLamp(light);
				 break;                                
		 
			 default:
				 printf("Exception - invalid state !!!!\n");
				 break;
		 }     
		 
		 /* Allow other threads to run */
		 sleep(1);
	 }
	 return NULL;
 }
 
 /*
  * Main function
  */
 int main() {
	 pthread_t state_thread, input_thread;
   
	 /* Init event queue */
	 initEVT();
   
	 /* Create threads */
	 pthread_create(&state_thread, NULL, state_machine_thread, NULL);
	 pthread_create(&input_thread, NULL, user_input_thread, NULL);
 
	 /* Terminating the input thread finishes the execution of the SM */
	 pthread_join(input_thread, NULL);
	 running = false;
	 pthread_join(state_thread, NULL);
	 
	 /* All done! */
	 return 0;
 }
 
 void SetLamp(LightControl light) {
	 if (light.state == ON) {
		 printf("Lamp is ON and intensity is %d \n", light.intensity);
	 } else {
		 printf("Lamp is OFF\n");
	 }    
 }
 
 void *user_input_thread(void *arg) {
	 char input;
	 int retval;
	 EventType evt = {EVT_NO, 0};
	 
	 while (running) {
		 printf("Enter command: (o: On/Off, +: Increase, -: Decrease, q: Quit)\n");
		 retval = scanf(" %c", &input);
		 
		 if (retval == EOF) {
			 printf("scanf() failed. Error is %d\n\r", errno);
		 }
		 
		 /* Set input type and args (none in this case) */
		 switch (input) {
			 case 'o':  /* On-Off button */
				 printf("ON/OFF button\n");
				 evt.event_t = EVT_ONOFF;
				 break;
				 
			 case '+':  /* Up button */
				 printf("Up button\n");
				 evt.event_t = EVT_U;
				 break;
				 
			 case '-':  /* Down button */
				 printf("Down button\n");
				 evt.event_t = EVT_D;
				 break;
				 
			 case 'q':  /* Quit button */
				 printf("Terminating ...\n");
				 running = false;
				 break;
				 
			 default:  /* Command not recognized  */
				 printf("Invalid command!\n");
				 evt.event_t = EVT_NO;
				 break;
		 }
		 
		 /* Add event to queue */
		 addEVT(evt);
				 
		 /* Allow other tasks to run */
		 sleep(1);
	 }
	 return NULL;
 }
 