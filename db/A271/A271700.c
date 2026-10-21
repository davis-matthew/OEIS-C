/*
 * A271700  Triangle read by rows, T(n,k) = Sum_{j=0..n}
 *          (-1)^(n-j)*C(-j-1,-n-1)*S1(k,j), S1 the Stirling cycle numbers
 *          A132393, for n>=0 and 0<=k<=n.
 * https://oeis.org/A271700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened triangle defined by T(r,c) = Sum_{j=0..c}
 *            C(r,j) * S1(c,j) where S1 are the unsigned Stirling cycle
 *            numbers
 * Range:     u8 n <= 19, u16 n <= 34, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A271700 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t lo = 0, hi = 1;
    /* Ensure hi is large enough: increase until hi*(hi+1)/2 > n */
    while (true) {
        __int128 val = (__int128)hi * (hi + 1) / 2;
        if (val > n) break;
        if (hi > (UINT64_MAX >> 1)) { hi = UINT64_MAX; break; }
        hi <<= 1;
    }
    /* Binary search for smallest hi with hi*(hi+1)/2 > n */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    idx_t r = lo - 1;
    __int128 cum128 = (__int128)r * (r + 1) / 2;
    idx_t cum = (idx_t)cum128;
    idx_t c = n - cum;       /* column within row */

    /* Compute Stirling numbers of the first kind S1(c,j) for 0 <= j <= c */
    seq_t *prev = (seq_t *)malloc((c + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((c + 1) * sizeof(seq_t));
    for (idx_t j = 0; j <= c; ++j)
        prev[j] = 0;
    prev[0] = 1;            /* S1(0,0) = 1 */

    for (idx_t size = 1; size <= c; ++size) {
        curr[0] = 0;                     /* S1(n,0) = 0 for n>0 */
        curr[size] = 1;                  /* S1(n,n) = 1 */
        for (idx_t j = 1; j < size; ++j) {
            seq_t term = prev[j];
            term = term * (size - 1);    /* (size-1)*prev[j] */
            curr[j] = prev[j - 1] + term;
        }
        /* Swap buffers */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }
    /* prev now holds S1(c,j) */

    /* Compute the sum T(r,c) = sum_{j=0..c} C(r,j) * S1(c,j) */
    seq_t sum = 0;
    seq_t binom = 1;   /* C(r,0) = 1 */
    for (idx_t j = 0; j <= c; ++j) {
        if (j > 0)
            binom = binom * (r - j + 1) / j;
        sum += binom * prev[j];
    }

    free(prev);
    free(curr);
    return sum;
}
