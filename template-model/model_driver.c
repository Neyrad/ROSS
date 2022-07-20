//The C driver file for a ROSS model
//This file includes:
// - an initialization function for each LP type
// - a forward event function for each LP type
// - a reverse event function for each LP type
// - a finalization function for each LP type

//Includes
#include <stdio.h>

#include "ross.h"
#include "model.h"

//Helper Functions
void SWAP (double *a, double *b) {
  double tmp = *a;
  *a = *b;
  *b = tmp;
}


//Init function
// - called once for each LP
// ! LP can only send messages to itself during init !
void model_init (state *s, tw_lp *lp) {
  if (lp->gid % 2 == 0) {
	s->type = HELLO_;
	printf("HELLO is initialized | ");
  } else if (lp->gid % 2 == 1) {
	s->type = GOODBYE_;
	printf("GOODBYE is initialized | ");
  }
  int self = lp->gid;

  // init state data
  s->rcvd_count_H = 0;
  s->rcvd_count_G = 0;
  s->value = -1;

  // Init message to myself
  tw_event *e = tw_event_new(self, 1, lp);
  message *msg = tw_event_data(e);
  switch(s->type)
  {
  	case HELLO:
		msg->type = HELLO_;
		break;
  	case GOODBYE:
		msg->type = GOODBYE_;
		break;
	default:
		printf("Invalid s->type = %d\n", s->type);
  }
  msg->contents = tw_rand_unif(lp->rng);
  msg->sender = self;
  tw_event_send(e);
}

//Forward event handler
void model_event (state *s, tw_bf *bf, message *in_msg, tw_lp *lp) {
  int self = lp->gid;

  // initialize the bit field
  *(int *) bf = (int) 0;

  // update the current state
  // however, save the old value in the 'reverse' message
  SWAP(&(s->value), &(in_msg->contents));

  // handle the message
  switch (in_msg->type) {
    case HELLO :
    {
      s->rcvd_count_H++;
      break;
    }
    case GOODBYE :
    {
      s->rcvd_count_G++;
      break;
    }
    default :
      printf("Unhandeled forward message type %d\n", in_msg->type);
  }

  tw_event *e = tw_event_new(self, 1, lp);
  message *msg = tw_event_data(e);
  //# choose message type
  if (s->type == HELLO_) {
    msg->type = HELLO;
  } else {
    msg->type = GOODBYE;
  }
  msg->contents = tw_rand_unif(lp->rng);
  msg->sender = self;
  tw_event_send(e);
}

//Reverse Event Handler
void model_event_reverse (state *s, tw_bf *bf, message *in_msg, tw_lp *lp) {
  int self = lp->gid;

  // undo the state update using the value stored in the 'reverse' message
  SWAP(&(s->value), &(in_msg->contents));

  // handle the message
  switch (in_msg->type) {
    case HELLO :
    {
      s->rcvd_count_H--;
      break;
    }
    case GOODBYE :
    {
      s->rcvd_count_G--;
      break;
    }
    default :
      printf("Unhandeled reverse message type %d\n", in_msg->type);
  }

  // don't forget to undo all rng calls
  tw_rand_reverse_unif(lp->rng);
}

//report any final statistics for this LP
void model_final (state *s, tw_lp *lp){
  int self = lp->gid;
  if      (s->type == HELLO_)   printf("%d handled %d Hello messages\n",   self, s->rcvd_count_H);
  else if (s->type == GOODBYE_) printf("%d handled %d Goodbye messages\n", self, s->rcvd_count_G);
}
