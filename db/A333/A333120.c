/*
 * A333120  a(1) = 1, a(n+1) = Sum_{d|n} a(d)^2.
 * https://oeis.org/A333120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n ≥ 1, a(n+1) = Σ_{d|n} a(d)^2
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Memoisation cache for the sequence */
static seq_t *cache      = NULL;   /* a(0) unused, a[i] holds a(i) */
static bool *computed    = NULL;   /* true if a[i] has been computed */
static idx_t cache_size  = 0;      /* maximum index currently stored (inclusive) */

/* Ensure the cache can store values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)   /* previous allocation already large enough */
        return;

    /* allocate new storage */
    idx_t new_size  = n + 1;           /* indices 0 .. n inclusive */
    seq_t  *new_c   = (seq_t *)malloc((new_size + 1) * sizeof(seq_t));
    bool   *new_cf  = (bool  *)malloc((new_size + 1) * sizeof(bool));

    if (!new_c || !new_cf) {
        /* allocation failed – leave previous cache unchanged */
        free(new_c);
        free(new_cf);
        return;
    }

    /* initialise new entries to 0/false */
    for (idx_t i = 0; i <= new_size; ++i) {
        new_c[i]  = 0;
        new_cf[i] = false;
    }

    /* copy existing computed values */
    if (cache) {
        for (idx_t i = 0; i <= cache_size; ++i) {
            new_c[i]  = cache[i];
            new_cf[i] = computed[i];
        }
        free(cache);
        free(computed);
    }

    cache      = new_c;
    computed   = new_cf;
    cache_size = new_size;
}

/* Return the n-th term of the sequence (n ≥ 1).  Calls may be in any order. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined behavior for n < 1 is ignored */

    ensure_cache(n);

    /* initialise a(1) once */
    if (!computed[1]) {
        computed[1] = true;
        cache[1]    = 1;
    }

    if (n == 1)
        return cache[1];

    /* compute all missing terms up to n */
    for (idx_t i = 2; i <= n; ++i) {
        if (computed[i])
            continue;

        /* compute a(i) by summing a(d)^2 over all divisors d of (i-1) */
        idx_t m   = i - 1;
        seq_t sum = 0;

        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                seq_t v1 = cache[d];
                sum += v1 * v1;
                idx_t d2 = m / d;
                if (d2 != d) {
                    seq_t v2 = cache[d2];
                    sum += v2 * v2;
                }
            }
        }

        cache[i]     = sum;
        computed[i]  = true;
    }

    return cache[n];
}
