/*
 * A291585  a(n) = ((2n-1)!!)^3 * Sum_{i=1..n} 1/(2*i-1)^3.
 * https://oeis.org/A291585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2n-1)!!)^3 * \sum_{i=1}^n 1/(2i-1)^3. Recurrence:
 *            a(0)=0, a(1)=1, a(n+1)=((2n-1)^3+(2n+1)^3)*a(n)-(2n-1)^6*a(n-1)
 *            for n>0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>   /* for uint64_t if needed */

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t last_computed = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_len = 2;                     /* hold a0 and a1 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failed */
        cache[0] = 0;
        cache[1] = 1;
        last_computed = 1;
        if (n == 0) return cache[0];
        if (n == 1) return cache[1];
    } else if (n <= last_computed) {
        /* already computed */
        return cache[n];
    }

    /* make room for required indices */
    if ((idx_t)n + 1 > cache_len) {
        idx_t new_len = (idx_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failed */
        cache = tmp;
        cache_len = new_len;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* recurrence uses m = i-1 */
        seq_t m = (seq_t)(i - 1);
        seq_t t = 2 * m - 1;                /* 2m-1 = 2i-3 */
        seq_t t_cubed = t * t * t;
        seq_t t_plus = t + 2;               /* 2m+1 = t+2 = 2i+1 */
        seq_t t_plus_cubed = t_plus * t_plus * t_plus;
        seq_t coeff = t_cubed + t_plus_cubed;   /* (2i-3)^3 + (2i-1)^3 */
        seq_t t_sixth = t_cubed * t_cubed;      /* (2i-3)^6 */

        seq_t prev  = cache[i - 1];
        seq_t prev2 = cache[i - 2];

        cache[i] = coeff * prev - t_sixth * prev2;
    }
    last_computed = n;
    return cache[n];
}
