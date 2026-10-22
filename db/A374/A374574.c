/*
 * A374574  a(n) = Sum_{j=n..2n} j!.
 * https://oeis.org/A374574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=n..2n} j! (a(0)=1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return ((seq_t)1);

    seq_t sum = 0;
    seq_t fact = 1;
    idx_t limit = 2 * n;

    for (idx_t j = 1; j <= limit; ++j)
    {
        fact = fact * (seq_t)j;
        if (j >= n)
            sum += fact;
    }

    return sum;
}
