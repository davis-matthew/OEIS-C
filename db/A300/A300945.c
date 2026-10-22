/*
 * A300945  Rectangular array A(n, k) = hypergeom([-k, k + n/2 - 1], [1], -4)
 *          with row n >= 0 and k >= 0, read by ascending antidiagonals.
 * https://oeis.org/A300945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(row,col), where A(row,col) = hypergeom([-col,
 *            col+row/2-1],[1],-4). The array is enumerated by ascending
 *            antidiagonals: s = ⌊(√(8*n+1)-1)/2⌋ col = n - s(s+1)/2 row = s -
 *            col For k=col>0, T(row,col) is computed by the recurrence:
 *            term_0 = 1 term_j = term_{j-1} * (k-j+1) * (4*k + 2*row + 4...
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t T_val(idx_t row, idx_t col)
{
    if (col == 0)
        return (seq_t)1;

    seq_t sum  = (seq_t)1;   /* term_0 */
    seq_t term = (seq_t)1;   /* term_0 */

    for (idx_t j = 1; j <= col; ++j) {
        seq_t numA = (seq_t)(col - j + 1);
        seq_t numB = (seq_t)(4 * col + 2 * row + 4 * j - 8);
        seq_t denom = (seq_t)j * (seq_t)j;

        seq_t g = gcd(numA, denom);
        numA /= g;
        denom /= g;

        g = gcd(numB, denom);
        numB /= g;
        denom /= g;

        g = gcd(term, denom);
        term /= g;
        denom /= g;

        /* denom should now be 1; if not, cancel with remaining term */
        if (denom != 0)
            term /= denom;

        term = term * numA;
        term = term * numB;
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Find the antidiagonal index s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t low = 0, high = 1;
    while (( (__int128)high * (high + 1) / 2 ) <= n)
        high <<= 1;
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if (( (__int128)mid * (mid + 1) / 2 ) <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t s = low;

    idx_t triangular = (idx_t)(( (__int128)s * (s + 1) ) / 2);
    idx_t col = n - triangular;
    idx_t row = s - col;

    return T_val(row, col);
}
