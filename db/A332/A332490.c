/*
 * A332490  a(n) = Sum_{k=1..n} k * ceiling(n/k).
 * https://oeis.org/A332490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 + Sum_{k=1..n-1} sigma(k)
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma(k) is the sum of all positive divisors of k. */

#include <stddef.h>   /* for size_t, needed for some compilers */

seq_t a(idx_t n)
{
    /* first part: n*(n+1)/2 */
    seq_t result = (seq_t)n * ((seq_t)n + 1) / 2;

    if (n <= 1) return result;            /* a(1) = 1 */

    /* accumulate sum_{k=1}^{n-1} sigma(k) by a divisor–sieve */
    seq_t sum_sigma = 0;
    for (idx_t d = 1; d < n; ++d)      /* divisor d */
    {
        /* every multiple of d less than n receives d as a divisor */
        for (idx_t m = d; m < n; m += d)
        {
            sum_sigma += (seq_t)d;
        }
    }

    return result + sum_sigma;
}
