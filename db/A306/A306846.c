/*
 * A306846  Square array A(n,k), n >= 0, k >= 1, read by antidiagonals, where
 *          column k is the expansion of g.f. ((1-x)^(k-1))/((1-x)^k-x^k).
 * https://oeis.org/A306846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = Sum_{j=0..floor(n/k)} binomial(n,k*j)
 * Range:     u8 n <= 43, u16 n <= 151, u32 n <= 559, u64 n <= 2143,
 *            u128 n <= 8382, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Mapping for a(n):  a(n) = A(i,k) where i is the row index and k≥1
 * derived from the antidiagonal traversal of the square array.
 */

#include <stdint.h>

/* Helper: compute binomial coefficient with minimal over‑flow risk */
static seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;

    /* If seq_t is at most 64 bits, use 128‑bit intermediate to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        __uint128_t r = 1;
        for (idx_t i = 1; i <= k; ++i) {
            r = r * (n - k + i);
            r /= i;
        }
        return (seq_t)r;
    } else {
        /* seq_t is an arbitrary‑precision type; use its own arithmetic. */
        seq_t r = 1;
        for (idx_t i = 1; i <= k; ++i) {
            r = r * (seq_t)(n - k + i);
            r = r / (seq_t)i;
        }
        return r;
    }
}

seq_t a(idx_t n)
{
    /* Determine the diagonal (d) containing the n-th term.
       Diagonals are numbered 0‑based and contain d+1 elements. */
    idx_t d = 0;
    while (((__uint128_t)(d + 1) * (d + 2)) / 2 <= n)
        ++d;

    /* Number of elements before this diagonal */
    idx_t tot_prev = (__uint128_t)d * (d + 1) / 2;
    /* Position within the diagonal (0‑based) */
    idx_t pos = n - tot_prev;

    /* Coordinates of the element in the square array:
       row index = pos (0‑based)
       column index k = d - pos + 1 (1‑based) */
    idx_t row = pos;
    idx_t k   = d - pos + 1;

    /* Compute A(row,k) = Sum_{t=0..floor(row/k)} binomial(row, k*t) */
    seq_t sum = 0;
    idx_t tmax = row / k;
    for (idx_t t = 0; t <= tmax; ++t) {
        idx_t mult = k * t;          /* multiple of k not exceeding row */
        sum += binom_calc(row, mult);
    }
    return sum;
}
