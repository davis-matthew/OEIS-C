/*
 * A386208  G.f. A(x) satisfies A(x) = 1/(1-x) + x^2 * (d/dx A(x)^2).
 * https://oeis.org/A386208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (n-1) * Sum_{k=0}^{n-1} a(k)*a(n-1-k) a(0) = 1, a(1)
 *            = 1 Recurrence uses convolution of the sequence with itself
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;     /* allocated capacity               */
    static idx_t last  = 0;      /* highest index computed so far    */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cap = (n < 1) ? 2 : ((size_t)n + 1);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = 1;
        cache[1] = 1;
        last = 1;
        if (n == 0) return cache[0];
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap;
        while ((size_t)n + 1 > newcap) newcap = newcap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap = newcap;
    }

    /* Compute terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            sum += cache[k] * cache[i - 1 - k];
        }
        cache[i] = 1 + (seq_t)(i - 1) * sum;
    }
    last = n;

    return cache[n];
}
