/*
 * A253158  Number of (n+1) X (7+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A253158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*2^(n-1) + 596 for n > 3 Initial values: a(1) = 613,
 *            a(2) = 624, a(3) = 633
 * Range:     u8 none, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0u) return 0u;          /* undefined, return 0 */
    if (n == 1u) return (seq_t)613;  /* a(1) */
    if (n == 2u) return (seq_t)624;  /* a(2) */
    if (n == 3u) return (seq_t)633;  /* a(3) */

    seq_t res = pow2(n - 1);          /* 2^(n-1) */
    res = res * (seq_t)9 + (seq_t)596;/* 9*2^(n-1) + 596 */
    return res;
}
