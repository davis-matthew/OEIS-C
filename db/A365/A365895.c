/*
 * A365895  Expansion of e.g.f. exp( Sum_{k>=0} x^(5*k+4) / (5*k+4)! ).
 * https://oeis.org/A365895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 ; a(n) = Σ_{k=0..⌊(n-4)/5⌋} binomial(n-1, 5*k+3) *
 *            a(n-5*k-4) for n≥1 (memoised, computed on demand)
 * Range:     u8 n <= 11, u16 n <= 15, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t  max_computed = 0;   /* largest n already computed */
    static size_t cap          = 0;   /* allocated size of cache */

    /* initialise cache */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);           /* out‑of‑memory abort */
        cache[0] = 1;                  /* a(0) = 1 */
        max_computed = 0;
    }

    /* grow cache if necessary */
    if ((size_t)(n + 1) > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap = n + 1;
    }

    /* compute all missing values up to n */
    if (n > max_computed) {
        for (idx_t m = max_computed + 1; m <= n; ++m) {
            if (m < 4) {
                cache[m] = 0;            /* a(1), a(2), a(3) = 0 */
            } else {
                seq_t sum = 0;
                idx_t max_k = (m - 4) / 5;          /* floor((m-4)/5) */
                for (idx_t k = 0; k <= max_k; ++k) {
                    idx_t i    = 5 * k + 3;           /* 5*k+3 */
                    seq_t bin  = binomial(m - 1, i);  /* choose(m‑1,i) */
                    seq_t term = bin * cache[m - 5 * k - 4];
                    sum += term;
                }
                cache[m] = sum;
            }
        }
        max_computed = n;
    }

    return cache[n];
}
