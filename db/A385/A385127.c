/*
 * A385127  Consecutive internal states of the linear congruential
 *          pseudo-random number generator for gcc 2.6.3 when started at 1.
 * https://oeis.org/A385127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(n) = (69069 * a(n-1) + 5) mod 2^32 (n ≥ 2)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) {
        return (seq_t)0; /* Undefined for n < 1; return 0 to avoid UB */
    }
    /* Use 64‑bit arithmetic to avoid overflow, then mask to 32 bits. */
    const unsigned long long MASK32 = 0xFFFFFFFFULL;
    unsigned long long val = 1ULL;          /* a(1) */
    if (n == 1) {
        return (seq_t)val;
    }
    for (idx_t i = 2; i <= n; ++i) {
        val = (69069ULL * val + 5ULL) & MASK32;
    }
    return (seq_t)val;
}
