#include <Stepper.h>



boolean PPTJustStartedFiring = true;

void extend() {
  s.step(100*reduction_ratio);
}

void retract() {
  s.step(-100*reduction_ratio);
}
