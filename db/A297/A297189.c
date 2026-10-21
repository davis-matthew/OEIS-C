/*
 * A297189  Expansion of (x + 3*x^2 - 2*x^3 - 3*x^4)/(1 - 8*x^2 + 9*x^4).
 * https://oeis.org/A297189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-2) - 9*a(n-4) (a(0)=0, a(1)=1, a(2)=3, a(3)=6,
 *            a(4)=21)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS sequence A297189. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap    = 0;
    static idx_t  next   = 5;          /* first index that has not been computed yet */
    static int   init    = 0;

    /* One‑time initialization of the first five terms. */
    if (!init) {
        cap   = 5;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)21;
        init   = 1;
    }

    /* Trivial case: one of the first five terms. */
    if ((size_t)n < 5) return cache[(size_t)n];

    /* Ensure the cache is large enough. */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t new_cap = need;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any terms that have not yet been cached. */
    if ((idx_t)n >= next) {
        for (size_t i = (size_t)next; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)8 * cache[i - 2] - (seq_t)9 * cache[i - 4];
        }
        next = (idx_t)n + 1;
    }

    return cache[(size_t)n];
}
