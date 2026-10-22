/*
 * A332624  a(n) = Sum_{k=1..n} ceiling(n/k)^n.
 * https://oeis.org/A332624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} ceil(n/k)^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n=0, return 0 to be safe */
        return 0;

    seq_t sum = 0;
    idx_t inc = 1;

    while (inc <= n) {
        /* t = ceil(n / inc) */
        idx_t t = (n + inc - 1) / inc;

        /* r = last k that yields the same t */
        idx_t r;
        if (t == 1)
            r = n;
        else
            r = (n - 1) / (t - 1);

        if (r > n)
            r = n;

        /* number of k's with this t */
        idx_t cnt = r - inc + 1;

        /* t^n */
        seq_t p = seq_pow((seq_t)t, n);

        /* accumulate */
        sum += p * (seq_t)cnt;

        inc = r + 1;
    }

    return sum;
}
