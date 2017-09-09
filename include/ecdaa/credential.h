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

#ifndef XAPTUM_ECDAA_CREDENTIAL_H
#define XAPTUM_ECDAA_CREDENTIAL_H
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct ecdaa_member_public_key_t;
struct ecdaa_issuer_secret_key_t;
struct ecdaa_group_public_key_t;

#include <amcl/ecp_BN254.h>
#include <amcl/big_256_56.h>
#include <amcl/randapi.h>

#include <stdint.h>

/*
 * Credential (provided to Member by Issuer, after successful Join).
 */
typedef struct ecdaa_credential_t {
    ECP_BN254 A;
    ECP_BN254 B;
    ECP_BN254 C;
    ECP_BN254 D;
} ecdaa_credential_t;

#define SERIALIZED_CREDENTIAL_LENGTH (4*(2*MODBYTES_256_56 + 1))
size_t serialized_credential_length(void);

/*
 * Signature over `ecdaa_credential_t` provided by an Issuer.
 */
typedef struct ecdaa_credential_signature_t {
    BIG_256_56 c;
    BIG_256_56 s;
} ecdaa_credential_signature_t;

#define SERIALIZED_CREDENTIAL_SIGNATURE_LENGTH (2*MODBYTES_256_56)
size_t serialized_credential_signature_length(void);

/*
 * Generate a new `ecdaa_credential_t`.
 *
 * Used by an Issuer, at the end of a successful Join process.
 */
int ecdaa_generate_credential(ecdaa_credential_t *cred,
                              ecdaa_credential_signature_t *cred_sig_out,
                              struct ecdaa_issuer_secret_key_t *isk,
                              struct ecdaa_member_public_key_t *member_pk,
                              csprng *rng);

/*
 * Validate a credential and its signature.
 *
 * Returns:
 * 0 on success
 * -1 if Join response is invalid
 */
int ecdaa_validate_credential(struct ecdaa_credential_t *credential,
                              struct ecdaa_credential_signature_t *credential_signature,
                              struct ecdaa_member_public_key_t *member_pk,
                              struct ecdaa_group_public_key_t *gpk);

/*
 * Serialize an `ecdaa_credential_t`
 *
 * Serialized format is;
 *  ( 0x04 | A.x-coord | A.y-coord |
 *      0x04 | B.x-coord | B.y-coord |
 *      0x04 | C.x-coord | C.y-coord |
 *      0x04 | D.x-coord | D.y-coord )
 *  where all numbers are zero-padded big-endian.
 *
 * The provided buffer is assumed to be large enough.
 */
void ecdaa_serialize_credential(uint8_t *buffer_out,
                                ecdaa_credential_t *credential);

/*
 * Serialize an `ecdaa_credential_signature_t`
 */
void ecdaa_serialize_credential_signature(uint8_t *buffer_out,
                                          ecdaa_credential_signature_t *cred_sig);

/*
 * De-serialize an `ecdaa_credential_t` and check its validity (signature check not performed).
 *
 * Expected serialized format is;
 *  ( 0x04 | A.x-coord | A.y-coord |
 *      0x04 | B.x-coord | B.y-coord |
 *      0x04 | C.x-coord | C.y-coord |
 *      0x04 | D.x-coord | D.y-coord )
 *  where all numbers are zero-padded big-endian.
 *
 *  Returns:
 *  0 on success
 *  -1 if credential is mal-formed
 *  -2 if signature is invalid
 */
int ecdaa_deserialize_credential_with_signature(ecdaa_credential_t *credential_out,
                                                struct ecdaa_member_public_key_t *member_pk,
                                                struct ecdaa_group_public_key_t *gpk,
                                                uint8_t *buffer_in);

/*
 * De-serialize an `ecdaa_credential_t` and `ecdaa_signature_t`, check its validity _and_ signature
 *
 * Expected serialized format is;
 *  ( 0x04 | A.x-coord | A.y-coord |
 *      0x04 | B.x-coord | B.y-coord |
 *      0x04 | C.x-coord | C.y-coord |
 *      0x04 | D.x-coord | D.y-coord |
 *      c |
 *      s )
 *  where all numbers are zero-padded big-endian.
 */
int ecdaa_deserialize_credential(ecdaa_credential_t *credential_out,
                                 uint8_t *buffer_in);

#ifdef __cplusplus
}
#endif

#endif
