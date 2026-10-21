/*
 * A276858  First differences of the Beatty sequence A022842 for sqrt(8).
 * https://oeis.org/A276858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(8)) - floor((n-1)*sqrt(8))
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset 1; return 0 for n=0 as a fallback */

    seq_t nn   = (seq_t)n;
    seq_t floor_n = floor_sqrt((seq_t)8 * nn * nn);

    seq_t floor_n0;
    if (n == 1) {
        floor_n0 = 0;
    } else {
        seq_t nn0 = (seq_t)(n - 1U);
        floor_n0 = floor_sqrt((seq_t)8 * nn0 * nn0);
    }

    return floor_n - floor_n0;
}
