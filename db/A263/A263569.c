/*
 * A263569  Number of distinct prime divisors p of 2*n such that lpf(2*n - p)
 *          = p, where lpf = least prime factor (A020639).
 * https://oeis.org/A263569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ p prime divisor of 2n : lpf(2n - p) = p }|
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9834 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                offset = 1  (a(1) is the first term) */

#include <stdint.h>
#include <stdbool.h>

/* Return the smallest prime factor of n   (n > 1) */
static inline idx_t lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;   /* n itself is prime – should not be called for primes */
}

seq_t a(idx_t n)
{
    /* 2*n is always even and fits in idx_t for n up to 10^4; use idx_t
       for all intermediate values to avoid overflow.               */
    idx_t m  = n + n;       /* m = 2*n                                     */
    idx_t val;              /* used to store m - p                         */
    idx_t temp  = m;        /* factor temp while extracting primes        */
    seq_t  count = 0;       /* result                                    */

    /* Factor m by trial division, counting each distinct prime divisor
       exactly once. The loop condition p*p <= temp ensures all prime
       divisors up to sqrt(m) are examined.                          */
    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)               /* p is a divisor of m          */
        {
            val = m - p;
            /* ignore zero or negative results (happens only for p=m)   */
            if (val > 0 && lpf(val) == p)
                ++count;

            /* remove all copies of p from temp to keep each prime
               divisor unique                                        */
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* If temp > 1, then temp itself is a prime divisor of m.        */
    if (temp > 1)
    {
        val = m - temp;
        if (val > 0 && lpf(val) == temp)
            ++count;
    }

    return count;
}
