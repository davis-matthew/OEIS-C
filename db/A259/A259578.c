/*
 * A259578  Reciprocity array of 2; rectangular, read by antidiagonals.
 * https://oeis.org/A259578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = read the reciprocity array of 2 in antidiagonal order *
 *            where T(m,n) = Sum_{k=0..m-1} floor((n*k+2)/m). The array is
 *            symmetric * and the sequence is obtained by traversing T(m,n) by
 *            antidiagonals
 * Range:     u8 n <= 546, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find diagonal d such that sum_{t=2..d} (t-1) >= n.
     * The cumulative count for diagonal d is (d-1)*d/2. */
    idx_t d = (idx_t)((sqrtl((long double)8 * n + 1) - 1) / 2);
    while ((unsigned __int128)d * (d - 1) / 2 < n) d++;
    while (d > 1 && (unsigned __int128)(d - 1) * (d - 2) / 2 >= n) d--;

    /* Compute offset inside this diagonal */
    idx_t prev = (d >= 2 ? (unsigned __int128)(d - 2) * (d - 1) / 2 : 0);
    idx_t offset = n - prev - 1;
    idx_t i = 1 + offset;
    idx_t j = d - i;                 /* 1-based indices for T(i,j) */

    /* Use symmetry: compute with the smaller dimension as divisor */
    idx_t m = i, n2 = j;
    if (m > n2) { idx_t t = m; m = n2; n2 = t; }

    seq_t sum = 0;
    for (idx_t k = 0; k < m; ++k) {
        seq_t val = (seq_t)n2 * (seq_t)k + 2; /* (n*k + 2) as seq_t */
        sum += val / m;                      /* floor division */
    }
    return sum;
}
