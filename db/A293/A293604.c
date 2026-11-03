/*
 * A293604  Expansion of e.g.f.: exp(x * (1 - x)).
 * https://oeis.org/A293604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=1, a(n)=a(n-1)-2*(n-1)*a(n-2) (n>1)
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 17, i64 n <= 28, i128 n <= 48,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 683 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper: free an array allocated by alloc_seq. */
static void free_seq(seq_t *p)
{
    mem_free(p);
}

static seq_t *seq_cache = NULL;
static size_t seq_cache_cap = 0;
static idx_t seq_cache_last = 1;

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* Allocate or enlarge cache as needed */
    if (seq_cache == NULL) {
        seq_cache_cap = (size_t)(n + 1);
        seq_cache = alloc_seq(seq_cache_cap);
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)1;
        seq_cache_last = 1;
    } else if (seq_cache_cap <= (size_t)n) {
        seq_t *old = seq_cache;
        seq_cache_cap = (size_t)(n + 1);
        seq_cache = alloc_seq(seq_cache_cap);
        for (idx_t i = 0; i <= seq_cache_last; ++i)
            seq_cache[i] = old[i];
        free_seq(old);
    }

    /* Compute all missing terms up to n */
    for (idx_t i = seq_cache_last + 1; i <= n; ++i) {
        seq_t prev1 = seq_cache[i - 1];
        seq_t prev2 = seq_cache[i - 2];
        seq_t term = ((seq_t)2 * (seq_t)(i - 1) * prev2);
        seq_cache[i] = prev1 - term;
    }
    seq_cache_last = n;

    return seq_cache[n];
}
