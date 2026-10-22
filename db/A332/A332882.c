/*
 * A332882  If n = Product (p_j^k_j) then a(n) = numerator of Product (1 +
 *          1/p_j^k_j).
 * https://oeis.org/A332882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Product_{p^k||n} (1 + 1/p^k) = \
 *            Product_{p^k||n} (p^k + 1) / gcd(Product_{p^k||n} (p^k + 1), n)
 * Range:     u8 n <= 200, u16 n <= 9713, u32 n <= 9713, u64 n <= 9713,
 *            u128 n <= 9713, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the integer value of a(n).  The function is valid for any n >= 1. */
#include <stdint.h>
#include "../utils/gcd.h"            /* seq_t gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition. */
    if (n <= 1)
        return 1;

    uint64_t temp = (uint64_t)n;      /* working copy of n for factorisation */
    seq_t   product = 1;              /* product of (p^k + 1) for all prime powers */

    /* trial‑division factorisation of temp (up to sqrt(temp)) */
    for (uint64_t d = 2; d <= temp / d; ++d)
    {
        if (temp % d == 0)
        {
            seq_t pk = 1;            /* p^k */
            while (temp % d == 0)
            {
                temp /= d;
                pk *= (seq_t)d;       /* multiply by the prime for each power */
            }
            product *= (pk + (seq_t)1);
        }
    }

    /* If temp > 1, it is a prime factor with exponent 1 */
    if (temp > 1)
        product *= (((seq_t)temp) + (seq_t)1);

    /* Reduce the product by its gcd with n to obtain the numerator */
    seq_t g = gcd(product, (seq_t)n);
    return product / g;
}
