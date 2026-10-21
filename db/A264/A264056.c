/*
 * A264056  Number of (n+1) X (4+1) arrays of permutations of 0..n*5+4 with
 *          each element having index change +-(.,.) 0,0 1,2 or 2,-2.
 * https://oeis.org/A264056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 15*a(n-1) - 25*a(n-2) + 375*a(n-3) - 5625*a(n-4)
 *               + 9375*a(n-5) - 15625*a(n-6) + 234375*a(n-7) - 390625*a(n-8)
 * for n > 10, with initial values a(1)=8, a(2)=216, a(3)=2592, a(4)=32400,
 * a(5)=450000, a(6)=5760000, a(7)=74880000, a(8)=988650000,
 * a(9)=12899250000, a(10)=168682500000. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms.  cache[i] holds a(i+1). */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;  /* number of terms stored (a(1) ... a(cache_len)) */
    static size_t cache_cap  = 0;  /* allocated capacity of cache */

    /* Initial terms for n = 1 .. 10 */
    const seq_t init_vals[10] = {
        (seq_t)8ULL,
        (seq_t)216ULL,
        (seq_t)2592ULL,
        (seq_t)32400ULL,
        (seq_t)450000ULL,
        (seq_t)5760000ULL,
        (seq_t)74880000ULL,
        (seq_t)988650000ULL,
        (seq_t)12899250000ULL,
        (seq_t)168682500000ULL
    };

    /* a(0) is undefined; return 0 for safety. */
    if (n == 0) return (seq_t)0;

    /* Allocate cache the first time the function is called. */
    if (cache_len == 0) {
        cache_cap = 16;                         /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failure */
        for (size_t i = 0; i < 10; ++i) cache[i] = init_vals[i];
        cache_len = 10;
    }

    /* Grow the cache and compute missing terms if needed. */
    if ((size_t)n > cache_len) {
        size_t needed = (size_t)n;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap < needed) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;          /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute terms from cache_len+1 up to n using the recurrence. */
        for (size_t k = cache_len + 1; k <= needed; ++k) {
            if (k <= 10) /* safety for extremely small n, though should not happen */
                cache[k-1] = init_vals[k-1];
            else {
                /* coefficients for the recurrence */
                const int coeffs[8] = {
                    15,   -25,    375,  -5625,
                    9375, -15625, 234375, -390625
                };

                seq_t sum_pos = 0;
                seq_t sum_neg = 0;
                for (int offset = 1; offset <= 8; ++offset) {
                    seq_t val  = cache[(size_t)(k - offset) - 1];
                    int   coeff = coeffs[offset-1];
                    if (coeff > 0)
                        sum_pos += val * (seq_t)coeff;
                    else
                        sum_neg += val * (seq_t)(-coeff);
                }
                cache[k-1] = sum_pos - sum_neg;   /* safe: result is non‑negative */
            }
        }
        cache_len = needed;
    }

    return cache[n-1];
}
