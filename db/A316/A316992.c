/*
 * A316992  Numbers m such that 1 < gcd(m, 15) < m and m does not divide 15^e
 *          for e >= 0.
 * https://oeis.org/A316992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th integer m>1 that is divisible by 3 or 5, but is
 *            not a divisor of any power of 15 (i.e., its prime factors
 *            contain at least one of 3 or 5 and at least one other prime).
 *            The function searches successively and counts valid candidates
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* offset = 1, so n must be >=1, but we guard against n==0 */
    if (n == 0)
        return 0;

    idx_t count = 0;

    /* start searching from 2 upward; 1 is excluded by the definition */
    for (seq_t m = 2; ; ++m)
    {
        /* must share a prime factor 3 or 5 (i.e., gcd(m,15)>1) */
        if (!(m % 3 == 0 || m % 5 == 0))
            continue;

        /* remove all factors 3 and 5 */
        seq_t t = m;
        while (t % 3 == 0)
            t /= 3;
        while (t % 5 == 0)
            t /= 5;

        /* if the remainder is 1, m has no other prime factor and
           therefore divides a power of 15; skip it */
        if (t == 1)
            continue;

        /* valid candidate */
        ++count;
        if (count == n)
            return m;
    }
}
