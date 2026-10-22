/*
 * A344392  T(n, k) = k!*Stirling2(n - k, k), for n >= 0 and 0 <= k <=
 *          floor(n/2). Triangle read by rows.
 * https://oeis.org/A344392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = Σ_{i=0}^{k} (-1)^{k-i} * C(k,i) * i^{n-k} where the
 *            linear index n is mapped to the triangle T(row,k) = k! *
 *            Stirling2(row-k,k) with 0 ≤ k ≤ ⌊row/2⌋. The mapping from n to
 *            (row,k) uses the antidiagonal order of the triangle
 * Range:     u8 n <= 27, u16 n <= 52, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    /* Find the row corresponding to the linear index n. */
    idx_t q = (idx_t) sqrt((long double) n);
    while ((q + 1) * (q + 1) <= n) ++q;
    while (q * q > n) --q;

    idx_t row = (n < q * (q + 1)) ? (2 * q - 1) : (2 * q);

    /* Count the number of entries before this row. */
    idx_t count_before;
    if (row % 2 == 0) {
        /* row = 2 * m */
        idx_t m = row / 2;
        count_before = m * (m + 1);
    } else {
        /* row = 2 * m + 1 */
        idx_t m = (row + 1) / 2;
        count_before = m * m;
    }

    /* k is the column inside the row */
    idx_t k = n - count_before;
    /* exponent needed for the power term */
    idx_t exp = row - k;

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t i = 0; i <= k; ++i) {
        seq_t bin = binomial((idx_t)k, i);
        seq_t pow_val = seq_pow((seq_t)i, exp);
        seq_t term = bin * pow_val;
        if (((k - i) & 1) == 0) {
            sum_pos += term;
        } else {
            sum_neg += term;
        }
    }

    return sum_pos - sum_neg;
}
