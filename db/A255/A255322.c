/*
 * A255322  a(n) = Product_{k=0..n} (k^2)!.
 * https://oeis.org/A255322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (k^2)!
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t product = 1;   /* running product of all factorials */
    seq_t fact    = 1;   /* current factorial value */
    idx_t last    = 0;   /* last value of k used to compute fact */

    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t m = k * k;              /* target factorial argument */
        /* extend current factorial from (last)! up to (m)! */
        for (idx_t i = last + 1; i <= m; ++i)
            fact *= (seq_t)i;

        /* multiply into the product */
        product *= fact;

        last = m;
    }

    return product;
}
