/*
 * A394081  Triangle read by rows: T(n, k) = n^(n - k)*Gamma(k + 1, k +
 *          1)*exp(k + 1).
 * https://oeis.org/A394081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) with * row = floor((sqrt(8*n+1)-1)/2), * col
 *            = n - row*(row+1)/2, * T(row, col) = row^(row-col) * col! * *
 *            sum_{m=0}^col (col+1)^m / m!. * This implements the triangle
 *            entry T(n,k) given in OEIS A394081 * and reads the triangle
 *            row‑by‑row
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: floor(sqrt(x)) for 0 <= x < 2^64. */
static idx_t floor_sqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* Integer power: base^exp for seq_t base and idx_t exponent. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

/* Computes the triangle entry T(row, col). */
static seq_t compute_T(idx_t row, idx_t col)
{
    /* factorial of k (col) */
    seq_t fact = 1;
    for (idx_t i = 1; i <= col; ++i) {
        fact *= (seq_t)i;
    }

    /* sum_{m=0}^{col} (col! / m!) * (col+1)^m */
    seq_t sum = 0;
    seq_t termFactor = fact;       /* col! / m!  (updated each step) */
    seq_t powVal = 1;              /* (col+1)^m    */
    seq_t baseKK = (seq_t)(col + 1);
    for (idx_t m = 0; m <= col; ++m) {
        sum += termFactor * powVal;
        if (m < col) {
            termFactor /= (seq_t)(m + 1);   /* exact division */
            powVal *= baseKK;
        }
    }

    /* row^(row-col) */
    idx_t exp = row - col;
    seq_t pown = pow_seq_t((seq_t)row, exp);

    return pown * sum;
}

/* Public entry point for OEIS sequence A394081. */
seq_t a(idx_t n)
{
    /* Determine (row, col) from linear index n. */
    __int128 tmp128 = (__int128)8 * n + 1;
    idx_t tmp = (idx_t)tmp128;                 /* 8*n+1 fits in idx_t for n up to 10^4 */
    idx_t root = floor_sqrt(tmp);
    idx_t row   = (root - 1) >> 1;             /* floor((sqrt(8*n+1)-1)/2) */
    idx_t prev_rows = row * (row + 1) / 2;      /* elements before this row */
    idx_t col = n - prev_rows;

    return compute_T(row, col);
}
