/*
 * A317538  Indices m for which A317413(m) = 1.
 * https://oeis.org/A317538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(n-1) + 1 - A073089(n)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A073089_a(idx_t n)
{
    if (n == 1U)
        return (seq_t)0;
    for (;;) {
        if ((n & 3ULL) == 0) return (seq_t)1;
        if ((n & 3ULL) == 2) return (seq_t)0;
        unsigned mod8 = (unsigned)(n & 7ULL);
        if (mod8 == 3) return (seq_t)1;
        if (mod8 == 7) return (seq_t)0;
        unsigned mod16 = (unsigned)(n & 15ULL);
        if (mod16 == 5) return (seq_t)1;
        if (mod16 == 13) return (seq_t)0;
        n = ((n - 1ULL) >> 1) + 1ULL;
    }
}

seq_t a(idx_t n)
{
    return (seq_t)3 * ((seq_t)n - 1) + 1 - A073089_a(n);
}
