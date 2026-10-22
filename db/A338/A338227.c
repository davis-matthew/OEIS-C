/*
 * A338227  a(n) = x(n) mod floor(sqrt(x(n))), where x(n) = floor((n^2)/2).
 * https://oeis.org/A338227
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^2/2) mod floor(sqrt(floor(n^2/2)))
 * Range:     u8 n <= 11, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n ≥ 2 for the sequence, but handle smaller values safely */
    if (n < 2) return 0;

    /* x(n) = floor(n^2/2) */
    seq_t x = (seq_t)n * (seq_t)n / (seq_t)2;

    /* d(n) = floor(sqrt(x(n))) */
    seq_t d = floor_sqrt(x);

    /* For n ≥ 2, d ≥ 1, so modulo is well‑defined */
    return x % d;
}
