/*
 * A346296  a(0) = 1; thereafter a(n) = 2*a(n-1) + 1, with digits rearranged
 *          into nondecreasing order.
 * https://oeis.org/A346296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 1; a(n) = A004185(2*a(n-1)+1) for n > 0 (A004185 sorts
 *            the decimal digits of its argument in nondecreasing order)
 * Range:     u8 n <= 8, u16 n <= 21, u32 n <= 52, u64 n <= 107, u128 n <= 224,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)1;          /* a(0) */

    /* For n = 0 the loop is skipped; the value 1 is returned. */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Compute next = 2*cur + 1 */
        seq_t next = cur * (seq_t)2 + (seq_t)1;

        /* Sort the digits of next in nondecreasing order. */
        unsigned char cnt[10] = {0};
        digit_counts(next, cnt);

        seq_t sorted = (seq_t)0;
        for (int d = 0; d < 10; ++d)
        {
            while (cnt[d] > 0)
            {
                sorted = sorted * (seq_t)10 + (seq_t)d;
                --cnt[d];
            }
        }
        cur = sorted;               /* a(i) */
    }

    return cur;                      /* a(n) */
}
