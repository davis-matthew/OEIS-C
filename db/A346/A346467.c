/*
 * A346467  a(n) is the least common multiple of the divisors d of n-1 such
 *          that d+1 is prime; a(1) = 1.
 * https://oeis.org/A346467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM{ d | n‑1 : d+1 is prime }, a(1)=1
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_small.h"

/* compute lcm of two seq_t values */
static seq_t lcm_seq(seq_t a_, seq_t b_)
{
    if (a_ == 0 || b_ == 0) return (seq_t)0;
    return (a_ / gcd(a_, b_)) * b_;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    idx_t m = n - 1;
    seq_t res = (seq_t)1;

    for (idx_t d = 1; d <= m; ++d)
    {
        if (m % d == 0 && is_prime_small((seq_t)(d + 1)))
        {
            res = lcm_seq(res, (seq_t)d);
        }
    }
    return res;
}
