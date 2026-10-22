/*
 * A353626  a(n) = 1 if n is a multiple of the square of an odd prime
 *          (equally: if the odd part of n is not squarefree), otherwise 0.
 * https://oeis.org/A353626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the odd part of n is not squarefree, otherwise 0
 * Range:     u8 n <= 9687, u16 n <= 9687, u32 n <= 9687, u64 n <= 9687,
 *            u128 n <= 9687, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* work on the odd part of n */
    idx_t m = n;
    /* strip all factors of 2 */
    while ((m & (idx_t)1) == 0)
        m >>= 1;

    if (m <= 1)
        return (seq_t)0;

    /* test for a square of an odd prime in m */
    idx_t p = 3;                       /* start with smallest odd prime */
    while (p <= m / p)
    {
        if (m % p == 0)
        {
            int exp = 0;
            while (m % p == 0)
            {
                m /= p;
                exp++;
                if (exp >= 2)         /* found p^2 | original odd part */
                    return (seq_t)1;
            }
        }
        p += 2;                       /* next odd candidate */
    }
    /* If we reach here, no odd prime square divides the odd part */
    return (seq_t)0;
}
