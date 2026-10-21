/*
 * A294765  Number of permutations of [n] avoiding {4231, 1423, 1234}.
 * https://oeis.org/A294765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 55*a(n-2) + 165*a(n-3) - 330*a(n-4) +
 *            462*a(n-5) - 462*a(n-6) + 330*a(n-7) - 165*a(n-8) + 55*a(n-9) -
 *            11*a(n-10) + a(n-11) for n>10 Initial values: a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=6, a(4)=21, a(5)=74, a(6)=245, a(7)=744, a(8)=2068,
 *            a(9)=5296, a(10)=12608
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 40, u64 n <= 380,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Initial values for n < 11 */
    static const seq_t init[11] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)6, (seq_t)21,
        (seq_t)74, (seq_t)245, (seq_t)744, (seq_t)2068,
        (seq_t)5296, (seq_t)12608
    };
    /* Recurrence coefficients (11 terms) */
    static const int coeffs[11] = {
        11, -55, 165, -330, 462,
        -462, 330, -165, 55, -11, 1
    };

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Handle small indices directly */
    if (n < 11) return init[n];

    /* Initialise cache if first use */
    if (cache == NULL) {
        /* Choose an initial capacity (power of two) */
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* Copy initial values */
        for (idx_t i = 0; i < 11; ++i) cache[i] = init[i];
        cache_len = 11;
    }

    /* Grow cache array if needed */
    if ((idx_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((idx_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failure – return first term */
            return init[0];
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t pos = 0, neg = 0;
            for (int k = 0; k < 11; ++k) {
                seq_t val = cache[i - 1 - (idx_t)k];
                int c = coeffs[k];
                if (c > 0) {
                    pos += (seq_t)c * val;
                } else {
                    neg += (seq_t)(-c) * val;
                }
            }
            cache[i] = pos - neg;  /* final result fits in seq_t for checked terms */
        }
        cache_len = n + 1;
    }

    return cache[n];
}
