/*
 * A270030  a(n) is the smallest b for which the base-b representation of n
 *          contains at least one 4 (or 0 if no such base exists).
 * https://oeis.org/A270030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 538, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Return the smallest base b ≥ 5 such that the base‑b representation
   of n contains a digit 4; return 0 when no such base exists (this happens
   only for n = 1,2,3,5,6,7,8).  For n ≥ 9 a base exists (at least b = n‑4). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for n = 4 */
    if (n == 4)
        return (seq_t)5;

    /* For n < 9 there is no base that yields a digit 4 */
    if (n < 9)
        return (seq_t)0;

    seq_t nseq = (seq_t)n;
    /* Search bases from 5 up to n-4 (inclusive). */
    for (idx_t b = 5; b <= n - 4U; ++b)
    {
        seq_t temp = nseq;
        while (temp > 0)
        {
            if (temp % (seq_t)b == (seq_t)4)
                return (seq_t)b;
            temp /= (seq_t)b;
        }
    }

    /* Should never reach here for n ≥ 9, but return 0 for safety */
    return (seq_t)0;
}
