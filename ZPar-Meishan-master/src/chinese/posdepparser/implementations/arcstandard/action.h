#ifndef POSDEPPARSER_ACTION_H
#define POSDEPPARSER_ACTION_H

namespace action {

// SH AL AR [AL+LABEL] [AR+LABEL] [SH+POS]
enum STACK_ACTION { NO_ACTION=0, SHIFT_IN, ARC_LEFT_IN, ARC_RIGHT_IN, POP_WORD, SHIFT, ARC_LEFT, ARC_RIGHT, POP_ROOT, IDLE, LABEL_AL_FIRST, LABEL_AR_FIRST=LABEL_AL_FIRST+DEP_LABEL_COUNT-1, LABEL_SH_FIRST=LABEL_AR_FIRST+DEP_LABEL_COUNT-1, MAX=LABEL_SH_FIRST+POS_LABEL_COUNT-1 };


static unsigned encodeAction(const STACK_ACTION &action, const unsigned &label=0) {
   if (action==ARC_LEFT) {
	  assert(label<DEP_LABEL_COUNT);
      if (label==0)
         return ARC_LEFT;
      else
         return LABEL_AL_FIRST+label-1;
   }
   else if (action==ARC_RIGHT) {
	  assert(label<DEP_LABEL_COUNT);
      if (label==0)
         return ARC_RIGHT;
      else
         return LABEL_AR_FIRST+label-1;
   }
   else if (action==SHIFT)
   {
	 assert(label<POS_LABEL_COUNT);
	 return LABEL_SH_FIRST+label-1;
   }
   else
      assert(label==0);

   return action;
}
static unsigned getUnlabeledAction(const unsigned &action ) {
   assert(action<MAX);
   if (action<LABEL_AL_FIRST)
      return action;
   else if (action<LABEL_AR_FIRST)
      return ARC_LEFT;
   else if (action<LABEL_SH_FIRST)
      return ARC_RIGHT;
   else
	   return SHIFT;
}
static unsigned getLabel(const unsigned &action) {
   assert(action<MAX);
   if (action < LABEL_AL_FIRST)
      return 0;
   else if (action < LABEL_AR_FIRST)
      return action-LABEL_AL_FIRST+1;
   else if (action<LABEL_SH_FIRST)
      return action-LABEL_AR_FIRST+1;
   else
	  return action-LABEL_SH_FIRST+1;
}


struct CScoredAction {
   unsigned action;
   SCORE_TYPE score;
public:
   bool operator < (const CScoredAction &a) const {
      return score < a.score;
   }
   bool operator > (const CScoredAction &a) const {
      return score > a.score;
   }
};

}//namespace
#endif
