/*
 * A348956  a(0) = 1; a(n) = Sum_{d|n, d < n} (-1)^(n/d + 1) * a(d - 1).
 * https://oeis.org/A348956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(0) = 1; a(n) = Sum_{d|n, d<n} (-1)^(n/d + 1) * a(d-1)
 * Range:     i8 n <= 20000, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 97 terms through n = 9453 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* allocated capacity */
    static idx_t computed = 0;        /* highest index computed so far */

    /* Ensure the cache exists and can address index n */
    if (cache == NULL) {
        /* Allocate at least (n+1) slots */
        size_t need = (size_t)(n + 1);
        cache = seq_alloc(need);
        cap   = need;
        cache[0] = 1;                 /* a(0) = 1 */
        computed = 0;
    } else if ((size_t)n + 1 > cap) {
        /* Grow the cache geometrically to amortize re‑allocations */
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        cache = seq_realloc(cache, cap, new_cap);
        cap   = new_cap;
    }

    /* If the requested n has already been computed, just return it */
    if (n <= computed)
        return cache[n];

    /* Compute all values from (computed+1) up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum = 0;

        /* Enumerate all proper divisors d of i */
        for (idx_t j = 1; j * j <= i; ++j) {
            if (i % j == 0) {
                /* First divisor d1 = j */
                idx_t d1 = j;
                if (d1 < i) {
                    seq_t sgn = ((i / d1) & 1U) ? 1 : -1;
                    sum += sgn * cache[d1 - 1];
                }

                /* Second divisor d2 = i/j, skip if equal to d1 or i itself */
                idx_t d2 = i / j;
                if (d2 != d1 && d2 < i) {
                    seq_t sgn = ((i / d2) & 1U) ? 1 : -1;
                    sum += sgn * cache[d2 - 1];
                }
            }
        }
        cache[i] = sum;
    }

    /* Update the most computed index */
    computed = n;
    return cache[n];
}
