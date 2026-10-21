/*
 * A293017  E.g.f.: exp(-x^3 * exp(x)).
 * https://oeis.org/A293017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 21, i128 n <= 36,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A293017
   EGF: exp(-x^3 * exp(x))
   a(n) = sum_{k=3}^{n} -(k*(k-1)*(k-2)) * binom(n-1, k-1) * a(n-k)   (a(0)=1) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t computed_to = 0;

    /* Ensure cache has room for index n (0‑based) */
    if (!cache) {
        cache_cap = (size_t)n + 1;
        cache = (*alloc_seq)(cache_cap);
        cache[0] = (seq_t)1;        /* a(0) = 1 */
        computed_to = 0;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;
        seq_t *new_cache = (*alloc_seq)(new_cap);
        for (idx_t i = 0; i <= computed_to; ++i)
            new_cache[i] = cache[i];
        (*free_seq)(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing entries up to n */
    for (idx_t cur = computed_to + 1; cur <= n; ++cur) {
        seq_t sum = 0;
        if (cur >= 3) {
            /* binom(cur-1, 2) = (cur-1)*(cur-2)/2 for k = 3 */
            seq_t binom = (seq_t)((idx_t)(cur - 1) * (cur - 2) / 2);
            for (idx_t k = 3; k <= cur; ++k) {
                /* coefficient = -k*(k-1)*(k-2) */
                seq_t coeff = -(seq_t)((idx_t)k * (k - 1) * (k - 2));
                /* term = coeff * binom * a(cur-k) */
                sum += coeff * binom * cache[cur - k];
                /* update binom for next k (unless we are at the last k) */
                if (k < cur) {
                    /* binom(cur-1, k) = binom(cur-1, k-1) * (cur-1 - (k-1)) / k */
                    seq_t numer = (seq_t)(cur - 1) - (seq_t)(k - 1);
                    binom = (binom * numer) / (seq_t)k;
                }
            }
        }
        cache[cur] = sum;
    }
    computed_to = n;
    return cache[n];
}
