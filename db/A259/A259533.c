/*
 * A259533  Number of restricted barred preferential arrangements of an n-set
 *          having 3 bars in which 3 fixed sections are restricted sections
 *          and 1 section is a free section.
 * https://oeis.org/A259533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + ∑_{k=0}^{n-1} binomial(n,k) * a(k), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_cache.h"

static seq_t *a_cache = NULL;      /* cached values of a(n)          */
static size_t a_cap   = 0;         /* current allocation size       */
static idx_t a_done   = 0;         /* number of computed terms       */
static seq_t a_pow3   = 0;         /* 3^k for the largest computed k */

seq_t a(idx_t n)
{
    /* initialise on first call */
    if (a_done == 0) {
        a_pow3 = 1;   /* 3^0 */
        a_done = 0;
    }

    /* grow the cache if necessary */
    if ((idx_t)n + 1 > (idx_t)a_cap)
        ensure_cache(n + 1, &a_cache, &a_cap);

    /* compute missing terms */
    for (idx_t i = a_done; i <= n; ++i) {
        if (i == 0) {
            a_cache[0] = 1;          /* a(0)=1 */
            a_pow3     = 1;          /* 3^0 */
            a_done = 1;
            continue;
        }

        a_pow3 = a_pow3 * (seq_t)3;   /* a_pow3 = 3^i */

        seq_t sum    = 0;
        seq_t binom  = 1;            /* binomial(i,0) */

        for (idx_t k = 0; k < i; ++k) {
            sum += binom * a_cache[k];

            /* update binom for next k: binom = binom * (i-k)/(k+1) */
            seq_t tmp = binom * (seq_t)(i - k);
            binom = tmp / (seq_t)(k + 1);
        }

        a_cache[i] = a_pow3 + sum;
        ++a_done;
    }

    return a_cache[n];
}
