/*
 * A252076  Numbers n such that the sum of the heptagonal numbers H(n) and
 *          H(n+1) is equal to the hexagonal number X(m) for some m.
 * https://oeis.org/A252076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1443*a(n-1) - 1443*a(n-2) + a(n-3); a(1) = 0, a(2) = 486,
 *            a(3) = 701100
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* storage for cached values */
static size_t  cap    = 0;    /* capacity of cache (indices 0..cap-1) */
static size_t  computed = 0;  /* highest index computed so far */

/* Ensure that cache[n] is computed for all 1 ≤ i ≤ n. */
static void ensure_cache(idx_t n)
{
    if (n <= (idx_t)computed)
        return;  /* already computed up to at least n */

    /* Expand cache if necessary */
    size_t need = (size_t)n + 1;                  /* accommodate indices 0 .. n */
    if (need > cap) {
        size_t newcap = cap ? cap << 1 : 4;       /* grow, at least 4 slots */
        if (newcap < need)
            newcap = need;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            abort();                              /* out of memory */
        cache = tmp;
        cap = newcap;
    }

    /* Initialise base cases on first use */
    if (computed == 0) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)486;
        cache[3] = (seq_t)701100;
        computed = 3;
    }

    /* Compute missing terms */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1] * (seq_t)1443
                 - cache[i-2] * (seq_t)1443
                 + cache[i-3];
    }

    if (n > computed)
        computed = (size_t)n;
}

/* Public function as required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for indices < 1 */

    ensure_cache(n);
    return cache[(size_t)n];
}
