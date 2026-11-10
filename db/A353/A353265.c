/*
 * A353265  Partial sums of A208981.
 * https://oeis.org/A353265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A208981(k), where A208981(k) is the number
 *            of 3x+1 steps from k to reach a power of 2
 * Range:     u8 n <= 26, u16 n <= 1159, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9784 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A208981_a(idx_t n)
{
    unsigned long long x = (unsigned long long)n;
    seq_t count = 0;
    while ((x & (x - 1)) != 0ULL)
    {
        if ((x & 1ULL) == 0ULL)
            x >>= 1;
        else
            x = 3ULL * x + 1ULL;
        ++count;
    }
    return count;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t k;
    for (k = 1; k <= n; k++)
        sum += A208981_a(k);
    return sum;
}
