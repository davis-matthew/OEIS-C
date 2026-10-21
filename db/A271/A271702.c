/*
 * A271702  Triangle read by rows, T(n,k) = Sum_{j=0..n}
 *          (-1)^(n-j)*C(-j-1,-n-1)*S2(k,j), S2 the Stirling set numbers
 *          A048993, for n>=0 and 0<=k<=n.
 * https://oeis.org/A271702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, T(r,k) = Sum_{j=0..k} binom(r,j) * S2(k,j) (Stirling
 *            set numbers)
 * Range:     u8 n <= 19, u16 n <= 43, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Determine the row r and column k from the linear index n */
    double d = sqrt((double)8 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);
    /* Correct possible rounding error */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* Allocate arrays for Stirling numbers of the second kind S2(k,j) */
    size_t sz = (size_t)k + 1;
    seq_t *prev = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *curr = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!prev || !curr) {
        if (prev) free(prev);
        if (curr) free(curr);
        return 0; /* Out of memory: fallback */
    }

    /* Initialize S2(0,0)=1 */
    for (size_t i = 0; i < sz; ++i) prev[i] = 0;
    prev[0] = 1;

    /* Generate S2 up to n = k */
    for (size_t i = 1; i <= k; ++i) {
        curr[0] = 0;
        for (size_t j = 1; j <= i; ++j) {
            seq_t t1 = 0;
            if (j <= i - 1) t1 = prev[j] * (seq_t)j; /* j * S2(i-1,j) */
            seq_t t2 = prev[j - 1];                  /* + S2(i-1,j-1) */
            curr[j] = t1 + t2;
        }
        /* Swap buffers for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Compute the binomial convolution */
    seq_t sum = 0;
    for (size_t j = 0; j <= k; ++j) {
        seq_t bin = binom(r, j);
        sum += bin * prev[j];
    }

    free(prev);
    free(curr);
    return sum;
}
