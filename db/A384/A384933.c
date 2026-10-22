/*
 * A384933  Consecutive states of the linear congruential pseudo-random number
 *          generator (1589013525*s + 1) mod 2^32 when started at s=1.
 * https://oeis.org/A384933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; * a(n) = (1589013525 * a(n-1) + 1) mod 2^32 for n ≥ 2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iterate the linear congruential formula.
 * Uses a 64‑bit unsigned intermediate to avoid overflow, and
 * masks the result to 32 bits. The return value is cast to seq_t,
 * which can be any unsigned integer type or arbitrary‑precision
 * integer class supported by the build. */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n) {
    /* handle the trivial case */
    if (n == 0) {
        /* undefined for n < 1; return 0 as a benign value */
        return (seq_t)0;
    }

    /* constants of the LCG */
    const uint64_t A = 1589013525UL;
    const uint64_t C = 1UL;
    const uint64_t MASK32 = 0xFFFFFFFFULL;

    /* start value a(1) = 1 */
    uint64_t cur = 1ULL;

    /* iterate up to the desired index */
    for (idx_t i = 1; i < n; ++i) {
        cur = (A * cur + C) & MASK32;
    }

    return (seq_t)cur;
}
