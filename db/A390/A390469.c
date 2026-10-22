/*
 * A390469  Array read by ascending antidiagonals: A(n,k) = [x^n] (1 - x^k)/(1
 *          - 2*x + x^(1+k)), with k >= 0.
 * https://oeis.org/A390469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = A(i,j) where A(n,k) is the coefficient of x^n in * (1 -
 *            x^k)/(1 - 2*x + x^(1+k)). The indices (i,j) are obtained by *
 *            reading the 2‑D array a(i,j) along ascending antidiagonals: *
 *            let s = floor( (sqrt(8*n+1)-1)/2 ) * let o = n - s*(s+1)/2 * i =
 *            s - o (row) * j = o (column, the k in A(i,k))...
 * Range:     u8 n <= 80, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Compute the index s such that s*(s+1)/2 ≤ n < (s+1)*(s+2)/2
 * using a binary search with 128‑bit arithmetic to avoid overflow. */
static inline idx_t diagonal_of(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;

    /* find an upper bound */
    while ((__int128)high * (high + 1) / 2 <= n)
        high <<= 1;

    /* binary search */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* mapping from n to (i,j) */
    idx_t s = diagonal_of(n);                            /* s = i + j */
    idx_t prev_sum = (idx_t)((__int128)s * (s + 1) / 2); /* ∑_{t=0}^{s-1} t+1 */
    idx_t o = n - prev_sum;                              /* position in diagonal */
    idx_t i = s - o;                                     /* row index */
    idx_t j = o;                                         /* column index (k) */

    /* j == 0 → no sequences allowed */
    if (j == 0)
        return (seq_t)0;

    /* Allocate an array f[0..i] to hold the recurrence values */
    seq_t *f = (seq_t*)malloc((i + 1) * sizeof(seq_t));
    if (!f)          /* allocation failure – fail hard */
        abort();

    f[0] = (seq_t)1; /* A(0,j) = 1 for j>0 */

    for (idx_t t = 1; t <= i; ++t) {
        if (t < j) {
            f[t] = ((seq_t)1 << t);              /* 2^t             */
        } else if (t == j) {
            f[t] = ((seq_t)1 << t) - 1;          /* 2^t - 1         */
        } else {
            f[t] = (seq_t)(2 * f[t - 1] - f[t - j - 1]); /* recurrence */
        }
    }

    seq_t result = f[i];
    free(f);
    return result;
}
