//
// Created by Irina Guberman on 9/26/17.
//
#include "theft.h"
#include "test_theft.h"
#include "greatest.h"
#include <ecdaa.h>


static enum theft_trial_res
prop_groupkey_generated(struct theft *t, void *arg1) {

    (void)t;
    (void) arg1;

    // Initialize PRNG
    struct ecdaa_prng rng;
    if (0 != ecdaa_prng_init(&rng)) {
        fputs("Error initializing ecdaa_prng\n", stderr);
        return THEFT_TRIAL_ERROR;
    }

    // Create issuer key-pair
    struct ecdaa_issuer_public_key_BN254 ipk;
    struct ecdaa_issuer_secret_key_BN254 isk;
    if (0 != ecdaa_issuer_key_pair_BN254_generate(&ipk, &isk, &rng)) {
        fprintf(stderr, "Error generating issuer key-pair\n");
        return THEFT_TRIAL_ERROR;
    }

    return THEFT_TRIAL_PASS;
}

static enum theft_alloc_res
uint8_alloc(struct theft *t, void *env, void **output) {
//    uint32_t *n = malloc(sizeof(uint32_t));
//    if (n == NULL) { return THEFT_ALLOC_ERROR; }
//    *n = (uint32_t)(theft_random(t) & 0xFFFFFFFF);
    (void)t; (void)env;
    uint8_t *n = malloc(sizeof(uint8_t) * 1024);
    *output = n;
    return THEFT_ALLOC_OK;
}

static void uint8_free(void *p, void *env) {
    (void)env;
    free(p);
}

static void uint8_print(FILE *f, const void *p, void *env) {
    (void)env;
    fprintf(f, "%u", *(uint8_t *)p);
}

static struct theft_type_info static_buffer_info = {
        .alloc = uint8_alloc,
        .free = uint8_free,
        .print = uint8_print,
        .autoshrink_config = {
                .enable = false,
        },
};

TEST test_groupkey_generation(void) {
//    struct repeat_once_env env = { .fail = false };

    /* Get a seed based on the current time */
    theft_seed seed = theft_seed_of_time();

    /* Property test configuration.
     * Note that the number of type_info struct pointers in
     * the .type_info field MUST match the field number
     * for the property function (here, prop1). */
    struct theft_run_config config = {
            .name = __func__,
            .prop1 = prop_groupkey_generated,
            .type_info = { &static_buffer_info },
            .seed = seed,
    };

    /* Run the property test. */
    enum theft_run_res res = theft_run(&config);
    return res == THEFT_RUN_PASS;
}

SUITE(groupkeygen) {

    // Various tests for single autoshrinking steps, with an injected PRNG
    RUN_TEST(test_groupkey_generation);
}

