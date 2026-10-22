/*
 * A304870  L.g.f.: log(Product_{k>=1} (1 + x^k)^(k^k)) = Sum_{n>=1}
 *          a(n)*x^n/n.
 * https://oeis.org/A304870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d+1) * d^(d+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not used, safety guard */

    seq_t pos = 0;
    seq_t neg = 0;

    /* iterate over all divisors d of n */
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0)
            continue;

        /* first divisor d */
        idx_t q = n / d;
        seq_t term = big_pow((seq_t)d, (seq_t)(d + 1));

        /* sign determined by parity of n/d */
        if (q & 1)          /* odd -> positive sign */
            pos += term;
        else                /* even -> negative sign */
            neg += term;

        /* counterpart divisor q, avoid double counting the square root */
        if (d != q)
        {
            seq_t term2 = big_pow((seq_t)q, (seq_t)(q + 1));

            /* sign for q depends on parity of n/q == d */
            if (d & 1)          /* d odd -> positive sign for q */
                pos += term2;
            else
                neg += term2;
        }
    }

    /* final result – guaranteed to be non‑negative for all n≥1 */
    return pos - neg;
}
