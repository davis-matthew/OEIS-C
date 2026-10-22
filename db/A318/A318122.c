/*
 * A318122  a(0) = a(1) = 1; a(n) = Sum_{k=0..n-2} gcd(a(k), a(n-k-2)).
 * https://oeis.org/A318122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(1) = 1; a(n) = Sum_{k=0}^{n-2} gcd(a(k), a(n-k-2))
 *            for n ≥ 2
 * Range:     u8 n <= 45, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   This implementation memoises the values in a static array and expands it
 *   as needed.  It guarantees correctness for any order of calls, and runs
 *   in O(n²) time, suitable for n up to about 10000 within the time limit. */
#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;          /* cache of computed a(n) */
static idx_t cache_cap = 0;          /* allocated size of 'cache' */
static idx_t computed_up_to = 0;     /* highest index that has been computed */

/* Ensure the cache has at least 'need+1' slots (to store a[0..need]) */
static void ensure_cache_capacity(idx_t need)
{
    if (cache_cap > 0 && cache_cap >= need + 1)
        return;                     /* already enough */

    idx_t new_cap = cache_cap ? cache_cap : 2;
    while (new_cap <= need) new_cap <<= 1;  /* grow until big enough */

    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    if (!new_cache) exit(1);                /* out of memory */

    if (cache) {
        for (idx_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    /* initialise new slots to 0 (will be computed as needed) */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n) as defined by OEIS A318122. */
seq_t a(idx_t n)
{
    if (n <= 1)          /* a(0) = a(1) = 1 */
        return 1;

    /* Ensure at least n+1 elements are allocated. */
    ensure_cache_capacity(n);

    /* On first use, initialise the base cases if not already set. */
    if (cache[0] == 0 && cache[1] == 0) {
        cache[0] = cache[1] = 1;
        computed_up_to = 1;
    }

    /* Compute missing values up to n. */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* i ≥ 2, so i-2 is valid */
        idx_t bound = i - 2;
        for (idx_t k = 0; k <= bound; ++k) {
            sum += gcd(cache[k], cache[i - k - 2]);
        }
        cache[i] = sum;
    }
    computed_up_to = n;
    return cache[n];
}
