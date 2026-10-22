/*
 * A386449  G.f. A(x) satisfies A(x) = 1/(1 - x - x^4*A'''(x)).
 * https://oeis.org/A386449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = a(n-1) + Σ_{k=0..n-1} (2*k - 3*k^2 + k^3) * a(k)
 *            * a(n-1-k) (n >= 1) (This implementation uses memoisation to
 *            allow calls in any order.)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* The function returns the nth term of the sequence A386449.
   The types seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t  cap       = 0;   /* allocated capacity of cache */
    static idx_t   last      = (idx_t)-1; /* last computed index */

    /* allocate and initialise cache when first called */
    if (cache == NULL) {
        cap = 16;                         /* initial capacity */
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!cache) return 0;             /* allocation failure – unlikely */
        for (size_t i = 0; i < cap; ++i) cache[i] = 0;
        cache[0] = 1;                     /* a(0) = 1 */
        last = 0;
    }

    /* grow cache if necessary */
    if ((idx_t)(cap-1) < n) {
        size_t newcap = cap;
        while ((idx_t)(newcap-1) < n) newcap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure – unlikely */
        cache = tmp;
        for (size_t i = cap; i < newcap; ++i) cache[i] = 0;
        cap = newcap;
    }

    /* compute terms up to n if they are not cached yet */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            /* compute coefficient (2*k - 3*k^2 + k^3) in a wider type to avoid overflow */
            unsigned long long kk = (unsigned long long)k;
            unsigned long long coeff64 =
                2ULL * kk
              - 3ULL * kk * kk
              +       kk * kk * kk;
            seq_t coeff = (seq_t)coeff64;

            seq_t term = coeff * cache[k];
            term = term * cache[i - 1 - k];
            sum = sum + term;
        }
        cache[i] = cache[i-1] + sum;
    }
    last = n;

    return cache[n];
}
