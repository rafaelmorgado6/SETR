#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include "myEvtLib.h"

/* Event list (not really a list ... but it should be) */
/* Note the static. Manipulation of queue should be made via the interface functions */

// Define the eventQueue (actual allocation of memory)
EventQueue eventQueue;  // Define the eventQueue, which holds the events

/* Event management function implementation */

/* Get next event in queue (removing it) */
int16_t getEVT(EventType *headEvt) {
    if (eventQueue.nevents > 0) {
        *headEvt = eventQueue.events[eventQueue.head];  // Copy the event to the provided pointer
        eventQueue.head = (eventQueue.head + 1) % MAX_EVENTS;  // Move the 'head' to the next event (circular)
        eventQueue.nevents--;
        return MYEVT_OK;  // Event successfully removed
    } else {
        return MYEVT_EMPTY;  // Queue is empty
    }
}

/* Add a new event to the end of the queue */
void addEVT(EventType newevt) {
    if (eventQueue.nevents < MAX_EVENTS) {
        eventQueue.events[eventQueue.tail] = newevt;  // Add the new event at the tail
        eventQueue.tail = (eventQueue.tail + 1) % MAX_EVENTS;  // Move the 'tail' to the next position (circular)
        eventQueue.nevents++;
    } else {
        printf("Event queue is full! Cannot add more events.\n");
    }
}

/* Initialize the event queue */
void initEVT(void) {
    eventQueue.head = 0;   // Start at the first index of the queue
    eventQueue.tail = 0;   // Also start at the first index for inserting new events
    eventQueue.nevents = 0;  // No events in the queue initially
}
