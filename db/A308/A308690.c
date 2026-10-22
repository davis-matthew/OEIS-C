/*
 * A308690  Square array A(n,k), n >= 1, k >= 0, where A(n,k) = Sum_{d|n}
 *          d^(k*n/d - k + 1), read by antidiagonals.
 * https://oeis.org/A308690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ∑_{d|r} d^((k*r)/d – k + 1) where (r,k) are the row and
 *            column of the n‑th element in the square array read by
 *            antidiagonals. The d‑sum runs over all divisors d of r
 * Range:     u8 n <= 50, u16 n <= 85, u32 n <= 162, u64 n <= 311,
 *            u128 n <= 578, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Find the diagonal t (1‑based) such that t(t+1)/2 ≥ n. */
    idx_t t = 0;
    idx_t cum = 0;
    while (cum < n) {
        ++t;
        cum += t;
    }

    /* pos = position within the current diagonal (1‑based). */
    idx_t prev_cum = cum - t;       /* cumulative count before diagonal t */
    idx_t pos = n - prev_cum;       /* 1‑based index inside diagonal */

    /* Coordinates of the array element: row r = pos, column k = t – pos. */
    idx_t r = pos;                  /* row index (≥1) */
    idx_t k = t - pos;              /* column index (≥0) */

    /* Compute the divisor sum. */
    seq_t sum = 0;
    for (idx_t d = 1; d <= r; ++d) {
        if (r % d == 0) {
            seq_t exp = (seq_t)k * (seq_t)(r / d) - (seq_t)k + 1;
            sum += big_pow((seq_t)d, exp);
        }
    }
    return sum;
}
