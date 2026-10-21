/*
 * A251123  Number of (n+1) X (3+1) 0..1 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements greater than the maximum of its
 *          antidiagonal elements.
 * https://oeis.org/A251123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) – 5*a(n-2) + a(n-3) + 2*a(n-4) – a(n-5) *
 *            Empirical recurrence for n > 6. * Initial values: a(1)=40,
 *            a(2)=69, a(3)=108, a(4)=173, a(5)=272, a(6)=430
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

static const idx_t base_count = 6;
static const seq_t base_vals[6] = {40, 69, 108, 173, 272, 430};

#if defined(__cplusplus)
static std::vector<seq_t> cache;
static idx_t cache_max = 0;
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* kept only for symmetry */
static idx_t cache_max = 0;
#endif

/* Ensure cache has space and values up to n */
static void ensure_values(idx_t n)
{
    if (n <= cache_max)
        return;

#if defined(__cplusplus)
    cache.resize(n + 1);               /* vector is 0‑based; ignore element 0 */
#else
    size_t new_cap = n + 1;
    if (!cache) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                  /* unrecoverable allocation failure */
        cache = tmp;
    }
    cache_cap = new_cap;
#endif

    /* initialise the first few entries with the base values */
    for (idx_t i = 1; i <= base_count && i <= n; ++i)
        cache[i] = base_vals[i - 1];

    /* compute new entries using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val;
        if (i <= base_count) {
            val = base_vals[i - 1];
        } else {
#if defined(__cplusplus)
            val = 4 * cache[i - 1] - 5 * cache[i - 2] + cache[i - 3]
                  + 2 * cache[i - 4] - cache[i - 5];
#else
            /* use a signed 64‑bit intermediate to avoid unsigned wrap */
            long long tmp = 0;
            tmp += 4LL * (long long)cache[i - 1];
            tmp -= 5LL * (long long)cache[i - 2];
            tmp += (long long)cache[i - 3];
            tmp += 2LL * (long long)cache[i - 4];
            tmp -= (long long)cache[i - 5];
            val = (seq_t)tmp;
#endif
        }
        cache[i] = val;
    }

    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined indices return 0 */
    ensure_values(n);
    return cache[n];
}
