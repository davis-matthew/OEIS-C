/*
 * A294927  Number of proper divisors of n that are nondeficient (A023196).
 * https://oeis.org/A294927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of proper divisors d of n that are nondeficient
 *            (σ(d) ≥ 2·d)
 * Range:     u8 n <= 8543, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9560 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* sum of all divisors of m (including m itself) */
static seq_t sum_of_divisors(idx_t m)
{
    seq_t sum = 0;
    /* iterate only up to sqrt(m) using integer arithmetic */
    for (idx_t i = 1; i * i <= m; ++i)
    {
        if (m % i == 0)
        {
            sum += (seq_t)i;
            idx_t j = m / i;
            if (j != i)
                sum += (seq_t)j;
        }
    }
    return sum;
}

/* true iff m is nondeficient (σ(m) ≥ 2·m) */
static bool is_nondeficient(idx_t m)
{
    seq_t sigma = sum_of_divisors(m);
    /* avoid underflow by comparing directly */
    return sigma >= ((seq_t)2 * (seq_t)m);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;          /* a(1) = 0, a(0) undefined */

    seq_t cnt = 0;
    idx_t limit = n;

    for (idx_t i = 1; i * i <= limit; ++i)
    {
        if (limit % i == 0)
        {
            /* first divisor */
            if (i < limit && is_nondeficient(i))
                ++cnt;

            idx_t j = limit / i;
            if (j != i && j < limit && is_nondeficient(j))
                ++cnt;
        }
    }

    return cnt;
}
