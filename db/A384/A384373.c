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
 * Validated: standard, pass, 42 terms through n = 8553 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

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
