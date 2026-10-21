/*
 * A289657  Number of 3-abelian equivalence classes of words of length n over
 *          a binary alphabet.
 * https://oeis.org/A289657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 3*a(n-1) -
 *            2*a(n-2) - a(n-3) + a(n-5) + 2*a(n-6) - 3*a(n-7) + a(n-8) + p[n]
 *            * where the numerator of the generating function gives p[n] =
 *            {1,-1,0,1,2,3,0,1,-1,2,0,0,...}
 * Range:     u8 n <= 8, u16 n <= 34, u32 n <= 528, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cached values of the sequence */
static seq_t *seq_cache = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* base values a(0) .. a(7) */
    static const seq_t base[8] = {1, 2, 4, 8, 16, 32, 60, 106};

    /* p[n] values taken from the numerator of the generating function */
    static const short p_val[10] = {  1, -1,  0,  1,  2,  3,  0,  1, -1,  2 };

    /* initialise cache if not yet done */
    if (cache_len == 0) {
        idx_t init_len = (n >= 7 ? n + 1 : 8);
        seq_cache = (seq_t *)malloc(init_len * sizeof(seq_t));
        if (!seq_cache) return (seq_t)0;     /* allocation failed */

        /* store base values */
        for (idx_t i = 0; i < 8; ++i) seq_cache[i] = base[i];
        cache_len = init_len;
    }

    /* grow cache to at least n+1 */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failed */
        seq_cache = tmp;

        /* compute remaining values up to n using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* Only for indices >= 8 does the recurrence apply */
            if (i < 8) {
                /* the first 8 terms are already known */
                seq_cache[i] = base[i];
                continue;
            }
            /* compute using only additions and a single subtraction */
            seq_t pos =   3 * seq_cache[i - 1]          /* 3*a(n-1) */
                      +     seq_cache[i - 5]            /* + a(n-5) */
                      + 2 * seq_cache[i - 6]           /* + 2*a(n-6) */
                      +     seq_cache[i - 8];          /* + a(n-8) */

            seq_t neg =   2 * seq_cache[i - 2]          /* 2*a(n-2) */
                      +     seq_cache[i - 3]          /* + a(n-3) */
                      + 3 * seq_cache[i - 7];         /* + 3*a(n-7) */

            seq_cache[i] = pos - neg;                 /* pos >= neg for all i≥8 */

            /* add the p[i] term if i < 10 */
            if (i < 10) seq_cache[i] += (seq_t)p_val[i];
        }
        cache_len = new_len;
    }

    return seq_cache[n];
}
