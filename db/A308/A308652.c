/*
 * A308652  a(n) = Product_{k=1..n} sigma(n,k).
 * https://oeis.org/A308652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} sigma(k,n) where sigma(k,n) = sum_{d|k}
 *            d^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_spf.h"
#include "../utils/big_pow.h"

/* static cache for smallest prime factors */
static idx_t *spf_cache = NULL;
static idx_t spf_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                    /* a(0) = 1 by definition */

    /* Ensure spf_cache covers [0..n] */
    if (n >= spf_cap) {
        if (spf_cache) free(spf_cache);
        build_spf(n, &spf_cache);
        spf_cap = n + 1;               /* spf_cache[0..n] inclusive */
    }

    seq_t result = 1;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t tmp = k;
        seq_t sigma = 1;              /* sigma(k,n) */

        while (tmp > 1) {
            idx_t p   = spf_cache[tmp];
            idx_t cnt = 0;

            while ((tmp % p) == 0) {
                tmp /= p;
                ++cnt;
            }

            /* Compute term = (p^{(cnt+1)*n} - 1) / (p^n - 1) */
            seq_t exp_for_p  = (seq_t)n;                                 /* p^n   */
            seq_t exponent   = (seq_t)(cnt + 1) * (seq_t)n;              /* (cnt+1)*n */
            seq_t numerator  = big_pow((seq_t)p, exponent);
            seq_t denominator = big_pow((seq_t)p, exp_for_p) - 1;
            seq_t term = (numerator - 1) / denominator;

            sigma *= term;
        }

        result *= sigma;
    }

    return result;
}
