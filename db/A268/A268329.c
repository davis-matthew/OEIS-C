/*
 * A268329  Expansion of (1 - sqrt(1 - 4*x))^5/16.
 * https://oeis.org/A268329
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10 * binomial(2*n-6, n-5)/n Recurrence used: a(n) =
 *            (2*(n-3)*(2*n-7)/ (n*(n-5))) * a(n-1) for n ≥ 6 Initial value:
 *            a(5) = 2
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>   /* for memset */
#include "../utils/gcd.h"   /* for seq_t gcd(seq_t, seq_t) */

seq_t a(idx_t n) {
    if (n < 5) return 0;  /* behaviour below offset is irrelevant */

    /* static cache for values already computed */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;      /* allocated size */
    static idx_t max_cached = 4;     /* last index computed (n) */

    if (n <= max_cached) {
        return cache[n];
    }

    /* allocate or enlarge cache */
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        memset(cache, 0, cache_cap * sizeof(seq_t));
        /* base value */
        cache[5] = 2;
        max_cached = 5;
    } else if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        memset(cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute new values using the recurrence */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t a_prev = cache[i - 1];
        idx_t ii = i;

        /* rational factor: num/den */
        seq_t num = (seq_t)2 * (seq_t)(ii - 3) * (seq_t)(2 * ii - 7);
        seq_t den = (seq_t)ii * (seq_t)(ii - 5);

        /* reduce factor */
        seq_t g1 = gcd(num, den);
        num /= g1;
        den /= g1;

        /* reduce with previous value */
        seq_t g2 = gcd(a_prev, den);
        a_prev /= g2;
        den /= g2;   /* den should now be 1 for exact division */

        /* multiply and divide */
        seq_t prod = a_prev * num;
        seq_t an = prod / den;   /* exact division */

        cache[i] = an;
    }

    max_cached = n;
    return cache[n];
}
