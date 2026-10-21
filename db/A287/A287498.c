/*
 * A287498  Number of maximal independent vertex sets (and minimal vertex
 *          covers) in the n-web graph.
 * https://oeis.org/A287498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + 3*a(n-3) + a(n-4) Initial values: a(1) = 0,
 *            a(2) = 4, a(3) = 9, a(4) = 12
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;

    static seq_t *cache = NULL;
    static idx_t cached_n = 0;

    if (n > cached_n) {
        /* Allocate or grow cache to hold indices up to n */
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();      /* Allocation failure */
        cache = new_cache;

        /* Fill in new entries */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            if (i == 1) {
                cache[(size_t)i] = (seq_t)0;
            } else if (i == 2) {
                cache[(size_t)i] = (seq_t)4;
            } else if (i == 3) {
                cache[(size_t)i] = (seq_t)9;
            } else if (i == 4) {
                cache[(size_t)i] = (seq_t)12;
            } else {
                cache[(size_t)i] = cache[(size_t)(i - 2)] * (seq_t)2
                                 + cache[(size_t)(i - 3)] * (seq_t)3
                                 + cache[(size_t)(i - 4)];
            }
        }
        cached_n = n;
    }
    return cache[(size_t)n];
}
