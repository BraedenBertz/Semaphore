/*DO NOT MODIFY THESE OR YOUR TESTS WILL FAIL*/
#ifndef GLOBAL_H
#define GLOBAL_H
#include <stdbool.h>
int tofu = 0;
int meat = 0;
int bread = 0;
int carnivores = 0;
int vegetarians = 0;
enum STATUS {
    NEUTRAL,
    CARNIVORE,
    VEGETARIAN,
    TRANSITION_TO_CARNIVORE,
    TRANSITION_TO_VEGETARIAN,
};
enum STATUS status = NEUTRAL;

void create_sandwich(bool veggie);

void assemble_sandwich(char c);
#endif
/*END*/