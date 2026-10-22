/*
 * A331958  a(n)^2 is the greatest square number of the form floor(n/k) where
 *          k > 0.
 * https://oeis.org/A331958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute the sequence A331958: a(n)^2 = max square of floor(n/k), k>0
   a(n) = sqrt( max_{k≥1} floor(n/k) that is a square )               */
/* uses the utility function floor_sqrt() from ../utils/floor_sqrt.h       */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n = 0 is the first term, trivially 0 */
    if (n == 0) return (seq_t)0;

    /* iterate over k from 1 up to n.  The first time we hit a perfect square,
       it is the largest, because floor(n/k) decreases with k.             */
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t v_int = n / k;          /* floor(n/k) fits in idx_t */
        seq_t v     = (seq_t)v_int;   /* promote for sqrt   */
        seq_t s     = floor_sqrt(v);  /* integer sqrt of v   */
        if (s * s == v)               /* v is a perfect square? */
            return s;                 /* return a(n) = sqrt(v) */
    }

    /* unreachable for n>0: floor(n/n)=1 is always a square          */
    return (seq_t)0; /* fallback, should not occur */
}
