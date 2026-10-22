/*
 * A333790  Smallest path sum when iterating from n to 1 with nondeterministic
 *          map k -> k - k/p, where p is any prime factor of k.
 * https://oeis.org/A333790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + min{ a(n - n/p) | p prime and p | n }
 * Range:     u8 n <= 78, u16 n <= 9928, u32 n <= 9928, u64 n <= 9928,
 *            u128 n <= 9928, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence base: a(1) = 1 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

/* Memoisation table for a(n); 0 means “not yet computed”. */
static seq_t *cache      = NULL;  /* one element per n, index 0 unused */
static size_t cache_cap  = 0;     /* size of cache == max index + 1 */

/* Smallest prime factor array for 1 … n. */
static idx_t *spf        = NULL;
static size_t spf_cap    = 0;     /* size of spf array == max value + 1 */

/* Ensure spf array covers at least n. */
static void ensure_spf(idx_t n)
{
    if (n < spf_cap) return;
    free(spf);                         /* free old array if any */
    build_spf(n, &spf);                /* build new array 0 … n   */
    spf_cap = n + 1;
}

/* Ensure cache covers at least n, initialising new slots to 0. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);           /* out of memory */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = (seq_t)0;
    cache     = new_cache;
    cache_cap = new_cap;
}

/* Recursive computation with memoisation. */
static seq_t compute_a(idx_t n)
{
    ensure_cache(n);
    if (cache[n] != (seq_t)0)               /* already computed */
        return cache[n];

    if (n == 1) {                           /* base case */
        cache[1] = (seq_t)1;
        return cache[1];
    }

    ensure_spf(n);

    idx_t temp = n;
    idx_t last_p = 0;
    seq_t best = (seq_t)-1;                 /* largest unsigned (invalid) */

    /* Enumerate distinct prime factors via the SPF array. */
    while (temp > 1) {
        idx_t p = spf[temp];
        if (p != last_p) {
            idx_t nxt = n - n / p;
            seq_t val = compute_a(nxt);
            if (best == (seq_t)-1 || val < best)
                best = val;
            last_p = p;
        }
        temp /= p;
    }

    cache[n] = best + (seq_t)n;
    return cache[n];
}

/* External function required by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for 0, return 0 */
    return compute_a(n);
}
