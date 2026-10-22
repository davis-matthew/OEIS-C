/*
 * A361000  Number of tilings of an n X 3 rectangle by integer-sided
 *          rectangular pieces that cannot be rearranged to produce a
 *          different tiling of the rectangle (including rotations and
 *          reflections of the original tiling).
 * https://oeis.org/A361000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A000005(n) - A083039(n) for n >= 3 where A000005(n) is
 *            the number of divisors of n, and A083039(n) is: 3 if n divisible
 *            by 6, 2 if n even (but not divisible by 6) or n divisible by 3
 *            only, 1 otherwise. (For n=1,2 the values are a(1)=2, a(2)=3.)
 * Range:     u8 n <= 9694, u16 n <= 9694, u32 n <= 9694, u64 n <= 9694,
 *            u128 n <= 9694, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* handle the offset values */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;

    /* compute number of divisors d(n) */
    seq_t d = 1;
    idx_t rem = n;
    for (idx_t p = 2; p * p <= rem; ++p)
    {
        if (rem % p == 0)
        {
            idx_t exp = 0;
            while (rem % p == 0)
            {
                rem /= p;
                ++exp;
            }
            d *= (seq_t)(exp + 1);
        }
    }
    if (rem > 1) d *= 2;   /* remaining prime factor */

    /* decide the correction value c(n) */
    seq_t c;
    if (n % 6 == 0)        c = 3;
    else if (n % 2 == 0)   c = 2;
    else if (n % 3 == 0)   c = 2;
    else                   c = 1;

    /* return 2*d - c */
    return 2 * d - c;
}
