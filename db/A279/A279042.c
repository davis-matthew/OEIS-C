/*
 * A279042  Numbers k such that 2*k+1 and 10*k+1 are both triangular numbers
 *          (A000217).
 * https://oeis.org/A279042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=4455, a(2)=30537, a(3)=461938302, a(4)=3166172226,
 *            a(5)=47894687058501; for n>5:
 *            a(n)=a(n-1)+103682*a(n-2)-103682*a(n-3)-a(n-4)+a(n-5) (linear
 *            recurrence)
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation: 1‑based indices, cache[0] unused */
    static seq_t *cache = NULL;
    static size_t max_n = 0;  /* highest n computed so far */
    static size_t cap = 0;    /* allocated size of cache (1‑based) */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 6;  /* indices 0..5 (0 unused) */
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);

        /* store initial terms */
        cache[1] = (seq_t)4455UL;
        cache[2] = (seq_t)30537UL;
        cache[3] = (seq_t)461938302ULL;
        cache[4] = (seq_t)3166172226ULL;
        cache[5] = (seq_t)47894687058501ULL;
        max_n = 5;
    }

    /* if requested n is already computed, just return it */
    if (n <= max_n) {
        return cache[(size_t)n];
    }

    /* grow cache if necessary */
    size_t desired = (size_t)n + 1;  /* 1‑based */
    if (desired > cap) {
        size_t new_cap = cap;
        while (new_cap < desired) {
            new_cap <<= 1;  /* double */
        }
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cap = new_cap;
    }

    /* compute terms from max_n+1 up to n using the recurrence */
    for (size_t i = max_n + 1; i <= (size_t)n; ++i) {
        seq_t term1 = cache[i-1];
        seq_t term2 = (seq_t)103682 * cache[i-2];
        seq_t term3 = (seq_t)103682 * cache[i-3];
        cache[i] = term1 + term2 - term3 - cache[i-4] + cache[i-5];
    }
    max_n = (size_t)n;
    return cache[(size_t)n];
}
