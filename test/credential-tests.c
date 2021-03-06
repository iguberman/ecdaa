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

#include <ecdaa/credential_BN254.h>
#include <ecdaa/member_keypair_BN254.h>
#include <ecdaa/issuer_keypair_BN254.h>
#include <ecdaa/prng.h>

#include "../src/amcl-extensions/big_256_56.h"
#include "../src/amcl-extensions/ecp_BN254.h"
#include "../src/amcl-extensions/ecp2_BN254.h"

#include <amcl/ecp_BN254.h>
#include <amcl/ecp2_BN254.h>

#include <string.h>

typedef struct credential_test_fixture {
    struct ecdaa_prng prng;

    struct ecdaa_member_public_key_BN254 pk;
    struct ecdaa_member_secret_key_BN254 sk;

    struct ecdaa_issuer_public_key_BN254 ipk;
    struct ecdaa_issuer_secret_key_BN254 isk;
} credential_test_fixture;

static void setup(credential_test_fixture* fixture);
static void teardown(credential_test_fixture* fixture);

static void cred_generate_then_validate();

int main()
{
    cred_generate_then_validate();
}

static void setup(credential_test_fixture* fixture)
{
    TEST_ASSERT(0 == ecdaa_prng_init(&fixture->prng));

    big_256_56_random_mod_order(&fixture->isk.x, get_csprng(&fixture->prng));
    ecp2_BN254_set_to_generator(&fixture->ipk.gpk.X);
    ECP2_BN254_mul(&fixture->ipk.gpk.X, fixture->isk.x);

    big_256_56_random_mod_order(&fixture->isk.y, get_csprng(&fixture->prng));
    ecp2_BN254_set_to_generator(&fixture->ipk.gpk.Y);
    ECP2_BN254_mul(&fixture->ipk.gpk.Y, fixture->isk.y);

    ecp_BN254_set_to_generator(&fixture->pk.Q);
    big_256_56_random_mod_order(&fixture->sk.sk, get_csprng(&fixture->prng));
    ECP_BN254_mul(&fixture->pk.Q, fixture->sk.sk);
}

static void teardown(credential_test_fixture* fixture)
{
    ecdaa_prng_free(&fixture->prng);
}

static void cred_generate_then_validate()
{
    printf("Starting join-tests::cred_generate_validate...\n");

    credential_test_fixture fixture;
    setup(&fixture);

    struct ecdaa_credential_BN254 cred;
    struct ecdaa_credential_BN254_signature cred_sig;
    TEST_ASSERT(0 == ecdaa_credential_BN254_generate(&cred, &cred_sig, &fixture.isk, &fixture.pk, &fixture.prng));

    TEST_ASSERT(0 == ecdaa_credential_BN254_validate(&cred, &cred_sig, &fixture.pk, &fixture.ipk.gpk));

    teardown(&fixture);

    printf("\tsuccess\n");
}
