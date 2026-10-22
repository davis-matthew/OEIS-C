/*
 * A391308  Triangle read by rows: T(n,k) is the number of linear intervals of
 *          height k in the weak order on permutations of length n (0 <= k <
 *          n).
 * https://oeis.org/A391308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A391308.
   The sequence is the flattening of the triangle T(m,k) where
   0 ≤ k < m,
   T(m,0) = m!,
   T(m,1) = (m‑1)*m!/2,
   T(m,k) = 2*(m‑k)*m!/(k+1) for k > 1.
   The term a(n) is the (n‑((m‑1)*m/2)‑1)-th element of the m‑th row.
*/

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* behaviour below offset is irrelevant */

    /* Find the row m such that m*(m+1)/2 ≥ n.  Use a binary search
       to keep all arithmetic in 64‑bit with a 128‑bit guard. */
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid;
    }
    idx_t m = high;                    /* 1‑based row index */
    idx_t prev = (m - 1) * m / 2;      /* terms before this row */
    idx_t k = n - prev - 1;            /* 0‑based column within the row */

    /* Compute m! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= m; ++i)
        fact *= (seq_t)i;

    /* Evaluate the appropriate formula for T(m,k). */
    seq_t result;
    if (k == 0) {
        result = fact;
    } else if (k == 1) {
        result = fact * (seq_t)(m - 1) / 2;
    } else {
        result = fact * 2 * (seq_t)(m - k) / (seq_t)(k + 1);
    }
    return result;
}
