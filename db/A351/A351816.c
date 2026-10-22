/*
 * A351816  G.f. A(x) satisfies: A(x) = 1 + x * A(x/(1 - x)^3) / (1 - x)^3.
 * https://oeis.org/A351816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n+2*k+1, n-k-1) * a(k) a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"

/* Cached values of the sequence.  The cache grows automatically
   when a new index is requested.  The implementation works for
   any seq_t type, whether it is a built‑in integer or an
   arbitrary‑precision class. */
seq_t a(idx_t n)
{
    /* static cache structures */
    static seq_t *cache   = NULL;   /* array of values */
    static size_t cap     = 0;      /* allocated size */
    static idx_t max_comp = 0;      /* largest index computed (>=0) */

    /* initialisation: compute a(0) */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;       /* allocation failure */
        cache[0] = (seq_t)1;
        max_comp = 0;
    }

    /* expand cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 1;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;         /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* already computed? */
    if ((size_t)n <= max_comp) return cache[n];

    /* compute a(i) for i = max_comp+1 ... n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t term = binomial(i + 2 * k + 1, i - k - 1);
            term *= cache[k];
            sum += term;
        }
        cache[i] = sum;
    }

    max_comp = n;
    return cache[n];
}
