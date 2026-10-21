/*
 * A274340  The period 4 sequence of the iterated sum of deficient divisors
 *          function (A187793) starting at 19.
 * https://oeis.org/A274340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A187793(a(n-1)), a(1) = 19, a(n+4)=a(n)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sum_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A187793 is the sum of deficient divisors of its argument.   */
/* The implementation uses the definition directly:  iterate from   */
/* a(1)=19 and apply sigma_d at each step.  This works for all n≥1. */

#include <stdint.h> /* for standard integer types */
#include "../utils/sum_divisors.h"

static int is_deficient(seq_t n)
{
    /* n is deficient iff σ(n) < 2n */
    seq_t sigma = sum_divisors(n);
    return sigma < 2 * n;
}

static seq_t sigma_d(seq_t n)
{
    /* sum of deficient divisors of n */
    seq_t sum = 0;
    for (seq_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            seq_t d1 = i;
            seq_t d2 = n / i;
            if (is_deficient(d1))
                sum += d1;
            if (d2 != d1 && is_deficient(d2))
                sum += d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined offset, but return 0 */
        return 0;

    seq_t val = 19;          /* a(1) */
    for (idx_t i = 1; i < n; ++i)
        val = sigma_d(val); /* a(n) = A187793(a(n-1)) */

    return val;
}
