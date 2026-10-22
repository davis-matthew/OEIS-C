/*
 * A368345  a(n) = Sum_{k=0..n} 4^(n-k) * floor(k/3).
 * https://oeis.org/A368345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 4*a(n-2) + a(n-3) - 5*a(n-4) + 4*a(n-5) with
 *            initial values a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=5, a(5)=21.
 *            This recurrence is derived from the definition a(n)=∑_{k=0..n}
 *            4^(n-k)*⌊k/3⌋, and it allows computation without dealing with
 *            massive powers of 4
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;     /* allocated size in terms of elements   */
    static size_t computed = 0;    /* number of terms actually computed (>= cap) */

    /* initialise cache on first call */
    if (!cache) {
        cap = 64;                       /* initial capacity */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* out of memory (unlikely in tests)          */
        /* base values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)21;
        computed = 6;
    }

    /* grow cache if necessary to hold up to n */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until sufficient */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        cache = tmp;
        /* initialise newly allocated part to zero (implementation defined) */
        for (size_t i = cap; i < new_cap; ++i) cache[i] = (seq_t)0;
        cap = new_cap;
    }

    /* compute missing terms up to n using the 5‑term recurrence */
    if (n >= computed) {
        for (idx_t k = computed; k <= n; ++k) {
            seq_t term;
            /* recurrence: a(k)=5*a(k-1)-4*a(k-2)+a(k-3)-5*a(k-4)+4*a(k-5)   */
            term = (5 * cache[k-1]) - (4 * cache[k-2]) + cache[k-3]
                   - (5 * cache[k-4]) + (4 * cache[k-5]);
            cache[k] = term;
        }
        computed = (size_t)n + 1;
    }

    return cache[n];
}
