/*
 * A373666  Smallest positive integer whose square can be written as the sum
 *          of n positive perfect squares whose square roots differ by no more
 *          than 1.
 * https://oeis.org/A373666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest d such that d² can be written as the sum * of n
 *            positive perfect squares whose square roots differ by at most 1.
 *            * The code searches increasing d, checking the necessary
 *            congruence * and integer‑ness conditions derived from the
 *            equivalent * representation d² = n·m² + t(2m+1) with...
 * Range:     u8 n <= 16, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n=0, safeguard */

    seq_t n_seq = (seq_t)n;

    /* start at ceil(sqrt(n)) */
    seq_t r = floor_sqrt(n_seq);
    seq_t d_start = r;
    if (r * r < n_seq) d_start = r + 1;   /* ceil(sqrt(n)) */
    if (d_start < 1) d_start = 1;         /* safety */

    for (seq_t d = d_start; ; ++d)
    {
        seq_t d2 = d * d;                 /* d² */

        /* m = floor(sqrt(d² / n))  (the smaller of the two square root values) */
        seq_t div = d2 / n_seq;
        seq_t m   = floor_sqrt(div);
        if (m == 0) continue;            /* should not happen for d ≥ ceil(sqrt(n)) */

        seq_t denom    = (2 * m) + 1;     /* 2m+1  */
        seq_t mm       = m * m;           /* m²    */
        seq_t numerator = d2 - n_seq * mm;/* d² – n·m² */

        /* numerator must be divisible by denom, and the quotient t must lie in [0,n] */
        if (numerator % denom != 0) continue;
        seq_t t = numerator / denom;
        if (t <= n_seq) return d;
    }

    /* unreachable */
    return 0;
}
