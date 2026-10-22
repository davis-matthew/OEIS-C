/*
 * A381131  If n = (p_1^e_1)*(p_2^e_2)*(p_3^e_3)*... and
 *          min(p_1^e_1,p_2^e_2,...) = p_k^e_k then a(n) = p_k, a(1) = 1.
 * https://oeis.org/A381131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the prime factor p of the smallest prime‑power divisor of
 *            n (i.e., min(p^e) over all prime power divisors p^e of n).
 *            Special case a(1) = 1
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the special case n = 1 */
    if (n == 1)
        return (seq_t)1;

    /* Work on a 64‑bit copy of n for factoring */
    uint64_t N = (uint64_t)n;
    uint64_t temp = N;

    /* Keep track of the minimal p^e found and its base prime p */
    uint64_t min_p        = 0;
    uint64_t min_pow_val  = UINT64_MAX;   /* start with a huge value */

    /* Trial division up to sqrt(temp) */
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            /* Remove all copies of p to get the exponent e */
            uint64_t e   = 0;
            uint64_t pow = 1;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
                pow *= p;    /* pow = p^e */
            }

            /* Update the minimal prime power if this one is smaller */
            if (pow < min_pow_val)
            {
                min_pow_val = pow;
                min_p       = p;
            }
        }
    }

    /* If a prime factor larger than sqrt(N) remains, it's a single
       prime power of exponent 1. */
    if (temp > 1)
    {
        /* temp itself is prime, so its power is temp^1 = temp */
        if (temp < min_pow_val)
        {
            min_pow_val = temp;
            min_p       = temp;
        }
    }

    /* min_p now holds the desired prime */
    return (seq_t)min_p;
}
