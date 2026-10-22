/*
 * A352080  a(n) is the number of times that the square root operation must be
 *          applied to n in order to reach an irrational number.
 * https://oeis.org/A352080
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_i v₂(eᵢ) + 1, where n = ∏ pᵢ^eᵢ, and v₂(k) is the
 *            exponent of 2 in the factorization of k (i.e., the number of
 *            trailing zero bits of k). For n ≥ 2. a(1) is undefined; this
 *            function returns 0 for n < 2
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   v2_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/v2_of.h"

seq_t a(idx_t n)
{
    if (n < 2u)            /* a(1) is undefined, return 0 for safety */
        return 0;

    idx_t tmp = n;
    unsigned min_v2 = (unsigned)-1; /* start with a very large value */

    /* factor the integer by trial division */
    for (idx_t p = 2u; p <= tmp / p; ++p)
    {
        if (tmp % p == 0u)
        {
            unsigned exp = 0u;
            while (tmp % p == 0u)
            {               /* count exponent of prime p */
                tmp /= p;
                ++exp;
            }
            /* update the minimal 2‑adic valuation */
            unsigned v2 = v2_of(exp);
            if (v2 < min_v2)
                min_v2 = v2;
            if (min_v2 == 0u)          /* odd exponent ⇒ answer is 1 */
                return 1;
        }
    }

    /* If a prime factor remains, its exponent is 1 (odd) */
    if (tmp > 1u)
        return 1;

    /* All exponents are even; return the minimal valuation + 1 */
    return (seq_t)(min_v2 + 1);
}
