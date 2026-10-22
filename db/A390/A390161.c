/*
 * A390161  Triangle read by rows: T(n,k) = ceiling(binomial(n, k) * 2^k /
 *          (2*n + 1)).
 * https://oeis.org/A390161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = ceil(binomial(r, k) * 2^k / (2*r + 1)) where r is the
 *            smallest integer with r*(r+1)/2 ≥ n, and k = n - (r-1)*r/2 (the
 *            column index within row r)
 * Range:     u8 n <= 23, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 ≥ n. */
    idx_t r = 0;
    while (r * (r + 1) / 2 < n)
        ++r;                 /* r now is the row index (1‑based) */

    /* Column index k (1‑based). */
    idx_t k = n - (r - 1) * r / 2;

    /* Compute ceil( binomial(r,k) * 2^k / (2*r + 1) ). */
    seq_t denom = (seq_t)(2 * r + 1);
    seq_t numer = binomial(r, k) * pow2(k);
    return (numer + denom - 1) / denom;
}
