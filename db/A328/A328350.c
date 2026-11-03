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
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

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
