/*
 * A367652  G.f. A(x) satisfies A(x) = 1 / (1 - x * (1 + x + x^2) * A(x^3)).
 * https://oeis.org/A367652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..n-1} a(floor(k/3)) * a(n-1-k) (OEIS
 *            A367652)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: values of a(0..len-1) */
    static seq_t *cache = NULL;
    static idx_t len = 0;     /* number of values currently stored (>=1 if initialized) */
    static idx_t cap = 0;     /* allocated capacity of cache (>= len) */

    /* Allocate initial cache and set a(0) = 1 */
    if (len == 0) {
        cap = 16;                  /* initial capacity (any reasonable value) */
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) return 0;        /* allocation failure: return 0 as a safe fallback */
        cache[0] = 1;
        len = 1;
    }

    /* Ensure we have space for all indices up to n */
    if (n >= cap) {
        idx_t new_cap = cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;  /* allocation failure: return 0 */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n (if any) using the recurrence */
    for (idx_t i = len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
            continue;
        }
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            sum += cache[k / 3] * cache[(i - 1) - k];
        }
        cache[i] = sum;
    }

    len = n + 1;   /* we have computed up to index n */
    return cache[n];
}
