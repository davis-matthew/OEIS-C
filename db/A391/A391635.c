/*
 * A391635  Triangle read by rows: T(n,k) = Sum_{j=0..k} (-1)^(k-j) *
 *          binomial(k,j) * (5+j)^n.
 * https://oeis.org/A391635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,c) where T(r,c) = Sum_{j=0..c} (-1)^(c-j) *
 *            binomial(c,j) * (5+j)^r and (r,c) are obtained from n by
 *            triangular numbering: n = r(r+1)/2 + c
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Determine row r and column c from flattened index n. */
    idx_t r = 0;
    idx_t m = n;
    while (m >= r + 1) {
        m -= r + 1;
        r++;
    }
    idx_t k = m;            /* column index k */

    seq_t pos = 0;
    seq_t neg = 0;

    for (idx_t j = 0; j <= k; ++j) {
        seq_t bin = binomial(k, j);                     /* binomial(k,j)  */
        seq_t pow = pow_seq((seq_t)(5 + j), (int)r);      /* (5+j)^r        */
        seq_t term = bin * pow;                           /* term value     */

        if (((k - j) & 1)) {         /* odd parity -> negative term */
            neg += term;
        } else {                     /* even parity -> positive term */
            pos += term;
        }
    }

    /* The result is always non‑negative (pos ≥ neg). */
    return pos - neg;
}
