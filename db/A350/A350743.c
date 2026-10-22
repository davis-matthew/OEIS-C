/*
 * A350743  Number of numbers k, 1 <= k <= n, such that k | sigma_k(n).
 * https://oeis.org/A350743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{1≤k≤n} [k divides σ_k(n)] (σ_k(n) = sum of k‑th powers
 *            of divisors of n)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* collect all divisors of n */
    seq_t divs[128];
    size_t divcnt = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            divs[divcnt++] = (seq_t)d;
            if (d * d != n && divcnt < 128)
                divs[divcnt++] = (seq_t)(n / d);
        }
    }

    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t modk = (seq_t)k;
        seq_t sum_mod = 0;
        for (size_t i = 0; i < divcnt; ++i)
        {
            sum_mod += mod_pow(divs[i], (uint64_t)k, modk);
            if (sum_mod >= modk) sum_mod -= modk;  /* keep modulo k */
        }
        if (sum_mod == 0)
            ++count;
    }
    return count;
}
