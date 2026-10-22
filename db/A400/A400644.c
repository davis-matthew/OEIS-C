/*
 * A400644  a(n+1) = round(sqrt(a(n)^2 + a(n-1)^2 + a(n-2)^2)).
 * https://oeis.org/A400644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(1) = 0, a(2) = 0, a(3) = 1, a(n) = round( sqrt( a(n-1)^2 +
 *            a(n-2)^2 + a(n-3)^2 ) ) for n >= 4
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 38, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;

    seq_t a0 = 0;   /* a(n-3) */
    seq_t a1 = 0;   /* a(n-2) */
    seq_t a2 = 1;   /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* sum of squares of three previous terms */
        seq_t sum = a0 * a0 + a1 * a1 + a2 * a2;

        /* integer floor of sqrt(sum) */
        seq_t t       = floor_sqrt(sum);
        seq_t t2      = t * t;
        seq_t t_next  = t + (seq_t)1;
        seq_t t_next2 = t_next * t_next;

        /* round to nearest integer (half up) */
        seq_t next = (sum - t2) < (t_next2 - sum) ? t : t_next;

        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;
}
