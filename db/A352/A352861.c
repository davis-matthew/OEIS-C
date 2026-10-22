/*
 * A352861  a(n) = 1 + Sum_{k=0..n-1} binomial(n+2,k+3) * a(k).
 * https://oeis.org/A352861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=0..n-1} binomial(n+2,k+3) * a(k)
 *            (equivalently: a(0)=1; a(n)=a(n-1)+Sum_{k=0..n-1}
 *            binomial(n+1,k+2)*a(k) ) The code uses the provided helper seq_t
 *            binomial(idx_t n, idx_t k); to compute binomial coefficients. It
 *            memoises the sequence in a static cache so calls can appear in
 *            any...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* for realloc, calloc */
#include "../utils/binomial.h"   /* binomial helper */

static seq_t *cache = NULL;   /* cache[0] = a(0), cache[1] = a(1), … */
static size_t cache_len = 0;  /* number of entries currently cached */

seq_t a(idx_t n)
{
    /* base case */
    if (n == 0)
        return 1;

    /* initialise cache if first call */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – unlikely */
        cache[0] = 1;                  /* a(0) */
    }

    /* grow cache if needed */
    if (cache_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure – unlikely */
        cache = tmp;

        /* compute missing terms from old_len up to n */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            seq_t sum = 1;                         /* initial +1 term */

            /* start with binomial(i+2, 3) */
            seq_t c = binomial(i + 2, 3);

            for (idx_t k = 0; k < i; ++k) {
                sum += c * cache[k];

                /* update c to binomial(i+2, k+4) for next iteration */
                /* only need to update if k+4 <= i+2, which holds for k < i */
                /* formula: C(n,r+1) = C(n,r) * (n-r)/(r+1) */
                seq_t num = (seq_t)(i - k - 1);      /* (n - r)  with r = k+3 */
                seq_t den = (seq_t)(k + 4);          /* r+1 */
                c = c * num / den;
            }

            cache[i] = sum;
        }
        cache_len = new_len;
    }

    return cache[n];
}
