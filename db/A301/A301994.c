/*
 * A301994  Number of n X 3 0..1 arrays with every element equal to 1, 2 or 4
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A301994
   Empirical recurrence:
   a(n) = 5*a(n-1) - 7*a(n-2) + 4*a(n-3)   for n > 3
   a(1) = 1, a(2) = 2, a(3) = 5
   The implementation caches computed values so that calls in any
   order are fast.  It works for ordinary unsigned integer types as
   well as for a user‑defined arbitrary‑precision class.  In
   C++ the cache uses std::vector to ensure proper construction of
   objects; in C the cache uses malloc/realloc with explicit casts. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n ≥ 1 */

#ifdef __cplusplus
    /* C++ implementation – use std::vector to store the cache */
    static std::vector<seq_t> cache;
    static idx_t computed_max = 0;    /* highest n already computed */

    if (!computed_max) {
        cache.resize(3);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        computed_max = 3;
    }

    if (n > cache.size()) {
        cache.resize(n);
        for (idx_t i = computed_max; i < n; ++i) {
            /* recurrence: a(i+1) = 5*a(i) - 7*a(i-1) + 4*a(i-2) */
            seq_t term = 5 * cache[i - 1];
            term -= 7 * cache[i - 2];
            term += 4 * cache[i - 3];
            cache[i] = term;
        }
        computed_max = n;
    }

    return cache[n - 1];
#else
    /* C implementation – use malloc/realloc for the cache */
    static seq_t *cache = NULL;     /* 0‑based array: cache[i] = a(i+1) */
    static size_t cap = 0;          /* allocated capacity */
    static idx_t computed_max = 0;  /* highest n already computed */

    /* enlarge the cache if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* initialise the first three terms if not yet done */
    if (computed_max < 3) {
        if (cap < 3) {
            size_t new_cap = 3;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cap = new_cap;
        }
        if (computed_max == 0) cache[0] = (seq_t)1;
        if (computed_max <= 1) cache[1] = (seq_t)2;
        if (computed_max <= 2) cache[2] = (seq_t)5;
        computed_max = 3;
    }

    /* compute terms up to n */
    while (computed_max < n) {
        /* recurrence: a(i) = 5*a(i-1) - 7*a(i-2) + 4*a(i-3)   for i > 3 */
        idx_t i = computed_max + 1;            /* i is 1‑based index of the new term */
        seq_t term = 5 * cache[i - 2];         /* a(i-1) */
        term -= 7 * cache[i - 3];              /* a(i-2) */
        term += 4 * cache[i - 4];              /* a(i-3) */
        cache[i - 1] = term;
        ++computed_max;
    }

    return cache[n - 1];
#endif
}
