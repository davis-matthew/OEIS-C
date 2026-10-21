/*
 * A287415  a(0) = 0; a(1) = 1; a(n) = n - a(phi(a(n-1))), where phi() is the
 *          Euler totient function (A000010).
 * https://oeis.org/A287415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; a(1)=1; a(n)=n - a(phi(a(n-1)))
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/calc_phi.h"

static seq_t *cache = NULL;      /* memoization array */
static size_t cache_cap = 0;     /* current capacity of cache */
static idx_t  max_computed = 1;  /* highest index already stored */

/* Ensure the cache array can accommodate at least "needed" elements. */
static void
ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 2;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the cache with the first two known terms. */
static void
init_cache(void)
{
    ensure_cache_capacity(2);
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)1;
    max_computed = 1;
}

seq_t a(idx_t n)
{
    /* Direct return for the initial terms. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    if (!cache)
        init_cache();

    if (n > max_computed)
    {
        ensure_cache_capacity((size_t)(n + 1));

        for (idx_t k = max_computed + 1; k <= n; ++k)
        {
            seq_t prev = cache[k - 1];

            /* phi(a(k-1)) */
            seq_t phi_val = calc_phi(prev);

            /* The index required is guaranteed to be < k. */
            idx_t phi_idx = (idx_t)phi_val;

            seq_t phi_a = cache[phi_idx];
            seq_t cur   = (seq_t)k - phi_a;

            cache[k] = cur;
        }
        max_computed = n;
    }

    return cache[n];
}
