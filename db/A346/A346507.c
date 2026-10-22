/*
 * A346507  Positive integers k that are the product of two integers greater
 *          than 1 and ending with 1.
 * https://oeis.org/A346507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1797, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th positive integer that is the
 * product of two integers greater than 1, each of which ends with the
 * digit 1.  Numbers are enumerated in increasing order.  For example
 * a(1) = 121 = 11*11, a(2) = 231 = 11*21, etc. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1 */

    seq_t candidate = 11;      /* first integer > 1 ending with 1 */
    idx_t   count    = 0;

    while (1)
    {
        /* Test all divisors d > 1 ending with 1 up to sqrt(candidate) */
        for (seq_t d = 11; d <= candidate / d; d += 10)
        {
            if (candidate % d == 0)
            {
                seq_t e = candidate / d;
                if (e % 10 == 1)  /* e also ends with 1 */
                {
                    ++count;
                    if (count == n) return candidate;
                    break;           /* move to next candidate */
                }
            }
        }
        candidate += 10;  /* next number ending with 1 */
    }
}
