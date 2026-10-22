/*
 * A378068  Table read by row: T(n, k) = Sum_{j=0..k} A217831(n, j). Partial
 *          row sums of Euclid's triangle.
 * https://oeis.org/A378068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = |{ j | 0 ≤ j ≤ k and gcd(j, r) = 1 }|, where r and k are
 *            chosen by writing the integer n in the row‑major order of
 *            Euclid's triangle: r = largest integer such that r*(r+1)/2 ≤ n k
 *            = n – r*(r+1)/2 (The rows are numbered from 0 and each row r has
 *            r+1 entries.)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    idx_t r = 0;
    /* Find the row r.  The loop is safe because r ≤ sqrt(2*n),
       and the product is computed in 128‑bit to avoid overflow. */
    while (( (__uint128_t)(r + 1) * (r + 2) / 2 ) <= n) ++r;

    /* Position of the first element of row r. */
    idx_t row_start = ( (__uint128_t)r * (r + 1) / 2 );
    /* Offset within the row. */
    idx_t k = n - row_start;

    seq_t count = 0;
    for (idx_t j = 0; j <= k; ++j) {
        if (gcd((seq_t)r, (seq_t)j) == (seq_t)1) ++count;
    }
    return count;
}
