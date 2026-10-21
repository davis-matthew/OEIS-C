/*
 * A290273  Number of minimal dominating sets in the n-pan graph.
 * https://oeis.org/A290273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-4) - a(n-6) Initial values: a(1)=2,
 *            a(2)=2, a(3)=3, a(4)=5, a(5)=7, a(6)=8
 * Range:     u8 n <= 15, u16 n <= 32, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[6] = {2, 2, 3, 5, 7, 8};

static seq_t *cache = NULL;          /* cache of computed values */
static size_t cache_cap = 0;         /* allocated capacity  (≥ max_computed) */
static idx_t max_computed = 0;       /* highest index whose value is stored */

seq_t a(idx_t n)
{
    if (n <= 0) {
        return 0;                     /* below the offset – behaviour is arbitrary */
    }

    if (n <= 6) {
        return base_vals[n - 1];       /* direct lookup for the first six terms */
    }

    /* initialise cache on first use */
    if (cache == NULL) {
        size_t init_cap = 6;
        if ((size_t)n > init_cap)
            init_cap = (size_t)n;

        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache)
            return 0;                  /* out‑of‑memory is an unlikely failure */

        /* copy the base values */
        for (idx_t i = 0; i < 6 && (size_t)i < init_cap; ++i)
            cache[i] = base_vals[i];

        /* zero the rest */
        for (size_t i = 6; i < init_cap; ++i)
            cache[i] = 0;

        cache_cap     = init_cap;
        max_computed  = 6;
    }

    /* enlarge the cache if necessary */
    if ((size_t)n > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp)
            return 0;                  /* out‑of‑memory */

        cache   = tmp;
        cache_cap = (size_t)n;

        /* initialise the new slots */
        for (size_t i = max_computed; i < cache_cap; ++i)
            cache[i] = 0;
    }

    /* compute any missing terms up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val;
            if (i <= 6) {
                val = base_vals[i - 1];
            } else {
                val = cache[i - 3] + cache[i - 4] + cache[i - 5];
                val -= cache[i - 7];      /* a(n-2)+a(n-3)+a(n-4)-a(n-6) */
            }
            cache[i - 1] = val;
        }
        max_computed = n;
    }

    return cache[n - 1];
}
