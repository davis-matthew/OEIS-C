/*
 * A338879  Denominators in a set of expansions of the single-term Machin-like
 *          formula for Pi.
 * https://oeis.org/A338879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, tabl
 * Formula:   a(n) = denominator of n / ((n-k)*(n-k+1) + n^2) where (row, k)
 *            is the position of n in the OEIS triangle T(row,k) read
 *            row‑major (rows 1,2,…; columns 1..row). The value is the reduced
 *            denominator of the fraction n / ((n-k)*(n-k+1)+n^2)
 * Range:     u8 n <= 66, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Protect against undefined behavior for n == 0 */
    if (n == 0) return 0;

    /* Find the row such that triangular number >= n. */
    idx_t low = 1, high = 1;
    /* Grow high until it covers n. */
    while ( ((unsigned __int128)high * (high + 1) / 2) < n ) {
        high <<= 1;
    }
    /* Binary search for the row. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ( ((unsigned __int128)mid * (mid + 1) / 2) >= n )
            high = mid;
        else
            low = mid + 1;
    }
    idx_t row = low;
    idx_t prev_tri = (row - 1) * row / 2;   /* triangular number of row-1 */
    idx_t k = n - prev_tri;                 /* column index (1‑based) */

    seq_t N = (seq_t)row;
    seq_t K = (seq_t)k;
    seq_t diff = N - K;                     /* n - k */
    seq_t D = diff * (diff + 1) + N * N;    /* denominator before reduction */

    seq_t g = gcd(N, D);                    /* reduce */
    return D / g;
}
