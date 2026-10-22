/*
 * A350268  a(n) = Sum_{k=0..n} (n - k)! * (n - 1)^k. Row sums of A350269.
 * https://oeis.org/A350268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (n - k)! * (n - 1)^k
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= i;                 /* factorial(n) */

    seq_t term = fact;
    seq_t sum  = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        sum += term;
        if (k < n)
            term = (term * (seq_t)(n - 1)) / (seq_t)(n - k);
    }
    return sum;
}
