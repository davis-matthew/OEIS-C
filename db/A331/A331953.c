/*
 * A331953  a(n) is the least positive k such that floor(n/k) is a square
 *          number.
 * https://oeis.org/A331953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest positive integer k such that floor(n/k) is a
 *            perfect square. The first two terms are a(0)=1, a(1)=1,
 *            following the OEIS offset 0
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    /* n == 0 is a special case because the loop would never run. */
    if (n == 0)
        return 1;                 /* floor(0/1) = 0 = 0^2 */

    /* Search k from 1 upwards. The first k that yields a square
       floor(n/k) is the minimal one. */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t t = (seq_t)n / (seq_t)k;   /* floor(n/k) */
        seq_t m = floor_sqrt(t);         /* integer sqrt of t   */
        if (m * m == t)                  /* t is a perfect square */
            return (seq_t)k;             /* minimal k found */
    }

    /* If no k in 1..n works, k = n+1 gives floor(n/(n+1)) = 0 = 0^2. */
    return (seq_t)n + 1;
}
