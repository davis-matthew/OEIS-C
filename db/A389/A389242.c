/*
 * A389242  Expansion of e.g.f. exp(x^5/120 * cosh(x)).
 * https://oeis.org/A389242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to choose five elements from each block of
 *            the partitions of an n‑set into odd blocks. Equivalent
 *            exponential generating function: exp( x^5/120 * cosh(x) ).
 *            Computed via the standard recurrence for E(x)=exp(F(x)) where
 *            F(x)=∑_{k≥1} f_k x^k/k! and f_k=binom(k,5) for odd k≥5,...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 18, u64 n <= 28, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *a_cache = NULL;      /* a[0..cached_n] */
    static seq_t *g_cache = NULL;      /* g[0..cached_n] (f_k) */
    static idx_t cached_n = 0;         /* largest n computed */

    /* initialise for n==0 or first call */
    if (n == 0) {
        if (!a_cache) {
            a_cache = (seq_t*)malloc(sizeof(seq_t));
            if (!a_cache) return 0;            /* out of memory */
            a_cache[0] = (seq_t)1;
            g_cache = (seq_t*)malloc(sizeof(seq_t));
            if (!g_cache) return 0;
            g_cache[0] = (seq_t)0;
            cached_n = 0;
        }
        return a_cache[0];
    }

    /* enlarge caches if needed */
    if (n > cached_n) {
        idx_t new_size = n + 1;
        seq_t *new_a = (seq_t*)realloc(a_cache, new_size * sizeof(seq_t));
        if (!new_a) return 0;
        a_cache = new_a;
        seq_t *new_g = (seq_t*)realloc(g_cache, new_size * sizeof(seq_t));
        if (!new_g) return 0;
        g_cache = new_g;

        /* compute g values for indices > cached_n */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            if ((i & (idx_t)1) && i >= 5) {
                /* binom(i,5) = i*(i-1)*(i-2)*(i-3)*(i-4)/120 */
                seq_t val = (seq_t)i;
                val = val * (seq_t)(i - 1);
                val = val * (seq_t)(i - 2);
                val = val * (seq_t)(i - 3);
                val = val * (seq_t)(i - 4);
                val = val / (seq_t)120;
                g_cache[i] = val;
            } else {
                g_cache[i] = (seq_t)0;
            }
        }

        /* compute a values up to n */
        for (idx_t m = cached_n; m < n; ++m) {   /* m = current n-1 */
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;               /* binom(m,0) */

            for (idx_t k = 0; k <= m; ++k) {
                seq_t gk1 = g_cache[k + 1];
                if (gk1) {
                    seq_t prod = gk1 * binom;
                    prod = prod * a_cache[m - k];
                    sum = sum + prod;
                }
                if (k < m) {
                    /* binom(m,k+1) = binom(m,k)*(m-k)/(k+1) */
                    binom = binom * (seq_t)(m - k);
                    binom = binom / (seq_t)(k + 1);
                }
            }
            a_cache[m + 1] = sum;
        }
        cached_n = n;
    }

    return a_cache[n];
}
