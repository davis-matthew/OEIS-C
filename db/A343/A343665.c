/*
 * A343665  Number of partitions of an n-set without blocks of size 5.
 * https://oeis.org/A343665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0}^{⌊n/5⌋} (-1)^k * Bell(n-5k) / ((n-5k)! * k!
 *            * (5!)^k) Derived recurrence: a(0)=1; for n≥0: a(n+1) =
 *            Σ_{i=0}^{n} binom(n,i) * a(i) - (n≥4 ? binom(n,4) * a(n-4) : 0)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom.h"

static seq_t *cache = NULL;    /* storage for computed a(n) values */
static size_t cache_cap = 0;   /* current capacity of the cache array */
static idx_t computed_max = 0; /* highest n for which a(n) is stored */

/* Ensure that cache contains a(0)…a(n).  All new values are computed
   using the recurrence above. */
static void ensure_cache(idx_t n)
{
    /* initialise cache if needed */
    if (cache == NULL) {
        cache_cap = 8;                     /* start with a small capacity */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return;                 /* allocation failed – abort */
        cache[0] = (seq_t)1;                /* a(0) = 1 */
        computed_max = 0;
    }

    /* extend cache up to n */
    while (computed_max < n) {
        idx_t next = computed_max + 1;                  /* we will compute a(next) */
        idx_t prev = next - 1;                          /* index in the recurrence */

        /* compute S = Σ_{i=0}^{prev} binom(prev,i)*a(i) */
        seq_t sum = (seq_t)0;
        for (size_t i = 0; i <= (size_t)prev; ++i) {
            seq_t bin = binom(prev, (idx_t)i);
            sum += cache[(size_t)i] * bin;
        }

        /* subtract the forbidden block of size 5 */
        if (prev >= 4) {
            seq_t bin4 = binom(prev, (idx_t)4);
            sum -= cache[(size_t)(prev - 4)] * bin4;
        }

        /* make sure there is space for the new entry */
        if ((size_t)next + 1 > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < (size_t)next + 1) new_cap = (size_t)next + 1;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) return;             /* allocation failed */
            cache = new_cache;
            cache_cap = new_cap;
        }

        cache[(size_t)next] = sum;
        computed_max = next;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_cache(n);
    return cache[(size_t)n];
}
