/* **************************************************************
 * Paulo Pedreiras, 2025/03, SETR
 *
 * Skeleton for the implementation of an event queue library
 *
 * At this moment stores just one event ... no queue!
 *
 * Implementing a FIFO queue is homework!
 * 
 ****************************************************************/

 #ifndef MY_EVT_LIB
 #define MY_EVT_LIB
 
 /* Error codes */
 #define MYEVT_OK      0   /* Success */
 #define MYEVT_EMPTY   -1  /* Event queue is empty */
 
 /* Define events */
 typedef struct {
	 uint16_t event_t;     /* The event */
	 uint16_t event_arg;   /* Arguments for the event */
 } EventType;
 
 #define MAX_EVENTS 10  // Maximum size of the event queue
 
 // EventQueue structure for storing events
 typedef struct {
	 EventType events[MAX_EVENTS];  // Array to store events
	 uint16_t head;                 // Head index of the queue
	 uint16_t tail;                 // Tail index of the queue
	 uint16_t nevents;             // Number of events in the queue
 } EventQueue;
 
 extern EventQueue eventQueue;  // Declaration of the eventQueue (extern means it's defined elsewhere)
 
 // Event queue interface functions
 int16_t getEVT(EventType *evt);  // Get the next event from the queue
 void addEVT(EventType evt);      // Add an event to the queue
 void initEVT(void);              // Initialize the event queue
 
 #endif
 