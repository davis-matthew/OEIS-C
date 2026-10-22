/*
 * A371077  Square array read by ascending antidiagonals: A(n, k) =
 *          3^n*Pochhammer(k/3, n).
 * https://oeis.org/A371077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is obtained by reading the square array * A(i,j) =
 *            ∏_{t=0}^{i-1} (3*t + j) (empty product = 1) * along ascending
 *            antidiagonals. * The linear index n is mapped to coordinates
 *            (i,j) by: * find the smallest s such that s*(s+1)/2 ≤ n <
 *            (s+1)*(s+2)/2 * offset = n - s*(s+1)/2 * i = s - offset, j =...
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 66, u64 n <= 153,
 *            u128 n <= 406, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    /* locate the diagonal 's' */
    double d = sqrt((double)n * 8.0 + 1.0);
    idx_t s = (idx_t)((d - 1.0) * 0.5);

    /* correct possible rounding errors */
    while ((s + 1) * (s + 2) / 2 <= n) ++s;
    while (s * (s + 1) / 2 > n) --s;

    /* compute offset within the diagonal */
    idx_t sum_prev = s * (s + 1) / 2;
    idx_t offset   = n - sum_prev;

    /* coordinates (i,j) in the array */
    idx_t i = s - offset;
    idx_t j = offset;

    /* product ∏_{t=0}^{i-1} (3*t + j) */
    seq_t prod = 1;
    for (idx_t t = 0; t < i; ++t) {
        prod *= (seq_t)(3 * t + j);
    }
    return prod;
}
