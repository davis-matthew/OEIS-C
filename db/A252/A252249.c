/*
 * A252249  Number of (n+2) X (3+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum not equal to 0 3 5 6 or 7 and every 3 X 3 diagonal
 *          and antidiagonal sum equal to 0 3 5 6 or 7.
 * https://oeis.org/A252249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) - 3*a(n-4) - a(n-6) + a(n-7) for n>9
 *            Empirical recurrence with a(1)=278, a(2)=242, a(3)=231,
 *            a(4)=249, a(5)=279, a(6)=318, a(7)=366, a(8)=444, a(9)=546
 * Range:     u8 none, u16 n <= 25, u32 n <= 59, u64 n <= 128, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache to allow arbitrary call order and reuse */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity in terms of number of entries (1‑based) */
    static idx_t cache_sz  = 0;   /* number of entries actually computed */

    /* Base values a(1)…a(9) */
    static const seq_t base[9] = {
        (seq_t)278,
        (seq_t)242,
        (seq_t)231,
        (seq_t)249,
        (seq_t)279,
        (seq_t)318,
        (seq_t)366,
        (seq_t)444,
        (seq_t)546
    };

    /* Handle trivial or already‑computed indices */
    if (n == 0)
        return (seq_t)0;          /* undefined for n = 0, return 0 for safety */

    if (n <= cache_sz)
        return cache[n - 1];

    /* Expand cache if needed */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < n)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)calloc((size_t)new_cap, sizeof(seq_t));
        if (!new_cache)                     /* allocation failure → abort */
            abort();
        if (cache) {
            for (idx_t i = 0; i < cache_sz; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = cache_sz + 1; i <= n; ++i) {
        if (i <= 9) {                     /* copy known base values */
            cache[i - 1] = base[i - 1];
        } else {
            /* recurrence: a(i) = a(i-1) + 3*a(i-3) - 3*a(i-4) - a(i-6) + a(i-7) */
            idx_t idx = i - 1;            /* 0‑based index in cache */
            seq_t pos = cache[idx - 1]          /* a(i-1) */
                        + (cache[idx - 3] + cache[idx - 3] + cache[idx - 3] /* 3*a(i-3) */)
                        + cache[idx - 7];          /* a(i-7) */
            seq_t neg = (cache[idx - 4] + cache[idx - 4] + cache[idx - 4] /* 3*a(i-4) */)
                        + cache[idx - 6];          /* a(i-6) */
            cache[idx] = pos - neg;              /* final result, guaranteed non‑negative */
        }
    }

    cache_sz = n;
    return cache[n - 1];
}
