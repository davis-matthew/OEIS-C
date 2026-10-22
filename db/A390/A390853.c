/*
 * A390853  a(n) = Sum_{k=0..n} (-1)^k * binomial(n+k+2,n-k) * Fibonacci(k+1).
 * https://oeis.org/A390853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A390853: a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3) - a(n-4)
   initial values: a(0)=1, a(1)=2, a(2)=3, a(3)=6 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache: a vector of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* last computed index + 1 */
    static idx_t cache_cap  = 0;   /* capacity of cache */

    /* enlarge cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* unrecoverable allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* initialize first four terms if not yet done */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)6;
        cache_size = 4;
    }

    /* compute terms up to n */
    if (n >= cache_size) {
        for (idx_t i = cache_size; i <= n; ++i) {
            /* recurrence: a(i) = 3*a(i-1) - 3*a(i-2) + 3*a(i-3) - a(i-4) */
            seq_t val = 3 * cache[i-1] - 3 * cache[i-2] + 3 * cache[i-3] - cache[i-4];
            cache[i] = val;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
