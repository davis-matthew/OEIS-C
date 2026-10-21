/*
 * A247528  Number of length n+3 0..3 arrays with some disjoint pairs in every
 *          consecutive four terms having the same sum.
 * https://oeis.org/A247528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 34, u64 n <= 75, u128 n <= 157,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Empirical recurrence for A247528:
 * a(n) = 2*a(n-1) - a(n-3) + a(n-4) - a(n-5) - a(n-6) + a(n-7)
 * with initial values:
 * a(1)=88, a(2)=136, a(3)=220, a(4)=364, a(5)=604,
 * a(6)=1018, a(7)=1732.
 * The function memoises computed values so calls can come in any order.
 */
#include <stdlib.h>

/* Pre‑computed initial values (index 0 unused). */
static const seq_t init_values[8] = {
    0ULL,   /* placeholder for a(0) */
    88ULL,  /* a(1) */
    136ULL, /* a(2) */
    220ULL, /* a(3) */
    364ULL, /* a(4) */
    604ULL, /* a(5) */
    1018ULL,/* a(6) */
    1732ULL /* a(7) */
};

seq_t a(idx_t n)
{
    /* Handle requested index directly if it lies in the initial range. */
    if (n <= 7) return init_values[(idx_t)n];

    /* Static storage for memoisation. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_max = 0;

    /* Initialise cache on first use. */
    if (cache_cap == 0) {
        cache_cap = 16;                           /* initial capacity */
        cache    = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (!cache) exit(1);                      /* abort on allocation failure */
        cache[0] = 0;
        for (idx_t i = 1; i <= 7; ++i)
            cache[i] = init_values[i];
        cache_max = 7;
    }

    /* Resize if the requested index exceeds current capacity. */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < n) new_cap <<= 1;        /* grow geometrically */
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to the requested index. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t v = 2 * cache[i - 1];
        v -= cache[i - 3];
        v += cache[i - 4];
        v -= cache[i - 5];
        v -= cache[i - 6];
        v += cache[i - 7];
        cache[i] = v;
    }
    cache_max = n;

    return cache[n];
}
