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

#ifndef XAPTUM_ECDAA_MEMBER_KEYPAIR_H
#define XAPTUM_ECDAA_MEMBER_KEYPAIR_H
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <amcl/big_256_56.h>
#include <amcl/ecp_BN254.h>

struct ecdaa_issuer_nonce_t;

/*
 * Member's public key.
 */
typedef struct ecdaa_member_public_key_t {
    ECP_BN254 Q;
    BIG_256_56 c;
    BIG_256_56 s;
} ecdaa_member_public_key_t;

#define SERIALIZED_MEMBER_PUBLIC_KEY_LENGTH ((2*MODBYTES_256_56 + 1) + MODBYTES_256_56 + MODBYTES_256_56)
size_t serialized_member_public_key_length();

/*
 * Member's secret key.
 */
typedef struct ecdaa_member_secret_key_t {
    BIG_256_56 sk;
} ecdaa_member_secret_key_t;

#define SERIALIZED_MEMBER_SECRET_KEY_LENGTH (MODBYTES_256_56)
size_t serialized_member_secret_key_length();

/*
 * Generate a fresh keypair.
 */
int ecdaa_generate_member_key_pair(ecdaa_member_public_key_t *pk,
                                   ecdaa_member_secret_key_t *sk,
                                   struct ecdaa_issuer_nonce_t *nonce,
                                   csprng *rng);

/*
 * Check the signature on an `ecdaa_member_public_key_t`.
 * 
 * Returns:
 * 0 on success
 * -1 if signature is not valid.
 */
int ecdaa_validate_member_public_key(ecdaa_member_public_key_t *pk,
                                     struct ecdaa_issuer_nonce_t *nonce_in);

/*
 * Serialize an `ecdaa_member_public_key_t`
 *
 * The serialized format is:
 *  ( 0x04 | Q.x-coord | Q.y-coord | c | s )
 *  where all numbers are zero-padded and in big-endian byte-order.
 *
 * The provided buffer is assumed to be large enough.
 */
void ecdaa_serialize_member_public_key(uint8_t *buffer_out,
                                       ecdaa_member_public_key_t *pk);

/*
 * De-serialize an `ecdaa_member_public_key_t`, and check its validity and signature.
 *
 * The `nonce_in` should be the `ecdaa_issuer_nonce_t`
 *  provided by the Issuer when the Member generated this public key.
 *
 * The serialized format is expected to be:
 *  ( 0x04 | Q.x-coord | Q.y-coord | c | s )
 *  where all numbers are zero-padded and in big-endian byte-order.
 *
 * Returns:
 * 0 on success
 * -1 if the format is incorrect
 * -2 if  (c,s) don't verify
 */
int ecdaa_deserialize_member_public_key(ecdaa_member_public_key_t *pk_out,
                                        uint8_t *buffer_in,
                                        struct ecdaa_issuer_nonce_t *nonce_in);

/*
 * Serialize an `ecdaa_member_secret_key_t`
 *
 * The serialized secret key is zero-padded in big-endian byte-order.
 *
 * The provided buffer is assumed to be large enough.
 */
void ecdaa_serialize_member_secret_key(uint8_t *buffer_out,
                                       ecdaa_member_secret_key_t *sk);

/*
 * De-serialize an `ecdaa_member_secret_key_t`
 *
 * The serialized secret key is expected to be zero-padded in big-endian byte-order.
 *
 * Returns:
 * 0 on success
 */
int ecdaa_deserialize_member_secret_key(ecdaa_member_secret_key_t *sk_out,
                                        uint8_t *buffer_in);

#ifdef __cplusplus
}
#endif

#endif
