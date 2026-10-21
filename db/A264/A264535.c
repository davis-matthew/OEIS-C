/*
 * A264535  Number of (2+1) X (n+1) arrays of permutations of 0..n*3+2 with
 *          each element having directed index change -2,-2 -1,0 0,1 or 1,0.
 * https://oeis.org/A264535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 39, u64 n <= 77, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A264535: number of (2+1)×(n+1) arrays of permutations of 0..3n+2
   with each element having directed index change -2,-2 -1,0 0,1 or 1,0.
   Recurrence: a(n) = 4*a(n-3) + 2*a(n-4) + 2*a(n-5) - 2*a(n-9) - a(n-10)
   Initial values: a(1)=0, a(2)=4, a(3)=2, a(4)=1, a(5)=16, a(6)=16,
   a(7)=16, a(8)=68, a(9)=97, a(10)=128. */
#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build system. */

static const seq_t base_vals[11] = {
    0ULL, /* base_vals[0] unused */
    0ULL, /* a(1) */
    4ULL, /* a(2) */
    2ULL, /* a(3) */
    1ULL, /* a(4) */
    16ULL,/* a(5) */
    16ULL,/* a(6) */
    16ULL,/* a(7) */
    68ULL,/* a(8) */
    97ULL,/* a(9) */
    128ULL/* a(10) */
};

seq_t a(idx_t n)
{
    /* sequences are defined only for n >= 1 */
    if (n < 1U) return 0;

    /* quick answer for the first ten terms */
    if (n <= 10U) return base_vals[n];

    /* static memoization cache */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* size of the allocated array (indices 0..cap-1) */
    static idx_t   filled = 0;  /* highest index for which cache[i] is valid */

    /* ensure the cache is large enough */
    if ((size_t)n + 1U > cap) {
        size_t new_cap = n + 1U;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure – unlikely */
        cache = tmp;
        /* initialise new entries to zero (realloc does not zero the new area) */
        for (size_t i = cap; i < new_cap; ++i) cache[i] = 0;
        cap = new_cap;
    }

    /* fill in the initial ten values if not already done */
    if (filled < 10U) {
        idx_t start = filled + 1U;
        if (start == 0U) start = 1U; /* never happen, but keep compiler happy */
        for (idx_t i = start; i <= 10U; ++i) {
            cache[i] = base_vals[i];
        }
        cache[0] = 0;          /* unused sentinel to keep indices tidy */
        filled = 10U;
    }

    /* compute any missing terms up to n */
    for (idx_t i = filled + 1U; i <= n; ++i) {
        seq_t val = (seq_t)4 * cache[i - 3];
        val += (seq_t)2 * cache[i - 4];
        val += (seq_t)2 * cache[i - 5];
        val -= (seq_t)2 * cache[i - 9];
        val -= cache[i - 10];
        cache[i] = val;
    }
    filled = n;

    return cache[n];
}
