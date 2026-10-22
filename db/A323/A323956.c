/*
 * A323956  Triangle read by rows: T(n, k) = 1 + n * (n - k) for 1 <= k <= n.
 * https://oeis.org/A323956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 + m*(m - (n - m*(m-1)/2)), where m = smallest integer
 *            with m*(m+1)/2 ≥ n (i.e., the row number of the n‑th element
 *            when the triangle T(i,j)=1+i*(i-j) is read row by row)
 * Range:     u8 n <= 136, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row m such that m*(m+1)/2 ≥ n */
    idx_t low  = 1;
    idx_t high = 1;
    while ( ((unsigned __int128)high * (high + 1) / 2) < n )
        high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    idx_t m = low;                       /* row number */

    /* Compute column index k = n - previous row sum */
    idx_t prev = (m - 1) * m / 2;
    idx_t k    = n - prev;

    /* T(m,k) = 1 + m*(m-k) */
    seq_t val = (seq_t)1 + (seq_t)m * (seq_t)(m - k);
    return val;
}
