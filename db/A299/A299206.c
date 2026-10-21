/*
 * A299206  Ramanujan's tau function (or tau numbers (A000594)) for 5^n.
 * https://oeis.org/A299206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = tau(5^n). Recurrence: a(0) = 1, a(1) = 4830; a(n) =
 *            4830*a(n-1) - 48828125*a(n-2) for n ≥ 2 (tau is Ramanujan’s tau
 *            function)
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 2, i64 n <= 4, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;          /* cache array */
static size_t cache_cap = 0;         /* cache capacity */
static idx_t cache_len = 0;          /* number of values stored */

static const seq_t BASE_TAU5    = 4830;          /* tau(5)   */
static const seq_t COEFF_P11    = 48828125;      /* 5^11     */

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 2;                          /* minimum capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed – should not happen in tests */
        cache[0] = 1;
        cache[1] = BASE_TAU5;
        cache_len = 2;
    }

    /* Extend cache if needed */
    if (n >= cache_len) {
        /* Ensure enough capacity */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failed – should not happen in tests */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute values up to n */
        for (idx_t k = cache_len; k <= n; ++k) {
            cache[k] = BASE_TAU5 * cache[k-1] - COEFF_P11 * cache[k-2];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
