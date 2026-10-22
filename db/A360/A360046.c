/*
 * A360046  a(n) = Sum_{k=0..floor(n/4)} binomial(n+3,4*k+3) * Catalan(k).
 * https://oeis.org/A360046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n+3,3) + Sum_{k=0}^{n-4} a(k)*a(n-4-k) (n≥4)
 *            Initial values: a(0)=1, a(1)=4, a(2)=10, a(3)=20 (See OEIS
 *            A360046 for terms)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 28, u64 n <= 54, u128 n <= 105,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    /* next index that must be computed (0 means nothing computed yet) */
    static idx_t next_to_compute = 0;

    /* enlarge cache if required */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();               /* out‑of‑memory abort */
        cache      = tmp;
        cache_size = new_size;
    }

    /* compute all remaining terms up to n */
    for (idx_t i = next_to_compute; i <= n; ++i) {
        /* part coming from the 1/(1-x)^4 term */
        seq_t term = binomial(i + 3, 3);

        /* convolution part: a(k) * a(i-4-k) for all k with 0<=k<=i-4 */
        if (i >= 4) {
            seq_t sum = 0;
            for (idx_t k = 0; k <= i - 4; ++k) {
                /* multiply using seq_t operators; works for both built‑in
                   integers and arbitrary‑precision class types           */
                seq_t prod = cache[k] * cache[i - 4 - k];
                sum += prod;
            }
            term += sum;
        }

        cache[i] = term;
    }
    next_to_compute = n + 1;  /* all indices up to n are now computed */

    return cache[n];
}
