/*
 * A328808  Constant term in the expansion of (3 + x + y + z + 1/x + 1/y + 1/z
 *          + x*y + y*z + z*x + 1/(x*y) + 1/(y*z) + 1/(z*x) + x*y*z +
 *          1/(x*y*z))^n.
 * https://oeis.org/A328808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A328808
 * Formula: a(n) = Sum_{i=0..n} binomial(n,i) * Sum_{j=0..i} binomial(i,j)^4
 * Recurrence (used in this implementation):
 *   n^3*a(n) = (2n-1)*(8n^2-8n+3)*a(n-1)
 *             + (n-1)*(22n^2-44n+13)*a(n-2)
 *             - 44*(n-2)*(n-1)*(2n-3)*a(n-3)
 *             + 51*(n-3)*(n-2)*(n-1)*a(n-4)
 * Initial values: a(0)=1, a(1)=3, a(2)=23, a(3)=225
 *
 * The function memoises computed values to allow arbitrary call order.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* memoisation array */
static size_t  cache_cap = 0;    /* allocated capacity   */
static idx_t   cached_max = 0;   /* highest index computed */

/* Grow the cache so it can hold indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > (size_t)n)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)n)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return;
    for (size_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];
    cache = new_cache;
    cache_cap = new_cap;
}

/* If this is the first call, initialise the cache with the first four terms */
static void initialise_cache(void)
{
    if (cache)
        return;
    cache_cap = 16;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache)
        return;
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)23;
    cache[3] = (seq_t)225;
    cached_max = 3;
}

seq_t a(idx_t n)
{
    initialise_cache();

    if (n <= cached_max)
        return cache[n];

    ensure_capacity(n);

    /* compute successive values using the recurrence */
    for (idx_t k = cached_max + 1; k <= n; ++k)
    {
        /* for k >= 4 the recurrence applies. */
        if (k < 4) {
            /* should already be initialised */
            continue;
        }

        /* Decide whether to use 128‑bit intermediate (safety for 64‑bit builds) */
        if (sizeof(seq_t) <= 8) {
            /* 128‑bit intermediate path */
            unsigned __int128 c1 = (unsigned __int128)(2*k - 1);
            unsigned __int128 c2 = (unsigned __int128)(8*k*k - 8*k + 3);
            unsigned __int128 c3 = (unsigned __int128)(k - 1);
            unsigned __int128 c4 = (unsigned __int128)(22*k*k - 44*k + 13);
            unsigned __int128 c5 = (unsigned __int128)44;
            unsigned __int128 c6 = (unsigned __int128)(k - 2);
            unsigned __int128 c7 = (unsigned __int128)(k - 1);
            unsigned __int128 c8 = (unsigned __int128)(2*k - 3);
            unsigned __int128 c9 = (unsigned __int128)51;
            unsigned __int128 c10 = (unsigned __int128)(k - 3);
            unsigned __int128 c11 = (unsigned __int128)(k - 2);
            unsigned __int128 c12 = (unsigned __int128)(k - 1);

            unsigned __int128 term1 = c1 * c2 * (unsigned __int128)cache[k-1];
            unsigned __int128 term2 = c3 * c4 * (unsigned __int128)cache[k-2];
            unsigned __int128 term3 = c5 * c6 * c7 * c8 * (unsigned __int128)cache[k-3];
            unsigned __int128 term4 = c9 * c10 * c11 * c12 * (unsigned __int128)cache[k-4];

            unsigned __int128 num = term1 + term2 + term4;
            num -= term3;

            unsigned __int128 n_cube = (unsigned __int128)k * (unsigned __int128)k * (unsigned __int128)k;
            cache[k] = (seq_t)(num / n_cube);
        } else {
            /* big‑int path – all operations on seq_t */
            seq_t c1 = (seq_t)(2*k - 1);
            seq_t c2 = (seq_t)(8*k*k - 8*k + 3);
            seq_t c3 = (seq_t)(k - 1);
            seq_t c4 = (seq_t)(22*k*k - 44*k + 13);
            seq_t c5 = (seq_t)44;
            seq_t c6 = (seq_t)(k - 2);
            seq_t c7 = (seq_t)(k - 1);
            seq_t c8 = (seq_t)(2*k - 3);
            seq_t c9 = (seq_t)51;
            seq_t c10 = (seq_t)(k - 3);
            seq_t c11 = (seq_t)(k - 2);
            seq_t c12 = (seq_t)(k - 1);

            seq_t term1 = c1 * c2 * cache[k-1];
            seq_t term2 = c3 * c4 * cache[k-2];
            seq_t term3 = c5 * c6 * c7 * c8 * cache[k-3];
            seq_t term4 = c9 * c10 * c11 * c12 * cache[k-4];

            seq_t num = term1 + term2 + term4;
            num = num - term3;

            seq_t n_cube = (seq_t)k * (seq_t)k * (seq_t)k;
            cache[k] = num / n_cube;
        }
    }

    cached_max = n;
    return cache[n];
}
