/*
 * A249138  Triangular array read by rows: row n gives the coefficients of the
 *          polynomial p(n,x) defined in Comments.
 * https://oeis.org/A249138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = coefficient of x^(n - r(r+1)/2) in the polynomial N_r(x)
 *            defined by N_0(x)=1, N_1(x)=1+x,
 *            N_k(x)=floor((k+2)/2)*N_{k-2}(x)+x*N_{k-1}(x) (k≥2)
 * Range:     u8 n <= 37, u16 n <= 93, u32 n <= 256, u64 n <= 707,
 *            u128 n <= 2020, big any n
 * Helpers:   start_of_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/start_of_row.h"

static inline idx_t
row_of(idx_t n)
{
    idx_t r = 0;
    while (((( __int128)(r + 1) * (r + 2)) / 2) <= n)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    idx_t j = n - start_of_row(r);

    if (r == 0)
        return (seq_t)1;
    if (r == 1)
        return (seq_t)1;

    seq_t *prev2;
    seq_t *prev1;
    seq_t *curr;

#ifdef __cplusplus
    prev2 = new seq_t[1];
    prev1 = new seq_t[2];
#else
    prev2 = malloc(sizeof(seq_t));
    prev1 = malloc(2 * sizeof(seq_t));
#endif
    prev2[0] = (seq_t)1;
    prev1[0] = (seq_t)1;
    prev1[1] = (seq_t)1;

    for (idx_t k = 2; k <= r; ++k) {
        size_t new_len = (size_t)(k + 1);
#ifdef __cplusplus
        curr = new seq_t[new_len];
#else
        curr = malloc(new_len * sizeof(seq_t));
#endif
        for (size_t t = 0; t < new_len; ++t)
            curr[t] = (seq_t)0;

        seq_t ak = (seq_t)((k + 2) / 2);
        for (idx_t i = 0; i <= k - 2; ++i)
            curr[i] += ak * prev2[i];
        for (idx_t i = 0; i <= k - 1; ++i)
            curr[i + 1] += prev1[i];

#ifdef __cplusplus
        delete[] prev2;
#else
        free(prev2);
#endif
        prev2 = prev1;
        prev1 = curr;
    }

    seq_t result = prev1[j];

#ifdef __cplusplus
    delete[] prev2;
    delete[] prev1;
#else
    free(prev2);
    free(prev1);
#endif
    return result;
}
