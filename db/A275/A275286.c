/*
 * A275286  a(n) = ((2n+1)!!)^2 * Sum_{k=0..n}(-1)^k/(2k+1)^2.
 * https://oeis.org/A275286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((2n+1)!!)^2 * Sum_{k=0..n}(-1)^k/(2k+1)^2 Recurrence
 *            used: a(0)=1; a(n) = (2n+1)^2 * a(n-1) + (-1)^n * ((2n)!/n!)^2 /
 *            4^n (n ≥ 1) The recurrence produces an integer value for every
 *            n. The implementation memoises computed terms up to the largest
 *            n requested
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t last        = (idx_t)-1;   /* largest index computed */
    static seq_t t_prev = 0;               /* t_{n-1} in the recurrence */

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 16) new_cap = 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure guard */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute all missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[0] = (seq_t)1;
            t_prev   = (seq_t)1;  /* t_0 = 1 */
        }
        else
        {
            /* compute t_i = ((2i)!/i!)^2 / 4^i, using the recurrence
               t_i = t_{i-1} * ((2i)*(2i-1))^2 / (i^2 * 4)           */
            seq_t two_i       = (seq_t)(2 * i);
            seq_t factor      = two_i * (two_i - 1);  /* (2i)*(2i-1) */
            seq_t factor_sq   = factor * factor;      /* ((2i)*(2i-1))^2 */
            seq_t numerator   = t_prev * factor_sq;
            seq_t denominator = (seq_t)i * (seq_t)i * 4;
            seq_t t_i = numerator / denominator;

            /* compute a_i = (2i+1)^2 * a_{i-1} + (-1)^i * t_i */
            seq_t two_ip1 = (seq_t)(2 * i + 1);
            seq_t prod = two_ip1 * two_ip1 * cache[i - 1];
            if (i & 1U)            /* odd i -> subtract */
                cache[i] = prod - t_i;
            else                  /* even i -> add */
                cache[i] = prod + t_i;

            t_prev = t_i;          /* prepare for next step */
        }
    }
    last = n;
    return cache[n];
}
