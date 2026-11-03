/*
 * A270528  Sum of divisors of the products of the smaller and larger parts of
 *          the partitions of n into two parts.
 * https://oeis.org/A270528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..⌊n/2⌋} σ(i·(n-i))
 * Range:     u8 n <= 11, u16 n <= 70, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 1000 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t limit = n / 2;
    for (idx_t i = 1; i <= limit; ++i)
    {
        idx_t prod = i * (n - i);
        seq_t divsum = sigma_of(prod);
        total += divsum;
    }
    return total;
}
