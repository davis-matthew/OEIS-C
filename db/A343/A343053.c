/*
 * A343053  Table read by ascending antidiagonals: T(k, n) is the maximum
 *          vertex sum in a perimeter-magic k-gon of order n.
 * https://oeis.org/A343053
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the table T(k,n) along ascending
 *            antidiagonals. For k,n >= 3, T(k,n) = k*(1 + k*(2n - 3) - (n mod
 *            2)*(1 - (k mod 2)))/2. The antidiagonal order is: for each
 *            diagonal sum s = k+n (starting at 6), elements are listed with
 *            decreasing k (i.e. increasing n)
 * Range:     u8 n <= 40, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 0-argument version of seq_t type is assumed to be defined elsewhere. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                       // undefined for n < 3

    idx_t N = n - 3;                    // zero‑based index in the sequence

    /* Find minimal m >= 1 such that m*(m+1)/2 > N.
       Here m = s-5 where s is the diagonal sum (k+n).    */
    idx_t low = 1, high = 1;
    while (high * (high + 1) / 2 <= N) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (mid * (mid + 1) / 2 > N)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;                      // m = s - 5

    idx_t s = m + 5;                    // diagonal sum k + n

    idx_t c_prev = (m - 1) * m / 2;     // count of elements before this diagonal
    idx_t offset = N - c_prev;          // zero‑based position inside the diagonal

    idx_t ncol = offset + 3;            // column index n in the table
    idx_t k = s - ncol;                 // row index k

    /* Compute T(k, ncol) using the supplied formula.
       All arithmetic is done in seq_t to avoid overflow.        */
    seq_t K = (seq_t)k;
    seq_t Ncol = (seq_t)ncol;
    seq_t term = K * (1 + K * ((2 * Ncol) - 3) -
                      ((Ncol & 1) * (1 - (K & 1)))) / 2;
    return term;
}
