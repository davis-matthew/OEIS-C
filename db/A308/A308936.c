/*
 * A308936  a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)), with a(n) =
 *          ceiling(4*n/5) for n <= 9.
 * https://oeis.org/A308936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 376, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)),
 * initial values a(n) = ceil(4*n/5) for 1 ≤ n ≤ 9.
 * Offset: 1  (a(1) is the first term)
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* static cache: cache[0] unused; cache[i] holds a(i) */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* allocated length (including the unused 0) */
static idx_t computed_max = 0;  /* largest n already computed */

/* increase cache capacity so that indices up to n can be safely accessed */
static void ensure_cache(idx_t n)
{
    if (cache_size <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;          /* include index 0 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                      /* out of memory */
        cache = tmp;
        cache_size = new_cap;
    }
}

/* initialise base values a(1)…a(9) */
static void initialise_base(void)
{
    ensure_cache(9);
    for (idx_t i = 1; i <= 9; ++i) {
        /* ceil(4*i/5) == (4*i + 4)/5 for positive integers */
        cache[i] = ((seq_t)4 * (seq_t)i + 4) / 5;
    }
    computed_max = 9;
}

seq_t a(idx_t n)
{
    if (n < 1)                  /* safety for out‑of‑range indices */
        return 0;

    if (cache_size == 0)        /* first call: initialise the cache */
        initialise_base();

    if (n > computed_max) {
        /* enlarge cache to accommodate the new indices */
        ensure_cache(n);

        /* compute all missing terms up to n */
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            idx_t idx1 = i - (idx_t)cache[i - 1];
            idx_t idx2 = i - (idx_t)cache[i - 2];
            idx_t idx3 = i - (idx_t)cache[i - 3];
            /* indices are guaranteed to be ≥1 because a(k) < k for k≥1 */
            cache[i] = cache[idx1] + cache[idx2] + cache[idx3];
        }
        computed_max = n;
    }

    return cache[n];
}
