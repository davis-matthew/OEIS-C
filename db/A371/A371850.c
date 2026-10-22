/*
 * A371850  Decimal expansion of 2^216091 - 1, the 31st Mersenne prime.
 * https://oeis.org/A371850
 *
 * Offset:    65050    (a(65050) is the first term)
 * Keywords:  nonn, cons, fini
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   pow2, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal expansion of 2^216091 − 1, offset 65050 */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/seq_to_str.h"

seq_t a(idx_t n)
{
    /* offset of first term */
    static const idx_t OFFSET = 65050;
    /* number of decimal digits of the prime */
    static const idx_t DIGITS = 65050;
    /* cached decimal representation */
    static char *prime_str = NULL;
    static size_t prime_len = 0;

    if (prime_str == NULL) {
        /* compute the prime 2^216091 − 1 */
        seq_t big = pow2((idx_t)216091);
        big = big - 1;          /* seq_t supports the minus operator             */
        /* allocate buffer large enough for the digits (+1 for safety) */
        prime_str = (char *)malloc(DIGITS + 2);
        if (!prime_str) {
            return 0; /* out of memory: return any value – behaviour below OFFSET does not matter */
        }
        /* convert to decimal string */
        seq_to_str(big, prime_str, &prime_len);
        /* ensure we have the expected length */
        if (prime_len != DIGITS) {
            /* fallback: 0 if size mismatch */
            return 0;
        }
    }

    if (n < OFFSET) {
        return 0; /* behaviour below offset does not matter */
    }
    idx_t idx = n - OFFSET;
    if (idx >= prime_len) {
        return 0; /* past the end: not required but safe */
    }
    return (seq_t)(prime_str[idx] - '0');
}
