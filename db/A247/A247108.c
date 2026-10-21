/*
 * A247108  Complementary Aitken's array: triangle of numbers {a(n,k), n >= 0,
 *          0<=k<=n} read by rows, defined by a(0,0)=1, a(n,0)=-a(n-1,n-1),
 *          a(n,k)=a(n,k-1)+a(n-1,k-1).
 * https://oeis.org/A247108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 41, i16 n <= 88, i32 n <= 187, i64 n <= 457,
 *            i128 n <= 1183, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n,0) where the triangle satisfies:
 *   a(0,0) = 1;
 *   a(n,0) = -a(n-1,n-1);
 *   a(n,k) = a(n,k-1) + a(n-1,k-1).
 * The function returns the n‑th term of the sequence (a(n,0)). */

#include <stdlib.h>
#include <stdint.h>

/* Find the row r and column k for the flattened index n.
 * Return r, written to *k_out. */
static idx_t find_row(idx_t n, idx_t *k_out)
{
    /* Exponential search to bound r. */
    idx_t low = 0;
    idx_t high = 1;
    while (high * (high + 1ULL) / 2ULL <= n) {
        if (high > (UINT64_MAX >> 1)) break;   /* avoid overflow */
        high <<= 1;
    }
    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = low + (high - low + 1ULL) / 2ULL;
        if (mid * (mid + 1ULL) / 2ULL <= n)
            low = mid;
        else
            high = mid - 1ULL;
    }
    *k_out = n - low * (low + 1ULL) / 2ULL;
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t k, r = find_row(n, &k);

    /* Allocate space for two consecutive rows of length r+1. */
    seq_t *prev = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));
    if (!prev || !curr) {
        if (prev) free(prev);
        if (curr) free(curr);
        return (seq_t)0;
    }

    /* Row 0 */
    prev[0] = (seq_t)1;

    /* Build rows 1 .. r. */
    for (idx_t i = 1; i <= r; ++i) {
        curr[0] = -prev[i - 1];          /* a(i,0) = -a(i-1,i-1) */
        for (idx_t j = 1; j <= i; ++j) {
            curr[j] = curr[j - 1] + prev[j - 1];  /* a(i,j) = a(i,j-1)+a(i-1,j-1) */
        }
        /* Swap prev and curr for next iteration. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[k];
    free(prev);
    free(curr);
    return result;
}
