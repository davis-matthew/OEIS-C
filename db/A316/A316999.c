/*
 * A316999  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 4, 6 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 3*a(n-2) + 7*a(n-3) -48*a(n-4) +5*a(n-5)
 *            +10*a(n-6) +2*a(n-7) -19*a(n-8) -15*a(n-9) -6*a(n-10) -a(n-11)
 *            for n>12 initial values: a(1)=4, a(2)=30, a(3)=162, a(4)=894,
 *            a(5)=4905, a(6)=26952, a(7)=148138, a(8)=814090, a(9)=4474116,
 *            a(10)=24589282, a(11)=135138808, a(12)=742704801
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   alloc_seq_n, free_seq_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_n.h"
#include "../utils/free_seq_n.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n <= 0) return (seq_t)0;

    /* Ensure cache is large enough. */
    if (!cache || cache_cap < (size_t)n) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = alloc_seq_n(new_cap);
        if (!new_cache) return (seq_t)0;  /* allocation failure */
        /* copy existing values if any */
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        if (cache)
            free_seq_n(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n > cache_len) {
        static const seq_t init[12] = {
            (seq_t)4, (seq_t)30, (seq_t)162, (seq_t)894,
            (seq_t)4905, (seq_t)26952, (seq_t)148138,
            (seq_t)814090, (seq_t)4474116, (seq_t)24589282,
            (seq_t)135138808, (seq_t)742704801
        };

        for (idx_t k = cache_len + 1; k <= n; ++k) {
            size_t idx = (size_t)(k - 1);  /* zero‑based index in cache */
            if (k <= 12) {
                cache[idx] = init[(size_t)(k - 1)];
            } else {
                /* compute recurrence */
                seq_t pos = (seq_t)0;
                seq_t neg = (seq_t)0;

                pos += (seq_t)5 * cache[idx - 1];
                pos += (seq_t)3 * cache[idx - 2];
                pos += (seq_t)7 * cache[idx - 3];
                pos += (seq_t)5 * cache[idx - 5];
                pos += (seq_t)10 * cache[idx - 6];
                pos += (seq_t)2 * cache[idx - 7];

                neg += (seq_t)48 * cache[idx - 4];
                neg += (seq_t)19 * cache[idx - 8];
                neg += (seq_t)15 * cache[idx - 9];
                neg += (seq_t)6 * cache[idx - 10];
                neg += (seq_t)1 * cache[idx - 11];

                cache[idx] = pos - neg;  /* pos > neg for all n>12 */
            }
        }
        cache_len = n;
    }

    return cache[(size_t)(n - 1)];
}
