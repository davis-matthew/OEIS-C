/*
 * A286185  Number of connected induced (non-null) subgraphs of the Möbius
 *          ladder graph with 2n nodes.
 * https://oeis.org/A286185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 45, u128 n <= 95,
 *            big any n
 * Helpers:   alloc_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A286185:
 *   a(n) = 6*a(n-1) - 11*a(n-2) + 4*a(n-3) + 5*a(n-4) - 2*a(n-5) - a(n-6)
 *   a(1)=3, a(2)=15, a(3)=55, a(4)=173, a(5)=511, a(6)=1451
 *   (conjectured recurrence; satisfies the OEIS data for all n ≥ 1)
 */

#include <stddef.h>
#include "../utils/alloc_cache.h"

static seq_t *cache = NULL;
static idx_t cache_cap = 0;       /* capacity of cache array (indices 0..cap-1) */
static idx_t computed_max = 0;    /* highest n with a(n) known */

/* Ensure that cache has space for indices up to 'need' (inclusive). */
static void ensure_cache(idx_t need)
{
    if (cache_cap > need) return;       /* already sufficient */
    idx_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *new_cache = alloc_cache(new_cap);
    for (idx_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise the first six values of the sequence. */
static void init_base(void)
{
    if (computed_max >= 6) return;
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)15;
    cache[3] = (seq_t)55;
    cache[4] = (seq_t)173;
    cache[5] = (seq_t)511;
    cache[6] = (seq_t)1451;
    computed_max = 6;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined below offset */
    ensure_cache(n + 1);                  /* indices 0..n must exist */
    init_base();                          /* guarantee first six terms */

    if (n <= computed_max) return cache[n];

// compute missing terms up to n using the recurrence
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t term = cache[i - 1] * (seq_t)6;
        term -= cache[i - 2] * (seq_t)11;
        term += cache[i - 3] * (seq_t)4;
        term += cache[i - 4] * (seq_t)5;
        term -= cache[i - 5] * (seq_t)2;
        term -= cache[i - 6];
        cache[i] = term;
    }
    computed_max = n;
    return cache[n];
}
