/*
 * A400698  a(n) = Sum_{k=0..n} mu(k^2 + 1).
 * https://oeis.org/A400698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, new
 * Formula:   a(n) = Sum_{k=0..n} mu(k^2 + 1)
 * Range:     i8 n <= 4220, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mu_of, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mu_of.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* static cache of cumulative sums */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;   /* number of elements stored (>=1) */

    /* grow cache if necessary */
    if ((size_t)n + 1 > cached_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = NULL;
        if (cache) {
            new_cache = seq_realloc(cache, cached_len, new_len);
        } else {
            new_cache = seq_alloc(new_len);
        }
        /* Initialize new entries */
        for (size_t k = cached_len; k < new_len; ++k) {
            idx_t m = k * k + 1;                       /* k^2 + 1 fits in idx_t */
            seq_t mu = (seq_t)mu_of(m);                /* mu_of returns int */
            if (k == 0) {
                new_cache[k] = mu;                     /* a(0) = mu(1) = 1 */
            } else {
                new_cache[k] = new_cache[k - 1] + mu;   /* cumulative sum */
            }
        }
        cache = new_cache;
        cached_len = new_len;
    }

    return cache[n];
}
