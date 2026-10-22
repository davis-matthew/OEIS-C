/*
 * A369082  Expansion of g.f. A(x) satisfying A(x) = 1 + x*(3*A(x)^2 +
 *          A(-x)^2)/4.
 * https://oeis.org/A369082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 44, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A369082: a(0)=1, for n≥1 let S = Σ_{i=0}^{n-1} a(i)*a(n-1-i);
   If n is odd, a(n)=S; if n is even, a(n)=S/2.
   This follows from A(x)=1+x*(3*A(x)^2+A(-x)^2)/4. */
#include <stdlib.h>

static seq_t *cache = NULL;              /* memoized terms */
static idx_t cache_len = 0;              /* number of slots in cache */
static idx_t computed = 0;               /* highest index computed */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* grow cache if needed */
    if (!cache || cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();                     /* out of memory */
        cache = tmp;
        for (size_t i = cache_len; i < new_len; ++i) cache[i] = 0;
        cache_len = new_len;
    }

    /* initialise a(0) once */
    if (cache[0] == 0) cache[0] = (seq_t)1;

    /* compute any missing terms up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t sum = 0;
        for (idx_t i = 0; i < k; ++i)
            sum += cache[i] * cache[k - 1 - i];
        if (k & 1)          /* n odd */
            cache[k] = sum;
        else                /* n even */
            cache[k] = sum / ((seq_t)2);
    }
    computed = n;
    return cache[n];
}
