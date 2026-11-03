/*
 * A393689  Array read by antidiagonals: A(n, k) is the minimum number of
 *          whole unit squares under the polygonal path (i, h_k(v_i)),
 *          associated to a k-dimensional balanced ballot path, n >= 1, k >=
 *          2.
 * https://oeis.org/A393689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = read antidiagonally from A(n,k) where A(n,k) =
 *            k*(k-2)*(2*k+1)*n/12 when k even, and (k^2-1)*(2*k-3)*n/12 when
 *            k odd
 * Range:     u8 n <= 51, u16 n <= 932, u32 n <= 11325, u64 n <= 11325,
 *            u128 n <= 11325, big any n
 * Validated: standard, pass, 82 terms through n = 9611 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t triangular_root_upper(idx_t n)
{
    /* Return minimal m such that m*(m+1)/2 >= n. */
    idx_t low = 0;
    idx_t high = 1;
    while (((unsigned __int128)high * (high + 1) / 2) < n) {
        high <<= 1;
    }
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (((unsigned __int128)mid * (mid + 1) / 2) >= n) {
            high = mid;
        } else {
            low = mid;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Find the sum index s = m + 2 where m is minimal with T_m >= n. */
    idx_t m = triangular_root_upper(n);
    /* Count of elements before the current antidiagonal: T_{m-1} */
    idx_t prev = ((idx_t)(m - 1) * m) / 2;
    /* Position within current diagonal (1-based) */
    idx_t pos = n - prev;           /* 1 <= pos <= m */
    /* k = pos + 1, s = m + 2, n_k = s - k = (m + 1) - pos */
    idx_t k = pos + 1;
    idx_t n_k = (m + 1) - pos;

    /* Compute coefficient c(k) */
    seq_t coeff;
    if ((k & 1ULL) == 0) {
        /* even k */
        coeff = (seq_t)k * (k - 2) * (2 * k + 1) / 12;
    } else {
        /* odd k */
        coeff = (seq_t)(k * k - 1) * (2 * k - 3) / 12;
    }

    return coeff * n_k;
}
