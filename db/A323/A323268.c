/*
 * A323268  The sequence denoted by r_n used in the calculation of A323260.
 * https://oeis.org/A323268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) - 8*a(n-3) + 2*a(n-4) + 6*a(n-5) -
 *            a(n-6); with a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=2, a(6)=6
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small index handling */
    static const seq_t base[7] = {0, 0, 0, 0, 0, 2, 6};

    if (n == 0) return 0;
    if (n <= 6) return base[(size_t)n];

    /* Memoised recurrence */
    static seq_t *cache = NULL;          /* array of computed a(i) */
    static size_t cache_cap = 0;         /* current capacity */
    static size_t cache_len = 0;         /* largest index stored (≥ 6) */

    /* Initialise or extend cache */
    if (cache_len < n) {
        if (cache == NULL) {
            /* First allocation: store base values */
            cache_cap = (n < 64 ? 64 : n + 1);
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) exit(1);
            for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
            for (size_t i = 1; i <= 6; ++i) cache[i] = base[i];
            cache_len = 6;
        } else if (cache_cap <= (size_t)n) {
            size_t new_cap = cache_cap;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute new values up to n using recurrence */
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
            seq_t v = cache[i-1] * ((seq_t)4);
            v += cache[i-2] * ((seq_t)2);
            v -= cache[i-3] * ((seq_t)8);
            v += cache[i-4] * ((seq_t)2);
            v += cache[i-5] * ((seq_t)6);
            v -= cache[i-6];
            cache[i] = v;
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)n];
}
