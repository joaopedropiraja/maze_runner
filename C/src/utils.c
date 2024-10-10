#include "utils.h"
#include <unistd.h>

void sleep_ms(int milliseconds) {
    if (milliseconds >= 1000) {
      sleep(milliseconds / 1000);
    }
    
    usleep((milliseconds % 1000) * 1000);
}