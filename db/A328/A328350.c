/*
 * A328350  Let S be any integer in the range 6 <= S <= 24. Sequence has the
 *          property that a(n)*S is the sum of all positive integers whose
 *          decimal expansion has <= n digits and contains at most three
 *          distinct nonzero digits d1, d2, d3 such that d1+d2+d3 = S.
 * https://oeis.org/A328350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 34*a(n-1) - 123*a(n-2) + 90*a(n-3) * Initial values:
 *            a(0)=0, a(1)=1, a(2)=34 * * The function is memoised; it can be
 *            called in any order for n ≥ 0. * It uses the provided utilities
 *            `grow_seq` and `mul_small`
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   grow_seq, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */
#include "../utils/grow_seq.h"
#include "../utils/mul_small.h"

static seq_t *cache = NULL;      /* cached a(n) values */
static idx_t  cache_size = 0;    /* number of values currently stored (0‑based) */
static size_t cache_cap  = 0;    /* capacity of the cache array (elements) */

/* Utility: initialise base values when needed */
static void init_base(void)
{
    if (cache_size == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)34;
        cache_size = 3;
    } else if (cache_size < 3) {
        if (cache_size <= 1) cache[1] = (seq_t)1;
        if (cache_size <= 2) cache[2] = (seq_t)34;
        cache_size = 3;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Return cached value if available */
    if (n < cache_size)
        return cache[n];

    /* Ensure the cache array is large enough for n */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed)
        grow_seq(&cache, &cache_cap, needed);

    /* Initialise base values if not present */
    init_base();

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t term = mul_small(cache[i - 1], 34);              /* 34*a(i-1) */
        term -= mul_small(cache[i - 2], 123);                 /* -123*a(i-2) */
        term += mul_small(cache[i - 3], 90);                  /* +90*a(i-3) */
        cache[i] = term;
    }

    /* Update logical size of cached portion */
    if (cache_size < (n + 1))
        cache_size = (idx_t)needed;

    return cache[n];
}
