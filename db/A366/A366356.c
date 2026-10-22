/*
 * A366356  G.f. satisfies A(x) = 1/(1 - x) + x/A(x).
 * https://oeis.org/A366356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1, a(1) = 2, for n ≥ 2: a(n) = a(n‑1) + Σ_{i=1}^{n-1}
 *            a(i)·a(n-1-i) - Σ_{i=1}^{n-1} a(i)·a(n-i) + (n==1? 1 : (n==2? -1
 *            : 0)); This recurrence follows from the generating function A(x)
 *            = 1/(1‑x) + x/A(x)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 16, i64 n <= 29, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = 0;   /* largest n computed */

    /* initial allocation and a(0) */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) exit(1);
        cache[0] = (seq_t)1;
        last = 0;
    }

    if (n <= last) {
        return cache[(size_t)n];
    }

    /* grow the cache if required */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache = tmp;
        cap   = new_cap;
    }

    /* compute from last+1 up to n */
    for (idx_t m = last + 1; m <= n; ++m) {
        seq_t sum1 = 0;   /* Σ a(i) a(m-1-i) */
        seq_t sum2 = 0;   /* Σ a(i) a(m-i)   */
        for (idx_t i = 1; i < m; ++i) {
            size_t si  = (size_t)i;
            size_t s1  = (size_t)(m - 1 - i);
            size_t s2  = (size_t)(m - i);
            sum1 += cache[si] * cache[s1];
            sum2 += cache[si] * cache[s2];
        }
        seq_t add = 0;
        if (m == 1) add = (seq_t)1;
        else if (m == 2) add = (seq_t)(-1);
        cache[(size_t)m] = cache[(size_t)(m - 1)] + sum1 - sum2 + add;
    }

    last = n;
    return cache[(size_t)n];
}
