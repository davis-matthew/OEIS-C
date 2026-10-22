/*
 * A385846  G.f. A(x) satisfies A(x) = 1/((1 - x) * (1 - x^5*A''''(x))).
 * https://oeis.org/A385846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 1 a(2) = 1 a(3) = 1 a(4) = 1 For n ≥ 1: a(n) = 1
 *            + Σ_{k=0}^{n-1} (k^4 - 6*k^3 + 11*k^2 - 6*k) * a(k) * a(n-1-k)
 *            (the kernel equals k*(k-1)*(k-2)*(k-3) and vanishes for k < 4)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* The types seq_t and idx_t are provided by the build system. */
extern seq_t a(idx_t n);

/* The function itself. */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;   /* array of a(i) values */
    static size_t cached = 0;     /* number of values currently cached */

    /* expand cache as needed */
    if ((size_t)n+1 > cached) {
        size_t newsize = (size_t)n+1;
        seq_t *newcache = (seq_t*)malloc(newsize * sizeof(seq_t));
        if (!newcache) return (seq_t)0; /* insufficient memory, abort */

        /* copy previous values */
        if (cached > 0) {
            memcpy(newcache, cache, cached * sizeof(seq_t));
            free(cache);
        }
        cache = newcache;

        /* compute new values */
        for (size_t i = cached; i < newsize; ++i) {
            /* a[0] = 1 automatically by following loop */
            seq_t sum = 0;
            for (size_t k = 0; k < i; ++k) {
                /* weight f(k) = k*(k-1)*(k-2)*(k-3)  (vanishes for k<4) */
                if (k < 4) continue;
                seq_t kk = (seq_t)k;
                seq_t weight = kk * (kk - 1) * (kk - 2) * (kk - 3);
                seq_t prod = cache[k] * cache[i - 1 - k];
                prod *= weight;
                sum += prod;
            }
            cache[i] = sum + (seq_t)1;
        }
        cached = newsize;
    }
    return cache[(size_t)n];
}
