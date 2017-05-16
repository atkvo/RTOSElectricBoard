
#ifndef ANIMATE_HPP
#define ANIMATE_HPP

#include <iostream>
#include <stdint.h>

#define AM Animation::getInstance()

using namespace std;

typedef struct anim_params{
    int32_t duration;
    int32_t start_val;
    int32_t end_val;
    int32_t progress;
    int32_t output;
    bool complete;
    void (* curve)(anim_params &params);
    anim_params() { duration = 0; start_val = 0; end_val = 0; progress = 0; curve = NULL;}
    anim_params(int32_t dur, int32_t start,int32_t end,void (* crv)(anim_params &params)) { duration = dur; start_val = start; end_val = end; progress = start; curve = crv; }
} anim_params_t;

typedef struct d_array {
    float *data;
    uint16_t size;
} d_array_t;

typedef uint16_t anim_id;


class Animation {
    static Animation *singleton;
    Animation() {
        cout << "animated" << endl;
        next_animation_id = 0;
    }
    anim_params_t animations[32]; // holds the state for all existing animations
    anim_id next_animation_id;

    public:

    anim_params *getAnimationState(anim_id id);    
    static Animation *getInstance();
    anim_id setAnimationState(int32_t dur, int32_t start, int32_t end, void (*curve)(anim_params_t &state));
    anim_id setAnimationState(anim_params &state);
    void setAnimationState(anim_id id, int32_t dur, int32_t start, int32_t end, void (*curve)(anim_params_t &state));
    void setAnimationState(anim_id id, anim_params &state);   
    void resetAnimation(anim_id id);
    static void animation1(anim_params_t &params);
    static void animation2(anim_params_t &params);
    static void animation3(anim_params_t &params);
    static void animation4(anim_params_t &params);
    static void animation5(anim_params_t &params);
    void animate();
    void animate(anim_id id);




};


#endif
