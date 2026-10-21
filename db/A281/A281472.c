/*
 * A281472  Number of 4 X n 0..1 arrays with no element equal to more than two
 *          of its horizontal, diagonal or antidiagonal neighbors and with new
 *          values introduced in order 0 sequentially upwards.
 * https://oeis.org/A281472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-5) for n>9 a(1)=8,
 *            a(2)=81, a(3)=60, a(4)=96, a(5)=130, a(6)=176, a(7)=260,
 *            a(8)=406, a(9)=636 (Offset 1)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* highest index computed so far */

    if (n == 0) return 0;           /* undefined for n=0, but safe guard */

    /* Expand cache if necessary */
    if ((size_t)n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
        if (!new_cache) return 0;          /* allocation failed */
        cache = new_cache;
        /* Initialise new slots to 0 (indicates not yet computed) */
        for (size_t i = cache_len + 1; i <= new_len; ++i) cache[i] = 0;
        cache_len = new_len;
    }

    /* If already computed return */
    if (cache[(size_t)n]) return cache[(size_t)n];

    /* Initial values for 1..9 */
    static const seq_t init[10] = {0, 8, 81, 60, 96, 130, 176, 260, 406, 636};

    /* Compute missing values up to n */
    for (idx_t i = 1; (idx_t)i <= n; ++i) {
        if (cache[(size_t)i]) continue;            /* already computed */
        if (i <= 9) {
            cache[(size_t)i] = init[(size_t)i];
        } else {
            seq_t t = cache[(size_t)(i-1)];
            t += cache[(size_t)(i-1)];          /* 2*a(i-1) */
            t -= cache[(size_t)(i-2)];          /* -a(i-2) */
            t += cache[(size_t)(i-3)];          /* +a(i-3) */
            t -= cache[(size_t)(i-5)];          /* -a(i-5) */
            cache[(size_t)i] = t;
        }
    }

    return cache[(size_t)n];
}
