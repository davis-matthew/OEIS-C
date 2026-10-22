/*
 * A370707  Triangle read by rows: T(n, k) = (-1)^k*Product_{j=0..k-1} (j -
 *          n)*(j + n), for 0 <= k <= n.
 * https://oeis.org/A370707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if n is a triangular number (n = t*(t+1)/2). Otherwise
 *            a(n) = ∏_{i=0}^{m} (k^2 - i^2), where k is the greatest integer
 *            with k*(k+1)/2 < n and m = n - k*(k+1)/2 - 1. The product is
 *            taken in type seq_t
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 34, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* Find k such that T_k = k*(k+1)/2 <= n < T_{k+1}. */
    idx_t k = (idx_t)((sqrt((double)(8.0 * (double)n + 1.0)) - 1.0) / 2.0);
    idx_t T = k * (k + 1) / 2;

    if (n == T)
        return 1;                       /* Triangular index */

    idx_t m = n - T - 1;
    seq_t result = 1;
    for (idx_t i = 0; i <= m; ++i) {
        seq_t term = (seq_t)(k * k - i * i);
        result *= term;
    }
    return result;
}
