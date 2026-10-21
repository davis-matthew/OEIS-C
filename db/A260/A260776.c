/*
 * A260776  Certain directed lattice paths.
 * https://oeis.org/A260776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/n)*C(5*n,2*n-1) for n >= 1, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    static seq_t *cache = NULL;   /* memoisation array                */
    static idx_t cache_len = 0;   /* current number of stored terms    */

    idx_t old_len = cache_len;     /* number of terms before grow        */

    /* Grow cache if necessary                                   */
    if (cache_len <= n) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed – unlikely in tests */
        cache = tmp;
        if (cache_len == 0) {          /* initialise first two entries       */
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)5;
        }
        cache_len = n + 1;
    }

    /* Compute missing values using the recurrence            */
    for (idx_t i = (old_len > 2 ? old_len : 2); i <= n; ++i) {
        if (sizeof(seq_t) <= 8) {        /* 64‑bit or smaller types           */
            __uint128_t num = 5;
            num *= (uint64_t)(5 * i - 4);
            num *= (uint64_t)(5 * i - 3);
            num *= (uint64_t)(5 * i - 2);
            num *= (uint64_t)(5 * i - 1);

            __uint128_t den = 6;
            den *= (uint64_t)i;
            den *= (uint64_t)(3 * i - 1);
            den *= (uint64_t)(2 * i - 1);
            den *= (uint64_t)(3 * i + 1);

            __uint128_t prod = (__uint128_t)cache[i - 1] * num;
            __uint128_t val   = prod / den;
            cache[i] = (seq_t)val;
        } else {                         /* arbitrary‑precision types           */
            seq_t num = (seq_t)5;
            num *= (seq_t)(5 * i - 4);
            num *= (seq_t)(5 * i - 3);
            num *= (seq_t)(5 * i - 2);
            num *= (seq_t)(5 * i - 1);

            seq_t den = (seq_t)6;
            den *= (seq_t)i;
            den *= (seq_t)(3 * i - 1);
            den *= (seq_t)(2 * i - 1);
            den *= (seq_t)(3 * i + 1);

            cache[i] = (cache[i - 1] * num) / den;
        }
    }

    return cache[n];
}
