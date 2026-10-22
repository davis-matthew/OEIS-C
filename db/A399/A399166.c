/*
 * A399166  G.f. A(x) satisfies A(x) = 1/(1 - x * A(x^4)^2).
 * https://oeis.org/A399166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n)=Sum_{j=0..floor((n-1)/4)} a(n-1-4*j) * Sum_{k=0..j}
 *            a(k)*a(j-k)
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 55, u64 n <= 108,
 *            u128 n <= 214, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for a(k) */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t sz = 0;

    /* static cache for convolution c(j) = Sum_{k=0..j} a(k)*a(j-k) */
    static seq_t *conv = NULL;
    static size_t c_cap = 0;
    static size_t c_sz = 0;

    /* initialise for n==0 */
    if (n == 0) {
        if (sz == 0) {
            cache = (seq_t*)malloc(sizeof(seq_t));
            if (!cache) exit(1);
            cap = 1;
            sz = 1;
            conv = (seq_t*)malloc(sizeof(seq_t));
            if (!conv) exit(1);
            c_cap = 1;
            c_sz = 1;
            cache[0] = 1;
            conv[0] = 1;   /* c(0) = a(0)*a(0) */
        }
        return cache[0];
    }

    /* grow cache if necessary */
    if (n + 1 > cap) {
        size_t newcap = cap ? cap * 2 : n + 1;
        if (newcap < n + 1) newcap = n + 1;
        seq_t *newcache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!newcache) exit(1);
        cache = newcache;
        cap = newcap;
    }

    /* grow convolution cache if necessary */
    if (conv == NULL || cap > c_cap) {
        size_t newc = cap;
        seq_t *newconv = (seq_t*)realloc(conv, newc * sizeof(seq_t));
        if (!newconv) exit(1);
        conv = newconv;
        c_cap = newc;
    }

    /* compute missing terms up to n */
    for (size_t i = sz; i <= n; ++i) {
        seq_t val;

        if (i == 0) {
            val = 1;                /* a(0) */
            cache[0] = val;
            conv[0] = 1;            /* c(0) = a(0)*a(0) */
            continue;
        }

        /* a(i) = sum_{j=0..floor((i-1)/4)} a(i-1-4*j) * c(j) */
        seq_t sum = 0;
        idx_t jmax = (idx_t)((i - 1) / 4);
        for (idx_t j = 0; j <= jmax; ++j) {
            idx_t idx = (idx_t)(i - 1 - 4 * j);
            sum += cache[idx] * conv[j];
        }
        val = sum;
        cache[i] = val;

        /* compute c(i) = sum_{k=0..i} a(k)*a(i-k) */
        seq_t cs = 0;
        for (idx_t k = 0; k <= i; ++k) {
            cs += cache[k] * cache[i - k];
        }
        conv[i] = cs;
    }

    sz = n + 1;
    return cache[n];
}
