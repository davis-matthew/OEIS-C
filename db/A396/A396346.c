/*
 * A396346  a(0) = 0; for n > 0, a(n) = a(n-1) - n if nonnegative and not
 *          already in the sequence, otherwise a(n) = a(n-1) + n - 1.
 * https://oeis.org/A396346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0) = 0; * For n > 0: * If a(n-1) >= n and (a(n-1) - n) is not
 *            already in the sequence, * a(n) = a(n-1) - n; * else * a(n) =
 *            a(n-1) + n - 1 * * This implementation memoises the sequence in
 *            a dynamic array. * Each new term is computed only once; later
 *            calls reuse the cached * value. Membership te...
 * Range:     u8 n <= 122, u16 n <= 8969, u32 n <= 8969, u64 n <= 8969,
 *            u128 n <= 8969, big any n
 * Helpers:   seq_contains
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_contains.h"

/* External types supplied by the build system. */
typedef uint64_t idx_t;   /* Index type */

static seq_t *seq_cache = NULL;   /* Cached sequence values */
static idx_t  seq_len  = 0;        /* Number of values stored (len >= 1) */
static size_t seq_cap  = 0;        /* Allocated capacity */

/* Grow the cache so that it contains values up to index n (inclusive). */
static void seq_ensure(idx_t n)
{
    if (n < seq_len)
        return;                     /* Already computed */

    /* Initialise if this is the first call. */
    if (seq_cache == NULL) {
        seq_cap = 16;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len  = 0;
    }

    /* Append the first element a(0)=0 if the cache is empty. */
    if (seq_len == 0) {
        seq_cache[0] = 0;
        seq_len = 1;
    }

    /* Expand capacity if necessary. */
    while (seq_cap <= (size_t)(n))
    {
        seq_cap *= 2;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }

    /* Compute new terms.  i is the index being filled. */
    for (idx_t i = seq_len; i <= n; ++i)
    {
        seq_t prev = seq_cache[i - 1];
        seq_t cand = prev - (seq_t)i;   /* Tentative candidate */

        if (prev >= (seq_t)i && !seq_contains(seq_cache, i, cand))
            seq_cache[i] = cand;        /* Use the candidate */
        else
            seq_cache[i] = prev + (seq_t)i - 1;   /* Fallback value */
    }

    seq_len = n + 1;
}

/* Public interface required by the test harness. */
seq_t a(idx_t n)
{
    seq_ensure(n);
    return seq_cache[n];
}
