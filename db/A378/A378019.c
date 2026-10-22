/*
 * A378019  E.g.f. satisfies A(x) = (1+x) * exp( x * (1+x) * A(x) ).
 * https://oeis.org/A378019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A378019
   E.g.f. satisfies A(x) = (1+x) * exp( x * (1+x) * A(x) ).
   Formula: a(n) = n! * Sum_{k=0..n} (k+1)^(k-1) * binomial(2*k+1, n-k) / k!   */

#include <stdlib.h>
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */
#include "../utils/seq_pow.h"    /* seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /*------------------ static caches -------------------------------------*/
    static seq_t *cache    = NULL;   /* cached a(i) values   */
    static idx_t cache_len = 0;      /* number of cached values */

    static seq_t *Kpow    = NULL;   /* Kpow[k] = (k+1)^(k-1)  */
    static idx_t Kpow_len = 0;      /* size of Kpow array    */

    /*------------------ increase cache size if needed ----------------------*/
    if (n >= cache_len) {
        idx_t new_cap = n + 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();             /* allocation failed */

        /* extend Kpow array */
        if (Kpow_len <= n) {
            idx_t old_len = Kpow_len;
            idx_t new_K_cap = n + 1;
            Kpow = (seq_t*)realloc(Kpow, new_K_cap * sizeof(seq_t));
            if (!Kpow) abort();          /* allocation failed */
            for (idx_t k = old_len; k <= n; ++k) {
                if (k == 0)
                    Kpow[0] = (seq_t)1;
                else
                    Kpow[k] = seq_pow((seq_t)(k + 1), (idx_t)(k - 1));
            }
            Kpow_len = new_K_cap;
        }

        /* compute a(i) for all new indices i in [cache_len .. n] */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            seq_t prod = (seq_t)1;      /* prod holds ∏_{j=k+1}^{i} j   */

            /* iterate k from i down to the smallest k with non‐zero
               binomial(2k+1, i-k).  When 2k+1 < i-k the binomial is 0.  */
            for (int k = (int)i; k >= 0; --k) {
                if ((2 * k + 1) < (i - k))   /* binomial zero for smaller k */
                    break;

                seq_t bin = binomial((idx_t)(2 * k + 1), (idx_t)(i - k));
                seq_t term = Kpow[(idx_t)k];
                term = term * prod;
                term = term * bin;
                sum  = sum + term;

                if (k > 0)
                    prod = prod * (seq_t)k;   /* prepare product for next k-1 */
            }
            cache[i] = sum;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
