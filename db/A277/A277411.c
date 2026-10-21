/*
 * A277411  Column 1 of triangle A277410.
 * https://oeis.org/A277411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A277411 (Column 1 of triangle A277410)
   Formula: a(n) = (8*(2^n-1)-n*(n+7))/2
   Recurrence for n>4:
      a(1)=0, a(2)=3, a(3)=13, a(4)=38
      a(n)=5*a(n-1)-9*a(n-2)+7*a(n-3)-2*a(n-4) */

#include <stddef.h>
#include <stdlib.h>

/* Internal cache for memoisation */
static seq_t *cache = NULL;      /* pointer to computed values */
static idx_t  max_cached = 0;    /* maximum n stored in cache */
static size_t cap = 0;           /* allocated capacity of cache */

/* Ensure cache can hold indices up to n (inclusive). */
static void
ensure_capacity(idx_t n)
{
    if (cap >= (size_t)(n + 1))
        return;

    size_t new_cap = cap ? cap : 16;
    while (new_cap < (size_t)(n + 1))
        new_cap <<= 1;          /* double until sufficient */

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                /* allocation failure -> abort */

    cache = new_cache;
    cap   = new_cap;
}

/* Return the n-th term of the sequence (a(1) = first term). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* offset is 1; behaviour for n<1 unspecified */

    if (n > max_cached) {
        /* Expand cache if necessary. */
        ensure_capacity(n);

        /* Initialise base case if this is the first call. */
        if (max_cached == 0) {
            cache[0] = 0;      /* value for a(0) is unused but useful */
        }

        /* Compute all missing terms up to n. */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = 0;
            } else if (i == 2) {
                val = 3;
            } else if (i == 3) {
                val = 13;
            } else if (i == 4) {
                val = 38;
            } else {
                /* recurrence: 5*a(i-1) - 9*a(i-2) + 7*a(i-3) - 2*a(i-4) */
                seq_t a1 = cache[(size_t)(i - 1)];
                seq_t a2 = cache[(size_t)(i - 2)];
                seq_t a3 = cache[(size_t)(i - 3)];
                seq_t a4 = cache[(size_t)(i - 4)];

                val = 5 * a1;
                val -= 9 * a2;
                val += 7 * a3;
                val -= 2 * a4;
            }
            cache[(size_t)i] = val;
        }
        max_cached = n;
    }

    return cache[(size_t)n];
}
