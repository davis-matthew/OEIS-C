/*
 * A331326  a(n) = n!*[x^n] sinh(x/(1 - x))/(1 - x).
 * https://oeis.org/A331326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by a(n+4) = 4*(n+3)*a(n+3) -
 *            (6*n^2+30*n+37)*a(n+2) + 4*(n+2)^3*a(n+1) - (n+1)^2*(n+2)^2*a(n)
 *            Initial values: a(0)=0, a(1)=1, a(2)=4, a(3)=19 (OEIS A331326)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (cache_len == 0) {              /* initialise first four values */
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)19;
    }

    if ((idx_t)cache_len <= n) {        /* extend cache up to n */
        size_t new_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

        for (idx_t i = cache_len; i <= n; ++i) {
            idx_t k = i - 4;          /* recurrence index n = k  */
            seq_t a0 = cache[k];
            seq_t a1 = cache[k + 1];
            seq_t a2 = cache[k + 2];
            seq_t a3 = cache[k + 3];

            seq_t kk = (seq_t)k;
            seq_t coeff = (seq_t)6 * kk * kk + (seq_t)30 * kk + (seq_t)37;

            /* term1 = 4*(n+3)*a(n+3) */
            seq_t term1 = a3 * (seq_t)4 * ((seq_t)(k + 3));

            /* term2 = 4*(n+2)^3*a(n+1) */
            seq_t n2 = (seq_t)(k + 2);
            seq_t n2_cube = n2 * n2 * n2;           /* (n+2)^3 */
            seq_t term2 = a1 * (seq_t)4 * n2_cube;

            /* term3 = -(6*n^2+30*n+37)*a(n+2) */
            seq_t term3 = a2 * coeff;
            term3 = -term3;

            /* term4 = -(n+1)^2*(n+2)^2*a(n) */
            seq_t n1 = (seq_t)(k + 1);
            seq_t n1_sq = n1 * n1;                  /* (n+1)^2 */
            seq_t n2_sq = n2 * n2;                  /* (n+2)^2 */
            seq_t term4 = a0 * n1_sq * n2_sq;
            term4 = -term4;

            /* a(i) = sum of the four terms */
            cache[i] = term1 + term2 + term3 + term4;
        }

        cache_len = new_len;
    }

    return cache[n];
}
