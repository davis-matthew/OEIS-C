/*
 * A348861  G.f. A(x) satisfies: A(x) = 1 / ((1 + x) * (1 - x * A(3*x))).
 * https://oeis.org/A348861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(n) = (-1)^n + Σ_{k=0}^{n-1} 3^k * a(k) * a(n-1-k) */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Persistent cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n >= cache_len) {                       /* extend cache if needed */
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) { /* allocation failed – leave cache unchanged */
            return 0;
        }
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {                       /* base case */
                cache[0] = 1;
                continue;
            }
            seq_t conv = 0;                     /* convolution sum */
            seq_t pow3 = 1;
            for (idx_t k = 0; k < i; ++k) {     /* Σ 3^k * a(k) * a(i-1-k) */
                seq_t term = cache[k];
                term *= cache[i - 1 - k];
                term *= pow3;
                conv += term;
                pow3 *= 3;
            }
            if (i & 1) {                        /* odd n : (-1)^n = –1 */
                conv -= 1;                      /* conv - 1 */
            } else {                            /* even n : (-1)^n = +1 */
                conv += 1;                      /* conv + 1 */
            }
            cache[i] = conv;
        }
        cache_len = new_len;
    }
    return cache[n];
}
