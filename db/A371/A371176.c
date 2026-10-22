/*
 * A371176  Numbers k such that A000120(k) <= A001511(k).
 * https://oeis.org/A371176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer k such that popcount(k) <=
 *            trailing_zeros(k) + 1 where trailing_zeros(k) is the number of
 *            zero bits before the first 1 in the binary representation of k
 *            (starting from the LSB)
 * Range:     u8 n <= 54, u16 n <= 2583, u32 n <= 8480, u64 n <= 8480,
 *            u128 n <= 8480, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t k = 1;          /* candidate number */
    idx_t found = 0;      /* how many numbers satisfying the condition we have seen */

    while (true)
    {
        /* compute number of trailing zeros of k */
        idx_t tz = 0;
        seq_t tmp = k;
        while ((tmp & (seq_t)1) == 0)
        {
            tz++;
            tmp >>= 1;
        }

        /* compute popcount(k) */
        seq_t pop = 0;
        tmp = k;
        while (tmp != 0)
        {
            pop += (tmp & (seq_t)1);
            tmp >>= 1;
        }

        /* check the condition          */
        if (pop <= (seq_t)(tz + 1))
        {
            found++;
            if (found == n)
                return k;
        }

        k += 1;           /* next candidate */
    }
}
