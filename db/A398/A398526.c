/*
 * A398526  a(n) = Sum_{k=1..n} A212793(floor(n/k)).
 * https://oeis.org/A398526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} [floor(n/k) is cubefree]
 * Range:     u8 n <= 260, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A(n) counts the number of k in [1,n] such that ⌊n/k⌋ has no
   prime factor with exponent ≥ 3. */
#include <stdbool.h>

static inline bool is_cubefree(idx_t x)
{
    idx_t t = x;
    for (idx_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            idx_t exp = 0;
            while (t % p == 0)
            {
                t /= p;
                ++exp;
                if (exp >= 3)
                    return false;    /* cube factor found */
            }
        }
    }
    /* Remaining factor (if any) has exponent ≤ 2, so no cube factor */
    return true;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t m = n / k;
        if (is_cubefree(m))
            ++sum;
    }
    return sum;
}
