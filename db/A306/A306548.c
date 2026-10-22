/*
 * A306548  Triangle T(n,k) read by rows, where the k-th column is the shifted
 *          self-convolution of the power function n^k, n >= 0, 0 <= k <= n.
 * https://oeis.org/A306548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,col) where T(row,col)= * sum_{t=0}^{row-col} t^col
 *            · (row-col-t)^col, * row and col are obtained by interpreting n
 *            as a flattened * triangle with rows of length row+1. The
 *            convention 0^0=0 * is used in the definition of T
 * Range:     u8 n <= 37, u16 n <= 70, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* 0^0 is defined as 0; otherwise standard exponentiation.  */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    if (exp == 0)
        return (base == 0) ? (seq_t)0 : (seq_t)1;
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/* Find the row that contains index n in the flattened triangle. */
static inline idx_t compute_row(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Exponential search for an upper bound. */
    while ((unsigned __int128)(high + 1) * (high + 2) / 2 <= n) {
        high <<= 1;
    }
    /* Binary search in [low, high]. */
    while (low < high) {
        idx_t mid = low + (high - low + 1U) / 2U;
        if ((unsigned __int128)mid * (mid + 1U) / 2 <= n) {
            low = mid;
        } else {
            high = mid - 1U;
        }
    }
    return low;
}

/* Main entry function. */
seq_t a(idx_t n)
{
    idx_t row = compute_row(n);
    idx_t count_before = (idx_t)((unsigned __int128)row * (row + 1U) / 2);
    idx_t col = n - count_before;          /* column index within the row */
    idx_t m   = row - col;                  /* upper limit for the inner sum */

    seq_t sum = (seq_t)0;
    for (idx_t t = 0; t <= m; ++t) {
        seq_t term1 = pow_seq((seq_t)t, col);
        seq_t term2 = pow_seq((seq_t)(m - t), col);
        sum += term1 * term2;
    }
    return sum;
}
