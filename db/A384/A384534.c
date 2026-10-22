/*
 * A384534  Consecutive states of the linear congruential pseudo-random number
 *          generator 1099087573 * s mod 2^32 when started at s=1.
 * https://oeis.org/A384534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(n)=1099087573*a(n-1) mod 2^32 for n>1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* use 64‑bit intermediate to avoid overflow of 32‑bit multiplication */
    uint64_t val = 1;
    const uint64_t mult = 1099087573ULL;
    const uint64_t mask = 0xFFFFFFFFULL;

    for (idx_t i = 2; i <= n; ++i) {
        /* keep only the low 32 bits of the previous value before multiplying */
        val = (mult * (val & mask)) & mask;
    }

    return (seq_t)val;
}
