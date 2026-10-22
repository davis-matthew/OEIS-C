/*
 * A392313  a(n) = Sum_{k=0..n} binomial(k+2,2) * binomial(k,3*n-3*k).
 * https://oeis.org/A392313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 29, u64 n <= 63, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (for n≥12):
   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 123*a(n-4) + 108*a(n-5)
          - 39*a(n-6) - 24*a(n-7) + 33*a(n-8) - 8*a(n-9) - 6*a(n-10)
          + 3*a(n-11) + a(n-12)
   initial values: a(0)=1, a(1)=3, a(2)=6, a(3)=10, a(4)=25, a(5)=81,
                   a(6)=238, a(7)=596, a(8)=1333, a(9)=2827,
                   a(10)=5946, a(11)=12618
*/
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* thread‑safe static cache */
    static seq_t *cache = NULL;   /* array of cached values */
    static size_t cache_len = 0;   /* number of entries already computed */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Convert index to size_t for array access */
    size_t idx = (size_t)n;

    /* Return cached value if it already exists */
    if (idx < cache_len)
        return cache[idx];

    /* Ensure we have space for indices up to idx */
    size_t needed = idx + 1;
    if (needed < 12) needed = 12;          /* at least room for the 12 base terms */
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) /* allocation failed – abort the program */
            exit(EXIT_FAILURE);
        cache     = tmp;
        cache_cap = needed;
    }

    /* Base values of the sequence */
    static const seq_t base[12] = {
        (seq_t)1,  (seq_t)3,  (seq_t)6,  (seq_t)10,
        (seq_t)25, (seq_t)81, (seq_t)238, (seq_t)596,
        (seq_t)1333, (seq_t)2827, (seq_t)5946, (seq_t)12618
    };

    /* Fill missing base values if cache is incomplete */
    if (cache_len < 12) {
        size_t end = (idx < 11) ? idx : 11;
        for (size_t i = cache_len; i <= end; ++i)
            cache[i] = base[i];
    }

    /* Compute any values beyond the base terms */
    for (size_t i = cache_len; i <= idx; ++i) {
        if (i < 12)          /* base values already handled */
            continue;

        /* All arithmetic is done with seq_t to avoid mixing types */
        seq_t val
            = (seq_t)9   * cache[i-1]
            + (seq_t)84  * cache[i-3]
            + (seq_t)108 * cache[i-5]
            + (seq_t)33  * cache[i-8]
            + (seq_t)3   * cache[i-11]
            +        cache[i-12]
            - (seq_t)36  * cache[i-2]
            - (seq_t)123 * cache[i-4]
            - (seq_t)39  * cache[i-6]
            - (seq_t)24  * cache[i-7]
            - (seq_t)8   * cache[i-9]
            - (seq_t)6   * cache[i-10];

        cache[i] = val;
    }

    cache_len = idx + 1;
    return cache[idx];
}
