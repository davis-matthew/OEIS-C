/*
 * A346094  a(n) = n / A275823(n), where A275823(n) is the least k such that n
 *          divides phi(k^2).
 * https://oeis.org/A346094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / A275823(n), where A275823(n) is the least k such that
 *            n divides φ(k²) = k·φ(k)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute φ(m) for m ≤ 10000.  The result fits in seq_t.  */
static seq_t phi_of(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t n = m;
    for (idx_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            while (n % p == 0)
                n /= p;
            result -= result / (seq_t)p;
        }
    }
    if (n > 1)
        result -= result / (seq_t)n;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1 – n=0 is not used */
        return 0;

    idx_t best = n;           /* at worst k = n itself */
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            if (d1 < best)
            {
                seq_t phi1 = phi_of(d1);
                idx_t prod1 = d1 * (idx_t)phi1;
                if (prod1 % n == 0)
                    best = d1;
            }

            idx_t d2 = n / i;
            if (d2 != d1 && d2 < best)
            {
                seq_t phi2 = phi_of(d2);
                idx_t prod2 = d2 * (idx_t)phi2;
                if (prod2 % n == 0)
                    best = d2;
            }
        }
    }

    /* Result fits in seq_t; perform division in idx_t to avoid overflow on casting. */
    return (seq_t)(n / best);
}
