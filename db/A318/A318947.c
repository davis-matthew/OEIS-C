/*
 * A318947  Column 2 of triangle A318945.
 * https://oeis.org/A318947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 31*a(n-2) + 50*a(n-3) - 36*a(n-4) + 8*a(n-5)
 *            for n > 7, with a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=6,
 *            a(6)=26, a(7)=97
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   ensure_cache, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include "../utils/mul_small.h"
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;             /* cached sequence values        */
static size_t cache_cap = 0;            /* capacity of cache             */
static idx_t computed_max = 0;          /* highest index computed so far */
static int initialized = 0;             /* flag for initial values        */

static const seq_t init_terms[8] = {0, 0, 0, 0, 1, 6, 26, 97};

static void ensure_capacity(idx_t n)
{
    /* Ensure the cache has space for indices up to n. */
    if ((idx_t)(cache_cap) <= n) {
        ensure_cache(n, &cache, &cache_cap);
    }
}

seq_t a(idx_t n)
{
    /* Make sure the cache can hold n. */
    ensure_capacity(n);

    /* Initialise the first 8 terms only once. */
    if (!initialized) {
        memcpy(cache, init_terms, sizeof(init_terms));
        initialized = 1;
        computed_max = 7;
    }

    /* If n is within the pre‑computed range, return it. */
    if (n <= 7)
        return cache[n];

    /* Compute missing terms up to n. */
    if (computed_max < n) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t t1 = mul_small(cache[i - 1], 9);
            seq_t t2 = mul_small(cache[i - 2], 31);
            seq_t t3 = mul_small(cache[i - 3], 50);
            seq_t t4 = mul_small(cache[i - 4], 36);
            seq_t t5 = mul_small(cache[i - 5], 8);
            cache[i] = t1 - t2 + t3 - t4 + t5;
        }
        computed_max = n;
    }

    return cache[n];
}
