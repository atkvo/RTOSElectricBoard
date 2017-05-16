
#include "animate.hpp"
#include <math.h>
#include <stdio.h>

Animation *Animation::singleton = NULL;


Animation *Animation::getInstance() {
    if (!singleton) {   
        singleton = new Animation();
    }
    return singleton;
}

anim_params *Animation::getAnimationState(anim_id id) {
    return &animations[id];
}

// assumption of animation running 60Hz
// each exectution of animate() function is 1 step in animation

// configure duration, start val, destination val

anim_id Animation::setAnimationState(int32_t dur, int32_t start, int32_t end, void (*curve)(anim_params_t &state)) {
    anim_id id = next_animation_id;
    animations[id].duration = dur;
    animations[id].start_val = start;
    animations[id].end_val = end;
    animations[id].progress = start;
    animations[id].output = 0;
    animations[id].curve = curve;
    next_animation_id++;
    return id;
}
anim_id Animation::setAnimationState(anim_params &state) {
    anim_id id = next_animation_id;
    animations[id] = state;
    next_animation_id++;

    return id;
}

void Animation::setAnimationState(anim_id id, int32_t dur, int32_t start, int32_t end, void (*curve)(anim_params_t &state)) {
    if (id < next_animation_id) {
        animations[id].duration = dur;
        animations[id].start_val = start;
        animations[id].end_val = end;
        animations[id].progress = start;
        animations[id].output = 0;
        animations[id].curve = curve;
    } else {
        printf("invalid animation id\n");
    }
}
void Animation::setAnimationState(anim_id id, anim_params &state) {
    if (id < next_animation_id) {
        animations[id] = state;
    } else {
        printf("invalid animation id\n");
    }
}


void Animation::resetAnimation(anim_id id) {
    if (id < next_animation_id) {
    animations[id].progress = 0;
    animations[id].complete = false;
    } else {
        printf("invalid animation id\n");
    }
}

void Animation::animation1(anim_params_t &params) {
    params.output = 5.0f * sin(params.progress);
    if (params.progress != params.duration) {
        params.progress++;
    }
}

void Animation::animation2(anim_params_t &params) {
    static float anim_percent;    
    static float base_anim;
    static float p = 0.2f;
    anim_percent = (float)params.progress/(float)params.duration;
    base_anim = pow(2,-10.0f * anim_percent) * sin((anim_percent-p/4.0f)*(2.0f * M_PI)/p) + 1.0f;;
    params.output  = params.start_val + base_anim * ( params.end_val - params.start_val );
    if (params.progress < params.duration) {
        params.progress++;
    }
}

// cubic ease in and out
void Animation::animation3(anim_params_t &params) {
    static float anim_percent;    
    static float base_anim;

    anim_percent = (float)params.progress/(float)params.duration;
    if (anim_percent < 0.5f) {
        base_anim = (pow(anim_percent*2.0f,3.0f)/2.0f);
    } else {
        base_anim = 1.0f - (pow((1.0f-anim_percent)*2.0f,3.0f)/2.0f);
    }
    // printf("EIO: prog: %3d dur: %3d st: %3d end: %3d base_anim: %4.2f  anim_percent: %4.2f output: %3d\n",    params.progress,
    //                                                                                                                     params.duration,
    //                                                                                                                     params.start_val,
    //                                                                                                                     params.end_val,
    //                                                                                                                     base_anim,
    //                                                                                                                     anim_percent,
    //                                                                                                                     params.output);
    params.output  = params.start_val + base_anim * ( params.end_val - params.start_val );
    if (params.progress < params.duration) {
        params.progress++;
    }
}

// cubic ease in
void Animation::animation4(anim_params_t &params) {
    static float anim_percent;    
    static float base_anim;

    anim_percent = (float)params.progress/(float)params.duration;
    base_anim = pow(anim_percent,3.0f);
    params.output  = params.start_val + base_anim * ( params.end_val - params.start_val );
    if (params.progress < params.duration) {
        params.progress++;
    }
}

// cubic ease out
void Animation::animation5(anim_params_t &params) {
    static float anim_percent;    
    static float base_anim;

    anim_percent = (float)params.progress/(float)params.duration;
    base_anim = 1.0f - pow(1.0f-anim_percent,3.0f);
    params.output  = params.start_val + base_anim * ( params.end_val - params.start_val );
    if (params.progress < params.duration) {
        params.progress++;
    }
}

void Animation::animate() {
    // increment all animation curves by 1 and marke those that are done as complete;
}

void Animation::animate(anim_id id) {
    if (animations[id].progress < animations[id].duration) {
            (*animations[id].curve)(animations[id]);
    } else {
        animations[id].complete = true;
    }
}