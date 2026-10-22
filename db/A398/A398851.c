/*
 * A398851  Upper (-1/2, 1/3)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((-1/2) * T(n) + (1/3) * P(n)) where T(n) = n*(n+1)/2
 *            (triangular number) and P(n) = n*(3*n-1)/2 (pentagonal number).
 *            This simplifies to a(n) = ceil(( -3*T(n) + 2*P(n) ) / 6)
 * Range:     u8 n <= 9, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* For n = 0 and 1 the value is 0, avoid underflows. */
    if (n <= 1) {
        return (seq_t)0;
    }

    /* Use seq_t for all computations to keep precision. */
    seq_t N   = (seq_t)n;
    seq_t T   = N * (N + (seq_t)1) / (seq_t)2;
    idx_t three_n_minus_one = (3*n - 1);
    seq_t P   = N * (seq_t)three_n_minus_one / (seq_t)2;

    /* Compute numerator of the rational expression: 2*P - 3*T */
    seq_t t2 = (seq_t)2 * P;
    seq_t t3 = (seq_t)3 * T;
    seq_t num = (t2 >= t3) ? (t2 - t3) : (seq_t)0;  /* non‑negative */

    /* Ceil division by 6: (num + 5) / 6 when num >= 0 */
    seq_t res = (num + (seq_t)5) / (seq_t)6;
    return res;
}
