/*
 * A385945  a(0) = 1; a(n) = Sum_{k=0..n-1} binomial(k+3,3) * binomial(n-1,k)
 *          * a(k) * a(n-1-k).
 * https://oeis.org/A385945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = sum_{k=0..n-1} binomial(k+3,3) *
 *            binomial(n-1,k) * a(k) * a(n-1-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t sz = 0;  /* number of computed elements in cache */

    /* ensure base cases */
    if (n == 0) {
        if (sz == 0) {
            size_t need = 1;
            cache = (seq_t*)realloc(cache, need * sizeof(seq_t));
            if (!cache) return (seq_t)0;
            cache[0] = (seq_t)1;
            sz = 1;
            capacity = need;
        }
        return cache[0];
    }

    if (sz <= 1) {
        size_t need = 2;
        cache = (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        if (sz == 0) {
            cache[0] = (seq_t)1;
            sz = 1;
        }
        cache[1] = (seq_t)1;
        sz = 2;
        capacity = need;
    }

    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity ? capacity * 2 : 1;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        capacity = new_cap;
    }

    if ((size_t)n + 1 <= sz)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t cur = sz; cur <= n; ++cur) {
        seq_t sum = (seq_t)0;
        idx_t m = cur - 1;
        seq_t bin = (seq_t)1;          /* binom(m,0) */
        for (idx_t k = 0; k <= m; ++k) {
            seq_t Ck = (seq_t)((k + 3) * (k + 2) * (k + 1) / 6); /* (k+3 choose 3) */
            seq_t term = Ck * bin;
            term = term * cache[k];
            term = term * cache[cur - 1 - k];
            sum += term;

            if (k != m)
                bin = bin * (m - k) / (k + 1);  /* update binom(m,k+1) */
        }
        cache[cur] = sum;
    }
    sz = n + 1;
    return cache[n];
}
