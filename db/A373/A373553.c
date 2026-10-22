/*
 * A373553  For any number m, let m* be the bi-infinite string obtained by
 *          repetition of the binary expansion of m; a(n) is the largest
 *          positive integer k such that the binary expansions of all positive
 *          integers <= k are found within n*.
 * https://oeis.org/A373553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the largest k such that the binary expansions of all *
 *            positive integers up to k appear as contiguous substrings of the
 *            * infinite periodic binary string obtained by repeating the
 *            binary * expansion of n
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* bit length of an unsigned 128-bit type is handled by the loop below */
static inline unsigned int bitlen_idx(idx_t v)
{
    unsigned int len = 0;
    while (v > 0) {
        ++len;
        v >>= 1;
    }
    return len;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* offset starts at 1, but guard for safety */
    }

    /* Length of the binary representation of n */
    const unsigned int L = bitlen_idx(n);

    /* Store bits of n starting with the most significant bit */
    uint8_t bits[64];
    for (unsigned int i = 0; i < L; ++i) {
        bits[i] = (uint8_t)((n >> (L - 1 - i)) & 1U);
    }

    /* Incrementally test k = 1,2,... until a missing binary
       representation is found */
    for (idx_t k = 1; ; ++k) {
        const unsigned int l = bitlen_idx(k);
        const seq_t target = (seq_t)k;
        bool found = false;

        /* Try every possible starting position modulo the period */
        for (unsigned int start = 0; start < L; ++start) {
            seq_t w = 0;
            for (unsigned int j = 0; j < l; ++j) {
                w = (w << 1) | (seq_t)bits[(start + j) % L];
            }
            if (w == target) {
                found = true;
                break;
            }
        }

        if (!found) {
            return (seq_t)(k - 1);
        }
    }
}
