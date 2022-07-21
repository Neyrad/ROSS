//The header file template for a ROSS model
//This file includes:
// - the state and message structs
// - extern'ed command line arguments
// - custom mapping function prototypes (if needed)
// - any other needed structs, enums, unions, or #defines

#ifndef _model_h
#define _model_h

#include "ross.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <assert.h>

#define MAX_ROOM_HEIGHT 100
#define MAX_ROOM_LENGTH 100

#define MAX_COMMENT_LENGTH 1000

struct room {
	int data[MAX_ROOM_HEIGHT][MAX_ROOM_LENGTH];
	int height;
	int length;
};


//Example enumeration of message type... could also use #defines
typedef enum {
  HELLO,
  GOODBYE,
} message_type;

typedef enum {
  HELLO_,
  GOODBYE_,
} lp_type;

//Message struct
//   this contains all data sent in an event
typedef struct {
  message_type type;
  double contents;
  tw_lpid sender;
} message;


//State struct
//   this defines the state of each LP
typedef struct {
  int rcvd_count_H;
  int rcvd_count_G;
  lp_type type; 
  double value;
} state;


//Command Line Argument declarations
extern unsigned int setting_1;

//Global variables used by both main and driver
// - this defines the LP types
extern tw_lptype model_lps[];

//Function Declarations
// defined in model_driver.c:
extern void model_init(state *s, tw_lp *lp);
extern void model_event(state *s, tw_bf *bf, message *in_msg, tw_lp *lp);
extern void model_event_reverse(state *s, tw_bf *bf, message *in_msg, tw_lp *lp);
extern void model_final(state *s, tw_lp *lp);
// defined in model_map.c:
extern tw_peid model_map(tw_lpid gid);
extern tw_lpid model_typemap (tw_lpid gid);

/*
//Custom mapping prototypes
void model_cutom_mapping(void);
tw_lp * model_mapping_to_lp(tw_lpid lpid);
tw_peid model_map(tw_lpid gid);
*/

extern void parse(struct room* storage, char* path);


#endif
