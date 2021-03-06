#pragma once

#include <stdint.h>
#include <stdio.h>
#include <complex.h>
#include <liquid/liquid.h>
#include <pthread.h>

#include "acquire.h"
#include "decode.h"
#include "frame.h"
#include "output.h"
#include "sync.h"

typedef struct input_t
{
    output_t *output;
    FILE *outfp;

    agc_crcf agc;
    firdecim_crcf filter;
    resamp_crcf resamp;
    float resamp_rate;
    float complex *buffer;
    unsigned int avail, used, skip;

    pthread_t worker_thread;
    pthread_cond_t cond;
    pthread_mutex_t mutex;

    acquire_t acq;
    decode_t decode;
    frame_t frame;
    sync_t sync;
} input_t;

void input_init(input_t *st, output_t *output, unsigned int program, FILE *outfp);
void input_cb(uint8_t *, uint32_t, void *);
void input_rate_adjust(input_t *st, float adj);
void input_set_skip(input_t *st, unsigned int skip);
void input_wait(input_t *st);
void input_pdu_push(input_t *st, uint8_t *pdu, unsigned int len);
