/*
 * A242745  Least k > 0 such that (k!-n!)/(k-n) is an integer.
 * https://oeis.org/A242745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 0 such that (k! - n!)/(k - n) is an integer
 * Range:     u8 n <= 229, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 107 terms through n = 960 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return a single value for a given n. */
static seq_t a_single(idx_t n)
{
    if (n == 1)  /* special case */
        return (seq_t)2;

    /* For n > 1, a(n) <= n-1 (k cannot equal n). */
    for (idx_t k = 1; k < n; ++k) {
        idx_t d = n - k;                 /* positive divisor */
        seq_t rem_k = 1;                 /* k! mod d */
        for (idx_t i = 2; i <= k; ++i)
            rem_k = (rem_k * (seq_t)i) % (seq_t)d;

        seq_t rem_n = rem_k;             /* n! starts as k! */
        for (idx_t i = k + 1; i <= n; ++i)
            rem_n = (rem_n * (seq_t)i) % (seq_t)d;

        seq_t diff = (rem_k + (seq_t)d - rem_n) % (seq_t)d;
        if (diff == 0)
            return (seq_t)k;
    }

    /* The sequence definition guarantees that a(n) is found for n > 1. */
    return (seq_t)(n - 1);   /* fallback – never reached for valid n */
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_n_cached = 0;   /* largest n for which cache[n] is set */

    if (n == 0) return 0;   /* undefined, return 0 */

    if (n > max_n_cached) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return 0;   /* allocation failure */
        cache = tmp;
        for (idx_t cur = max_n_cached + 1; cur <= n; ++cur) {
            cache[cur] = a_single(cur);
        }
        max_n_cached = n;
    }
    return cache[n];
}
