/*
 * A280822  Expansion of phi(-x) * f(-x)^2 * f(-x^6)^3 in powers of x where
 *          phi(), chi(), f() are Ramanujan theta functions.
 * https://oeis.org/A280822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. = ∏_{k>0} (1 - x^k)^4 · (1 - x^{2k})^2 · (1 + x^{2k} +
 *            x^{4k})^3
 * Range:     i8 n <= 55, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   exp_for_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/exp_for_m.h"

/* Pre‑computed binomial coefficients for 0≤c≤6 */
static const seq_t binom[7][7] = {
    {0,0,0,0,0,0,0},
    {1,1,0,0,0,0,0},
    {1,2,1,0,0,0,0},
    {1,3,3,1,0,0,0},
    {1,4,6,4,1,0,0},
    {1,5,10,10,5,1,0},
    {1,6,15,20,15,6,1}
};

seq_t a(idx_t n)
{
    /* Cached coefficients up to current maximum */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Recompute if we need a larger index */
    if (n >= cache_size)
    {
        idx_t new_size = n + 1;
        seq_t *coeff  = (seq_t*)calloc(new_size, sizeof(seq_t));
        seq_t *temp   = (seq_t*)calloc(new_size, sizeof(seq_t));
        if (!coeff || !temp) return 0;          /* memory failure – unlikely in tests */

        coeff[0] = (seq_t)1;                     /* a(0)=1 */
        seq_t *cur = coeff;
        seq_t *nxt = temp;

        for (idx_t m = 1; m <= n; ++m)
        {
            int e = exp_for_m(m);

            /* clear next buffer */
            for (idx_t i = 0; i <= n; ++i) nxt[i] = 0;

            /* multiply current polynomial by (1 - x^m)^e */
            for (idx_t i = 0; i <= n; ++i)
            {
                seq_t val = cur[i];
                if (!val) continue;

                /* term k = 0 */
                nxt[i] += val;

                /* terms k = 1 .. e */
                for (int k = 1; k <= e; ++k)
                {
                    idx_t idx = i + (idx_t)k * m;
                    if (idx > n) break;
                    seq_t coef = binom[e][k];
                    if (k & 1) coef = -coef;
                    nxt[idx] += val * coef;
                }
            }

            /* swap current and next buffers */
            seq_t *tmp = cur;
            cur = nxt;
            nxt = tmp;
        }

        /* Final coefficients are in cur.  Copy into cache. */
        free(cache);
        cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        for (idx_t i = 0; i < new_size; ++i) cache[i] = cur[i];
        cache_size = new_size;

        /* free temporary buffers */
        free(cur);
        free(nxt);
    }

    return cache[n];
}
