/*
 * A277872  Number of ways of writing n as a sum of powers of 4, each power
 *          being used at most four times.
 * https://oeis.org/A277872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(4n)=a(n)+a(n-1); a(4n+r)=a(n) for r=1,2,3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t   *cache = NULL;    /* holds computed terms */
static unsigned char *computed = NULL; /* marks entries that are set */
static size_t   cache_cap = 0;   /* size of cache arrays */

/* ensure the cache can hold index n (0‑based) */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < needed) new_cap <<= 1;

    seq_t  *new_cache   = (seq_t  *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);
    unsigned char *new_comp = (unsigned char *)realloc(computed,
                                                      new_cap * sizeof(unsigned char));
    if (!new_comp) exit(1);

    /* initialise new cache slots to zero (safe for all seq_t types) */
    for (size_t i = cache_cap; i < new_cap; ++i) new_comp[i] = 0;

    cache    = new_cache;
    computed = new_comp;
    cache_cap = new_cap;

    /* initialise a(0) only once */
    if (!computed[0]) {
        cache[0] = (seq_t)1;
        computed[0] = 1;
    }
}

/* compute a(n) using memoisation */
static seq_t compute_a(idx_t n)
{
    if (computed[n]) return cache[n];

    seq_t result;
    if (n == 0) {
        result = (seq_t)1;
    } else if ((n & 3) == 0) {               /* n divisible by 4 */
        idx_t k = n >> 2;
        seq_t a1 = compute_a(k);
        seq_t a2 = compute_a(k - 1);
        result = a1 + a2;
    } else {                                 /* n ≡ 1,2,3 (mod 4) */
        result = compute_a(n >> 2);
    }

    cache[n] = result;
    computed[n] = 1;
    return result;
}

/* external function required by the build system */
extern seq_t a(idx_t n)
{
    ensure_cache(n);
    return compute_a(n);
}
