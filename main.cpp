#include "Turtle.h"
/**
 * Sample turtle graphics program
 */
using namespace turtle;
int main() {
    lt(90);     // 90 degree left turn
    fd(50);     // 50 pixel move forward
    rt(90);     // 90 degree right turn
    bk(25);     // 25 pixel move backwards
    fd(50);     // 50 pixel move forward
    sleep_for(1s);
    ht();               // hide turtle. (Other commands: st = show turtle, pu = pen up, pd = pen down)
    sleep_for(3s);
    return 0;
}