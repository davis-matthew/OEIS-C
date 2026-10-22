/*
 * A384451  Consecutive states of the linear congruential pseudo-random number
 *          generator randq1 from Numerical Recipes when started at 1.
 * https://oeis.org/A384451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1664525 * a(n-1) + 1013904223) mod 2^32, with a(1)=1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Off‑by‑one handling – the official offset is 1, but guard
       for possible out‑of‑range calls. */
    if (n <= 1)
        return (seq_t)1;

    seq_t val = 1;
    const uint64_t mult = 1664525ULL;
    const uint64_t add  = 1013904223ULL;

    /* Iterate until the desired index is reached.  The intermediate
       product is done in 64‑bit arithmetic to avoid overflow; the
       result is then reduced to 32 bits. */
    for (idx_t i = 2; i <= n; ++i) {
        uint64_t temp = ((uint64_t)val * mult) + add;
        val = (seq_t)(temp & 0xFFFFFFFFULL);   /* mod 2^32 */
    }

    return val;
}
