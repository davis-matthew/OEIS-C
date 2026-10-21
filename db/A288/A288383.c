/*
 * A288383  Positions of 1 in A288381; complement of A288382.
 * https://oeis.org/A288383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 3 + ⌊log₂(n)⌋ (for n ≥ 1)
 * Range:     u8 n <= 245, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(log2(x)) for x ≥ 1. */
static inline idx_t floor_log2(idx_t x)
{
    idx_t r = 0;
    while (x > 1)
    {
        x >>= 1;
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* sequence is defined for n ≥ 1 (offset 1) */
    idx_t ln = floor_log2(n);          /* log2(n) */
    seq_t result = (seq_t)(n + 3 + ln);/* n + 3 + floor(log2(n)) */
    return result;
}
