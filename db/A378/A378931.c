/*
 * A378931  Triangle read by rows, based on products of Jacobsthal numbers
 *          (A001045).
 * https://oeis.org/A378931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) is the n‑th term of the triangle read by rows * where
 *            T(n,k) = (2*4^n - (-2)^n - 1)/9 when k == n * and T(n,k) =
 *            -2^(n-1-k) * (2^(k+1)+(-1)^k)^2 / 9 for 1 ≤ k < n
 * Range:     i8 n <= 5, i16 n <= 27, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Find the row r and column k such that a(n) = T(r,k). */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) r++;

    idx_t prev_sum = (r - 1) * r / 2;
    idx_t k = n - prev_sum;          /* 1‑based column index */

    /* Compute T(r,k) using the formulas above. */
    if (k == r) {
        /* Diagonal element: (2*4^r - (-2)^r - 1)/9 */
        seq_t term1 = (seq_t)2 * pow_seq((seq_t)4, (int)r);
        seq_t pow2r = pow_seq((seq_t)2, (int)r);
        if (r & 1) pow2r = -pow2r;   /* (-2)^r */
        seq_t numer = term1 - pow2r - (seq_t)1;
        return numer / (seq_t)9;
    } else {
        /* Off‑diagonal element: -2^(r-1-k) * (2^(k+1)+(-1)^k)^2 / 9 */
        seq_t exp1 = (seq_t)(r - 1 - k);
        seq_t pow2_exp1 = pow_seq((seq_t)2, (int)exp1);

        seq_t pow2_kplus1 = pow_seq((seq_t)2, (int)(k + 1));
        seq_t signk = (k & 1) ? (seq_t)-1 : (seq_t)1;
        seq_t inner = pow2_kplus1 + signk;
        seq_t inner_sq = inner * inner;

        seq_t numer = -pow2_exp1 * inner_sq;
        return numer / (seq_t)9;
    }
}
