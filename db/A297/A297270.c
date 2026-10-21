/*
 * A297270  Numbers whose base-10 digits have greater down-variation than
 *          up-variation; see Comments.
 * https://oeis.org/A297270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth integer whose first digit is greater than its last
 *            digit
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = 1;                      /* current candidate number */
    idx_t  count = 0;                 /* how many valid numbers seen so far */

    for (;;)
    {
        seq_t tmp   = x;
        seq_t last  = tmp % 10;       /* last digit */

        /* obtain first digit */
        while (tmp >= 10)
            tmp /= 10;
        seq_t first = tmp;

        if (first > last)             /* satisfy the sequence condition */
        {
            ++count;
            if (count == n)
                return x;
        }
        ++x;                          /* next candidate */
    }
}
