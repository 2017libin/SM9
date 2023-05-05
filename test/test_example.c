#include <stdio.h>
#include "fp.h"
#include "debug.h"
#include "fp_low.h"

void test_add(){
    fp_t a = {0xD5FC11967BE65333, 0x780272354F8B78F4, 0xF300000002A3A6F2, 0x0000000000000000};
    fp_t b = {0x4C949C7FA2A96686, 0x57D778A9F8FF4C8A, 0x711E5F99520347CC, 0x2D40A38CF6983351};
    fp_t c;

    fp_addn_low(c, a, b);
    fp_print(c);
    fp_add(c, a, b);
    fp_print(c);

    PERFORMANCE_TEST_NEW("fp_addn_low", fp_addn_low(c, a, b));
    PERFORMANCE_TEST_NEW("fp_add_basic", fp_add_basic(c, a, b));
}

int main(){
    test_add();
    return 0;
}