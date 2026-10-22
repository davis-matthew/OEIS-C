/*
 * A328007  Expansion of e.g.f. 1 / (2 - exp(-x) / (1 - x)).
 * https://oeis.org/A328007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   // OEIS A328007: Expansion of e.g.f. 1 / (2 - exp(-x) / (1 - x))
   // a(0) = 1
   // a(n) = Σ_{k=1..n} binomial(n,k) * !k * a(n-k)     (n≥1)
   // where !k are derangements: !0 = 1, !1 = 0,
   // !k = (k-1) * (! (k-1) + ! (k-2))   for k≥2
*/

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *a_cache = NULL;
    static seq_t *d_cache = NULL;
    static idx_t a_max = 0, d_max = 0;
    static size_t a_cap = 0, d_cap = 0;

    /* ---- ensure derangement cache up to n --------------------------------*/
    if (n > d_max) {
        /* make sure the derangement array has room */
        size_t need = (size_t)(n + 1);
        if (need > d_cap) {
            void *tmp = realloc(d_cache, need * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failure -> undefined behaviour */
            d_cache = (seq_t *)tmp;
            d_cap   = need;
        }

        /* initialise or extend the derangement table */
        if (d_max == 0) {
            d_cache[0] = 1;           /* !0 = 1 */
            d_max = 0;
        }
        if (d_max < 1 && n >= 1) {
            d_cache[1] = 0;           /* !1 = 0 */
            d_max = 1;
        }
        for (idx_t k = d_max + 1; k <= n; ++k) {
            seq_t sum = d_cache[k - 1] + d_cache[k - 2];
            d_cache[k] = (seq_t)(k - 1) * sum;   /* !k = (k-1)( !(k-1)+!(k-2) ) */
        }
        d_max = n;
    }

    /* ---- ensure sequence cache up to n ------------------------------------*/
    if (n == 0) return 1;  /* a(0) = 1 */
    if (n > a_max) {
        /* make sure the value array has room */
        size_t need = (size_t)(n + 1);
        if (need > a_cap) {
            void *tmp = realloc(a_cache, need * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failure */
            a_cache = (seq_t *)tmp;
            a_cap   = need;
        }

        /* initialise or extend the sequence table */
        if (a_max == 0) {
            a_cache[0] = 1;  /* a(0) = 1 */
            a_max = 0;
        }

        for (idx_t i = a_max + 1; i <= n; ++i) {
            seq_t sum = 0;
            seq_t binom = (seq_t)i;           /* C(i,1) */
            for (idx_t k = 1; k <= i; ++k) {
                /* term = C(i,k) * !k * a(i-k) */
                seq_t term = binom * d_cache[k];
                term = term * a_cache[i - k];
                sum += term;

                /* update binom to C(i,k+1) */
                binom = binom * (seq_t)(i - k);
                binom = binom / (k + 1);
            }
            a_cache[i] = sum;
        }
        a_max = n;
    }

    return a_cache[n];
}
