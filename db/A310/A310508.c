/*
 * A310508  Coordination sequence Gal.4.123.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-5) - a(n-10) for n > 10 Initial terms: a(0) = 1,
 *            a(1) = 4, a(2) = 10, a(3) = 16, a(4) = 21, a(5) = 24, a(6) = 29,
 *            a(7) = 36, a(8) = 42, a(9) = 46, a(10) = 48
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache to allow arbitrary call order */
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t len = 0;

    /* initial 11 values */
    static const seq_t init[11] = {
        1ULL, 4ULL, 10ULL, 16ULL, 21ULL, 24ULL,
        29ULL, 36ULL, 42ULL, 46ULL, 48ULL
    };

    /* Grow cache if needed */
    if (n >= cap) {
        idx_t new_cap = (cap < 16) ? 16 : cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failure – unlikely but safe */
        cache = new_cache;
        cap = new_cap;
    }

    /* Compute terms up to n */
    if (len <= n) {
        /* fill initial values */
        if (len < 11) {
            idx_t i = len;
            for ( ; i < 11 && i <= n; ++i)
                cache[i] = init[i];
            len = (i > 10) ? i : 11;
        }

        /* compute remaining terms using recurrence */
        for (idx_t i = len; i <= n; ++i) {
            cache[i] = 2 * cache[i - 5] - cache[i - 10];
        }
        len = n + 1;
    }

    return cache[n];
}
