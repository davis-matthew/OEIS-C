/*
 * A371018  Expansion of e.g.f. (1/x) * Series_Reversion( x/(1 + x^2*exp(x))
 *          ).
 * https://oeis.org/A371018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!/(n+1)) * Sum_{k=0..floor(n/2)} k^(n-2*k) *
 *            binomial(n+1,k) / (n-2*k)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* product_last = n!/(n-2*k)!  = ∏_{t=n-2k+1}^{n} t  */
        seq_t product_last = 1;
        idx_t start = n - 2 * k + 1;
        for (idx_t t = start; t <= n; ++t)
            product_last *= t;

        /* binomial coefficient */
        seq_t bin = binomial(n + 1, k);

        /* k^(n-2k) */
        seq_t k_pow = big_pow((seq_t)k, (seq_t)(n - 2 * k));

        /* term = k_pow * bin * product_last / (n+1) */
        seq_t term;
        if (sizeof(seq_t) <= 8)  /* 8‑byte (or smaller) types: use 128‑bit intermediate */
        {
            unsigned __int128 mult =
                (unsigned __int128)product_last *
                (unsigned __int128)bin *
                (unsigned __int128)k_pow;
            term = (seq_t)(mult / (n + 1));
        }
        else                       /* larger types or arbitrary‑precision: use native ops */
        {
            seq_t tmp = product_last * bin;
            tmp = tmp * k_pow;
            term = tmp / (seq_t)(n + 1);
        }

        sum += term;
    }

    return sum;
}
