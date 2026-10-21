/*
 * A285118  Triangle read by rows: T(0,n) = T(n,n) = 0; and for n > 0, 0 < k <
 *          n, T(n,k) = C(n-1,k-1) AND C(n-1,k), where C(n,k) is binomial
 *          coefficient (A007318) & AND is bitwise-AND (A004198).
 * https://oeis.org/A285118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 218, u32 n <= 683, u64 n <= 2446,
 *            u128 n <= 8975, big any n
 * Helpers:   binom_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A285118:  a(n) = C(n-1,k-1) AND C(n-1,k) where n is the row index
 *          and k is the column index within that row, i.e.
 *          a(n) == T(row,k) of the triangle read row‑by‑row.
 *  The value is computed by binomial coefficients modulo 2^64
 *  (so the low 64 bits), then bitwise ANDed.  This avoids overflow
 *  while computing C(n,k) exactly in 64 bits for all n for which
 *  the final result fits in 64 bits. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom_mod64.h"

/* Find the row i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2 */
static inline idx_t find_row(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val <= n) low = mid;
        else high = mid - 1;
    }
    return low;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t row = find_row(n);                 /* i */
    idx_t before = row * (row + 1) / 2;      /* total entries before this row */
    idx_t col  = n - before;                 /* j */

    /* boundary values are zero */
    if (row == 0 || col == 0 || col == row) return (seq_t)0;

    /* internal triangle entry */
    seq_t v1 = binom_mod64(row - 1, col - 1);
    seq_t v2 = binom_mod64(row - 1, col);
    return v1 & v2;
}
