/*
 * A384373  Consecutive internal states of the linear congruential
 *          pseudo-random number generator 131075*s mod 2^27 when started at
 *          s=1.
 * https://oeis.org/A384373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 131075 * a(n-1) mod 2^27, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    /* a(1) = 1 */
    if (n <= 1) return (seq_t)1;

    /* Modulus 2^27; keep it in a type that fits seq_t. */
    const seq_t MOD_MASK = ((seq_t)(((unsigned long long)1 << 27) - 1));

    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)131075;
    idx_t exp    = n - 1;

    while (exp) {
        if (exp & 1) {
            unsigned int b = (unsigned int)base;
            result = mul_small(result, b) & MOD_MASK;
        }
        exp >>= 1;
        if (exp) {
            unsigned int b = (unsigned int)base;
            base = mul_small(base, b) & MOD_MASK;
        }
    }
    return result;
}
