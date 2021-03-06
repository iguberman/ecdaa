/******************************************************************************
 *
 * Copyright 2017 Xaptum, Inc.
 * 
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 * 
 *        http://www.apache.org/licenses/LICENSE-2.0
 * 
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License
 *
 *****************************************************************************/

#include "ecdaa-test-utils.h"

#include "../src/amcl-extensions/ecp_BN254.h"

#include <ecdaa/member_keypair_BN254.h>
#include <ecdaa/prng.h>

static void member_secret_is_valid();
static void member_public_is_valid();
static void member_proof_checks();

int main()
{
    member_secret_is_valid();
    member_proof_checks();
    member_public_is_valid();
}

void member_secret_is_valid()
{
    printf("Starting context::member_secret_is_valid...\n");

    struct ecdaa_member_secret_key_BN254 sk1;
    struct ecdaa_member_public_key_BN254 pk1;
    uint8_t nonce[32] = {0};

    struct ecdaa_prng prng;
    TEST_ASSERT(0 == ecdaa_prng_init(&prng));

    ecdaa_member_key_pair_BN254_generate(&pk1, &sk1, nonce, sizeof(nonce), &prng);

    TEST_ASSERT(!pk1.Q.inf);

    struct ecdaa_member_secret_key_BN254 sk2;
    struct ecdaa_member_public_key_BN254 pk2;
    ecdaa_member_key_pair_BN254_generate(&pk2, &sk2, nonce, sizeof(nonce), &prng);

    TEST_ASSERT(BIG_256_56_comp(sk1.sk, sk2.sk) != 0);

    ecdaa_prng_free(&prng);

    printf("\tsuccess\n");
}

void member_public_is_valid()
{
    printf("Starting context::member_public_is_valid...\n");

    struct ecdaa_prng prng;
    TEST_ASSERT(0 == ecdaa_prng_init(&prng));

    struct ecdaa_member_secret_key_BN254 sk;
    struct ecdaa_member_public_key_BN254 pk;
    uint8_t nonce[32] = {0};
    ecdaa_member_key_pair_BN254_generate(&pk, &sk, nonce, sizeof(nonce), &prng);

    ecdaa_prng_free(&prng);

    printf("\tsuccess\n");
}

void member_proof_checks()
{
    // TODO: Check signature (c, s) in member's key
}
